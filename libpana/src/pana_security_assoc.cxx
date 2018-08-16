/* BEGIN_COPYRIGHT                                                        */
/*                                                                        */
/* Open Diameter: Open-source software for the Diameter and               */
/*                Diameter related protocols                              */
/*                                                                        */
/* Copyright (C) 2002-2004 Open Diameter Project                          */
/*                                                                        */
/* This library is free software; you can redistribute it and/or modify   */
/* it under the terms of the GNU Lesser General Public License as         */
/* published by the Free Software Foundation; either version 2.1 of the   */
/* License, or (at your option) any later version.                        */
/*                                                                        */
/* This library is distributed in the hope that it will be useful,        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      */
/* Lesser General Public License for more details.                        */
/*                                                                        */
/* You should have received a copy of the GNU Lesser General Public       */
/* License along with this library; if not, write to the Free Software    */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307    */
/* USA.                                                                   */
/*                                                                        */
/* In addition, when you copy and redistribute some or the entire part of */
/* the source code of this software with or without modification, you     */
/* MUST include this copyright notice in each copy.                       */
/*                                                                        */
/* If you make any changes that are appeared to be useful, please send    */
/* sources that include the changed part to                               */
/* diameter-developers@lists.sourceforge.net so that we can reflect your  */
/* changes to one unified version of this software.                       */
/*                                                                        */
/* END_COPYRIGHT                                                          */

#include "od_utl_sha1.h"
#include "pana_security_assoc.h"
#include "pana_exceptions.h"
#include "pana_memory_manager.h"

#define PANA_SA_DEBUG   0

void PANA_MacKey::Generate(PANA_Nonce &pac,
                           PANA_Nonce &paa,
                           diameter_octetstring_t &aaaKey,
                           diameter_utf8string_t &sessionId)
{
    //
    // The PANA_MAC_Key is used to integrity protect PANA messages and
    // derived from AAA-Key(s).  When two AAA-Keys (AAA-Key1 and AAA-Key2)
    // are generated as a result of double EAP authentication (see Section
    // 4.3) the compound AAA-Key can be computed as follows ('|' indicates
    // concatenation):
    //
    //    AAA-Key = AAA-Key1 | AAA-Key2
    //
    //    PANA_MAC_KEY = The first N bits of
    //              HMAC_SHA1(AAA-Key, PaC_nonce | PAA_nonce | Session-ID)
    //
    // where the value of N depends on the integrity protection algorithm in
    // use, i.e., N=160 for HMAC-SHA1.  The length of AAA-Key MUST be N bits
    // or longer.  See Section Section 4.1.6 for the detailed usage of the
    // PANA_MAC_KEY.
    //
#if PANA_SA_DEBUG
    printf("AAA key[%d]: ", aaaKey.size());
    for (size_t i=0; i<aaaKey.size(); i++) {
        printf("%02X ", (unsigned char)((char*)aaaKey.data())[i]);
    }
    printf("\n");
    printf("Pac Nonce[%d]: ", pac.Get().size());
    for (size_t i=0; i<pac.Get().size(); i++) {
        printf("%02X ", (unsigned char)((char*)pac.Get().data())[i]);
    }
    printf("\n");
    printf("Paa Nonce[%d]: ", paa.Get().size());
    for (size_t i=0; i<paa.Get().size(); i++) {
        printf("%02X ", (unsigned char)((char*)paa.Get().data())[i]);
    }
    printf("\n");
#endif

    OD_Utl_Sha1 sha1;
    sha1.Update((unsigned char*)aaaKey.data(), aaaKey.size());
    sha1.Update((unsigned char*)pac.Get().data(), pac.Get().size());
    sha1.Update((unsigned char*)paa.Get().data(), paa.Get().size());
    sha1.Update((unsigned char*)sessionId.data(), sessionId.size());
    sha1.Final();

    char sbuf[PANA_MACGEN_HMACSIZE];
    ACE_OS::memset(sbuf, 0x0, sizeof(sbuf));
    sha1.GetHash((unsigned char*)sbuf);

    m_Value.assign((char*)(sbuf), sizeof(sbuf));
    m_IsSet = true;

#if PANA_SA_DEBUG
    printf("Mac key[%d]: ", m_Value.size());
    for (size_t i=0; i<m_Value.size(); i++) {
        printf("%02X ", (unsigned char)((char*)m_Value.data())[i]);
    }
    printf("\n");
#endif
}

void PANA_SecurityAssociation::GenerateMacKey
    (diameter_utf8string_t &sessionId) 
{
    if (m_Type == DOUBLE) {
        diameter_octetstring_t combinedKey;
        if (! m_AAAKey1.IsSet()) {
            throw (PANA_Exception(PANA_Exception::FAILED, 
                   "Mac key generation failed, no keys present"));
        }
        combinedKey = m_AAAKey1.Get();
        if (m_AAAKey2.IsSet()) {
            combinedKey += m_AAAKey2.Get();
        }
        Set(combinedKey);
    }
    m_MacKey.Generate(m_PacNonce,
                      m_PaaNonce,
                      m_Value,
                      sessionId);
}

void PANA_SecurityAssociation::GenerateMacAvpValue
           (const char *PDU,
            ACE_UINT32 PDULength,
            diameter_octetstring_t &macValue)
{
    //
    //   A PANA message can contain a MAC (Message Authentication Code) AVP 
    //   for cryptographically protecting the message. 
    //
    //   When a MAC AVP is included in a PANA message, the value field of the 
    //   MAC AVP is calculated by using the PANA_MAC_Key in the following 
    //   way: 
    //
    //       MAC AVP value = HMAC_SHA1(PANA_MAC_Key, PANA_PDU) 
    //
    //   where PANA_PDU is the PANA message including the PANA header, with 
    //   the MAC AVP value field first initialized to 0.  
    //    
    OD_Utl_Sha1 sha1;
    sha1.Update((unsigned char*)m_MacKey.Get().data(), 
                 m_MacKey.Get().size());
    sha1.Update((unsigned char*)PDU, PDULength);
    sha1.Final();

    char sbuf[PANA_MACGEN_HMACSIZE];
    ACE_OS::memset(sbuf, 0x0, sizeof(sbuf));
    sha1.GetHash((unsigned char*)sbuf);
    macValue.assign((char*)sbuf, sizeof(sbuf));

#if PANA_SA_DEBUG
    printf("Generate - Mac Key[%d]: ", m_MacKey.Get().size());
    for (size_t i=0; i<m_MacKey.Get().size(); i++) {
        printf("%02X ", (unsigned char)((char*)m_MacKey.Get().data())[i]);
    }
    printf("\n");
    
    printf("Generate - PDU [%d]: ", PDULength);
    for (size_t i=0; i<PDULength; i++) {
        printf("%02X ", (unsigned char)PDU[i]);
    }
    printf("\n");

    printf("Generate - MAC Value [%d]: ", macValue.size());
    for (size_t i=0; i<macValue.size(); i++) {
        printf("%02X ", (unsigned char)((char*)macValue.data())[i]);
    }
    printf("\n");
#endif
}

bool PANA_SecurityAssociation::AddKeyIdAvp(PANA_Message &msg)
{
    ACE_UINT32 keyId;
    if (m_Type == DOUBLE) {
        if (! m_AAAKey1.IsSet()) {
            return (false);
        }
        else if (! m_AAAKey2.IsSet()) {
            keyId = m_AAAKey1.Id();
        }
        else {
            keyId = m_AAAKey2.Id();
        }
    }
    else {
        return (false);
    }
    AAA_UInt32AvpWidget keyIdAvp(PANA_AVPNAME_KEYID);
    keyIdAvp.Get() = keyId;
    msg.avpList().add(keyIdAvp());
    return (true);
}

bool PANA_SecurityAssociation::AddMacAvp(PANA_Message &msg)
{
    // add mac-avp
    PANA_MacAvpWidget macAvp(PANA_AVPNAME_MAC);
    PANA_TVData_t &mac = macAvp.Get();
    msg.avpList().add(macAvp());

    // init this to zero so we can compute true MAC
    char sbuf[PANA_MACGEN_HMACSIZE];
    ACE_OS::memset(sbuf, 0x0, sizeof(sbuf));
    mac.value.assign(sbuf, sizeof(sbuf));
    mac.type = PANA_MACGEN_HMACSHA1;

    PANA_MessageBuffer *rawBuf = PANA_MESSAGE_POOL()->malloc();

    PANA_HeaderParser hp;
    AAADictionaryOption opt(PARSE_STRICT, PANA_DICT_PROTOCOL_ID);
    hp.setRawData(reinterpret_cast<AAAMessageBlock*>(rawBuf));
    hp.setAppData(static_cast<PANA_MsgHeader*>(&msg));
    hp.setDictData(&opt);

    hp.parseAppToRaw();

    // Parse the payload
    PANA_PayloadParser pp;
    pp.setRawData(reinterpret_cast<AAAMessageBlock*>(rawBuf));
    pp.setAppData(&(msg.avpList()));
    pp.setDictData(msg.getDictHandle());

    pp.parseAppToRaw();

    // Re-do the header again to set the length
    msg.length() = rawBuf->wr_ptr() - rawBuf->base();
    hp.parseAppToRaw();

    // generate mac value
    GenerateMacAvpValue(rawBuf->base(), 
                        msg.length(),
                        mac.value);
    PANA_MESSAGE_POOL()->free(rawBuf);
    msg.avpList().reset();

    return (true);
}

bool PANA_SecurityAssociation::ValidateMacAvp(PANA_Message &msg)
{
    try {
        PANA_MacAvpContainerWidget macAvp(msg.avpList());
        PANA_TVData_t *mac = macAvp.GetAvp(PANA_AVPNAME_MAC);
        if (mac == NULL) {
            throw (PANA_Exception(PANA_Exception::FAILED, 
                                  "Missing MAC-AVP"));
        }

        // verify type
        if (mac->type != PANA_MACGEN_HMACSHA1) {
            throw (PANA_Exception(PANA_Exception::FAILED, 
                   "Warning: MAC algorithm not supported"));
        }

        // temporarily save the MAC value and insert 0 mac
        diameter_octetstring_t mvalue;
        mvalue.assign(mac->value.data(), mac->value.size());

#if PANA_SA_DEBUG
        printf("Validate - Received mac[%d]: ", mvalue.size());
        for (size_t i=0; i<mvalue.size(); i++) {
            printf("%02X ", (unsigned char)((char*)mvalue.data())[i]);
        }
        printf("\n");
#endif

        // reset to zero
        char sbuf[PANA_MACGEN_HMACSIZE];
        ACE_OS::memset(sbuf, 0x0, sizeof(sbuf));
        mac->value.assign(sbuf, sizeof(sbuf));
        mac->type = PANA_MACGEN_HMACSHA1;

        PANA_MessageBuffer *aBuffer = PANA_MESSAGE_POOL()->malloc();
        msg.avpList().reset();

        // parse the message 
        PANA_HeaderParser hp;
        AAADictionaryOption opt(PARSE_STRICT, PANA_DICT_PROTOCOL_ID);
        hp.setRawData(reinterpret_cast<AAAMessageBlock*>(aBuffer));
        hp.setAppData(static_cast<PANA_MsgHeader*>(&msg));
        hp.setDictData(&opt);

        hp.parseAppToRaw();

        // Parse the payload
        PANA_PayloadParser pp;
        pp.setRawData(reinterpret_cast<AAAMessageBlock*>(aBuffer));
        pp.setAppData(&(msg.avpList()));
        pp.setDictData(msg.getDictHandle());

        pp.parseAppToRaw();

        // Re-do the header again to set the length
        msg.length() = aBuffer->wr_ptr() - aBuffer->base();
        hp.parseAppToRaw();

        GenerateMacAvpValue(aBuffer->base(), 
                            msg.length(),
                            mac->value);        
        PANA_MESSAGE_POOL()->free(aBuffer);

#if PANA_SA_DEBUG
        printf("Validate - Mac Key[%d]: ", m_MacKey.Get().size());
        for (size_t i=0; i<m_MacKey.Get().size(); i++) {
            printf("%02X ", (unsigned char)((char*)m_MacKey.Get().data())[i]);
        }
        printf("\n");
        
        printf("Validate - PDU [%d]: ", aBuffer->length());
        for (size_t i=0; i<aBuffer->length(); i++) {
            printf("%02X ", (unsigned char)((char*)aBuffer->base())[i]);
        }
        printf("\n");

        printf("Validate - Computed MAC Value [%d]: ", mac->value.size());
        for (size_t i=0; i<mac->value.size(); i++) {
            printf("%02X ", (unsigned char)((char*)mac->value.data())[i]);
        }
        printf("\n");
#endif

        // do comparison
        if (mac->value == mvalue) {
            return (true);
        }
        ACE_DEBUG((LM_ERROR, "(%P|%t) MAC value is invalid\n"));
    }
    catch (AAAErrorStatus st) {
        ACE_DEBUG((LM_ERROR, "(%P|%t) Parsing error is session transmitter\n"));
    }  
    catch (PANA_Exception &e) {
        ACE_DEBUG((LM_ERROR, "(%P|%t) %s\n", e.description().data()));
    }
    return (false);
}

