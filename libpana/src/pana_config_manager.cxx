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

#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "pana_config_manager.h"
#include "pana_defs.h"
#include "pana_exceptions.h"
#include "od_utl_xml_parser.h"

class PANA_XMLData : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLData(std::string &name, PANA_Cfg &data) :
                   OD_Utl_XMLElementParser(name),
	               payload(data) { }
      int svc(DOMNode *n);
   protected:
      PANA_Cfg &payload;
};

class PANA_XMLDataGeneral : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLDataGeneral(std::string &name, PANA_CfgGeneral &data) :
                          OD_Utl_XMLElementParser(name),
	                      payload(data) { }
      void print();
      int svc(DOMNode *n);
   protected:
      PANA_CfgGeneral &payload;
};

class PANA_XMLRetransmissionParam : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLRetransmissionParam(std::string &name,
                                  PANA_CfgRetransmissionParam &data) :
                           OD_Utl_XMLElementParser(name),
                           payload(data) { }
      void print();
      int svc(DOMNode *n);
   protected:
      PANA_CfgRetransmissionParam &payload;
};

class PANA_XMLProviderInfo : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLProviderInfo(std::string &name, PANA_CfgProviderInfo &data) :
                           OD_Utl_XMLElementParser(name),
                           payload(data) { }
      void print();
      int svc(DOMNode *n);
      std::string &banner() { 
          return banner_; 
      }
   protected:
      std::string banner_;
      PANA_CfgProviderInfo &payload;
};

class PANA_XMLPPAC : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLPPAC(std::string &name, PANA_CfgPPAC &data) :
                   OD_Utl_XMLElementParser(name),
                   payload(data) { }
      void print();
      int svc(DOMNode *n);
      std::string &banner() { 
          return banner_; 
      }
   protected:
      std::string banner_;
      PANA_CfgPPAC &payload;
};

class PANA_XMLDeviceId : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLDeviceId(std::string &name, PANA_DeviceId &data) :
                       OD_Utl_XMLElementParser(name),
                       payload(data) { }
      void print();
      int svc(DOMNode *n);
      std::string &banner() { 
          return banner_; 
      }
   protected:
      std::string banner_;
      PANA_DeviceId &payload;
};

class PANA_XMLDataClient : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLDataClient(std::string &name, PANA_CfgClient &data) :
 	                   OD_Utl_XMLElementParser(name),
	                   payload(data) { }
      int svc(DOMNode *n);
      void print();
   protected:
      PANA_CfgClient &payload;
};

class PANA_XMLDataAuthAgent : public OD_Utl_XMLElementParser {
   public:
      PANA_XMLDataAuthAgent(std::string &name, PANA_CfgAuthAgent &data) :
 	                   OD_Utl_XMLElementParser(name),
	                   payload(data) { }
      int svc(DOMNode *n);
      void print();
   protected:
      PANA_CfgAuthAgent &payload;
};

void PANA_XMLDataGeneral::print()
{
    ACE_DEBUG((LM_INFO, "     General configuration\n"));
    ACE_DEBUG((LM_INFO, "          Listen Port     : %d\n", payload.m_ListenPort));
    ACE_DEBUG((LM_INFO, "          Interface name  : %s\n", payload.m_Interface.data()));
    ACE_DEBUG((LM_INFO, "          IPv6            : %d\n", payload.m_IPv6Enabled));
    ACE_DEBUG((LM_INFO, "          Protection Cap  : %d\n", payload.m_ProtectionCap));
    ACE_DEBUG((LM_INFO, "          Dictionary      : %s\n", payload.m_Dictionary.data()));
    ACE_DEBUG((LM_INFO, "          Mobility        : %d\n", payload.m_MobilityEnabled));
    ACE_DEBUG((LM_INFO, "          Separate NAP/ISP: %d\n", payload.m_SeparateAuth));
    ACE_DEBUG((LM_INFO, "          Abort on 1st EAP: %d\n", payload.m_AbortOnFirstEapFailure));
    ACE_DEBUG((LM_INFO, "          EAP Piggyback   : %d\n", payload.m_EapPiggyback));
}

int PANA_XMLDataGeneral::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "listen_port";
   OD_Utl_XMLDataUInt32 listen_port(tagName, payload.m_ListenPort);
   listen_port.populate(n->getFirstChild());

   tagName = "interface_name";
   OD_Utl_XMLDataString iface_name(tagName, payload.m_Interface);
   iface_name.populate(n->getFirstChild());

   tagName = "ipv6";
   OD_Utl_XMLDataUInt32 ipv6(tagName, payload.m_IPv6Enabled);
   ipv6.populate(n->getFirstChild());

   tagName = "protection_capability";
   OD_Utl_XMLDataUInt32 pcap(tagName, payload.m_ProtectionCap);
   pcap.populate(n->getFirstChild());

   tagName = "dictionary_filename";
   OD_Utl_XMLDataString dict(tagName, payload.m_Dictionary);
   dict.populate(n->getFirstChild());

   tagName = "mobility";
   OD_Utl_XMLDataUInt32 mob(tagName, payload.m_MobilityEnabled);
   mob.populate(n->getFirstChild());

   tagName = "separate";
   OD_Utl_XMLDataUInt32 separate(tagName, payload.m_SeparateAuth);
   separate.populate(n->getFirstChild());

   tagName = "abort_on_first_eap_failure";
   OD_Utl_XMLDataUInt32 abort(tagName, payload.m_AbortOnFirstEapFailure);
   abort.populate(n->getFirstChild());

   tagName = "eap_piggyback";
   OD_Utl_XMLDataUInt32 ep(tagName, payload.m_EapPiggyback);
   ep.populate(n->getFirstChild());

   tagName = "keep_alive_interval";
   OD_Utl_XMLDataUInt32 keepAlive(tagName, payload.m_KeepAliveInterval);
   keepAlive.populate(n->getFirstChild());

   tagName = "wpa_bootstrap";
   OD_Utl_XMLDataUInt32 wpa(tagName, payload.m_WPASupport);
   wpa.populate(n->getFirstChild());

   print();
   
   tagName = "retransmission_param";
   PANA_XMLRetransmissionParam rt(tagName, payload.m_RT);
   rt.populate(n->getFirstChild());

   tagName = "popa_config";
   PANA_XMLPPAC popa(tagName, payload.m_PPAC);
   popa.populate(n->getFirstChild());   

   return (0);
}

void PANA_XMLRetransmissionParam::print()
{
    ACE_DEBUG((LM_INFO, "          Re-Transmission\n"));
    ACE_DEBUG((LM_INFO, "                     IRT  : %d\n", payload.m_IRT));
    ACE_DEBUG((LM_INFO, "                     MRC  : %d\n", payload.m_MRC));
    ACE_DEBUG((LM_INFO, "                     MRT  : %d\n", payload.m_MRT));
    ACE_DEBUG((LM_INFO, "                     MRD  : %d\n", payload.m_MRD));
}

int PANA_XMLRetransmissionParam ::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "initial_rt_timeout";
   OD_Utl_XMLDataUInt32 irt(tagName, payload.m_IRT);
   irt.populate(n->getFirstChild());

   tagName = "max_rt_count";
   OD_Utl_XMLDataUInt32 mrc(tagName, payload.m_MRC);
   mrc.populate(n->getFirstChild());

   tagName = "max_rt_timeout";
   OD_Utl_XMLDataUInt32 mrt(tagName, payload.m_MRT);
   mrt.populate(n->getFirstChild());

   tagName = "max_rt_duration";
   OD_Utl_XMLDataUInt32 mrd(tagName, payload.m_MRD);
   mrd.populate(n->getFirstChild());

   print();

   return (0);
}

void PANA_XMLProviderInfo::print()
{
    ACE_DEBUG((LM_INFO, "        Provider Info [%s]:\n", banner_.data()));
    ACE_DEBUG((LM_INFO, "                     Name : %s\n", payload.m_Name.data()));
    ACE_DEBUG((LM_INFO, "                     ID   : %d\n", payload.m_Id));
}

int PANA_XMLProviderInfo ::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "id";
   OD_Utl_XMLDataUInt32 id(tagName, payload.m_Id);
   id.populate(n->getFirstChild());

   tagName = "name";
   OD_Utl_XMLDataString name(tagName, payload.m_Name);
   name.populate(n->getFirstChild());

   print();

   return (0);
}

void PANA_XMLPPAC::print()
{
    ACE_DEBUG((LM_INFO, "                      PPAC\n"));
    ACE_DEBUG((LM_INFO, "                No Config : %d\n", 
        payload.Get().u.m_NoConfig));
    ACE_DEBUG((LM_INFO, "                     DHCP : %d\n", 
        payload.Get().u.m_Dhcp));
    ACE_DEBUG((LM_INFO, "                Stateless : %d\n", 
        payload.Get().u.m_Stateless));
    ACE_DEBUG((LM_INFO, "                   Tunnel : %d\n", 
        payload.Get().u.m_Tunnel));
    ACE_DEBUG((LM_INFO, "                    IKEv2 : %d\n", 
        payload.Get().u.m_IKEv2));
}

int PANA_XMLPPAC ::svc(DOMNode *n)
{
   ACE_UINT32 flg;
   std::string tagName;

   tagName = "dhcp";
   OD_Utl_XMLDataUInt32 dhcp(tagName, flg);
   dhcp.populate(n->getFirstChild());
   payload.Get().u.m_Dhcp = flg;

   tagName = "stateless";
   OD_Utl_XMLDataUInt32 stateless(tagName, flg);
   stateless.populate(n->getFirstChild());
   payload.Get().u.m_Stateless = flg;

   tagName = "tunnel";
   OD_Utl_XMLDataUInt32 tunnel(tagName, flg);
   tunnel.populate(n->getFirstChild());
   payload.Get().u.m_Tunnel = flg;

   tagName = "ikev2";
   OD_Utl_XMLDataUInt32 ikev2(tagName, flg);
   ikev2.populate(n->getFirstChild());
   payload.Get().u.m_IKEv2 = flg;

   payload.Get().u.m_NoConfig = (payload.Get().v > 0) ? 
                                 false : true;

   print();

   return (0);
}

void PANA_XMLDeviceId::print()
{
   ACE_DEBUG((LM_INFO, "          EP Device Id    : %d, ",
              payload.type));
}

int PANA_XMLDeviceId::svc(DOMNode *n)
{
   std::string tagName;
   std::string id;
   
   tagName = "type";
   ACE_UINT32 type;
   OD_Utl_XMLDataUInt32 ptype(tagName, type);
   ptype.populate(n->getFirstChild());
   payload.type = type;

   tagName = "id";
   OD_Utl_XMLDataString pid(tagName, id);
   pid.populate(n->getFirstChild());

   print();
   ACE_DEBUG((LM_INFO, "[%s]\n", id.data()));

   char buf[256];
   if ((payload.type != AAA_ADDR_FAMILY_IPV4) &&
       (payload.type != AAA_ADDR_FAMILY_IPV6)) {
       int len = PANA_DeviceIdConverter::AsciiToHex
           (id.data(), (unsigned char*)buf);
       if (len > 0) {
           payload.value.assign(buf, len);
       }
       else {
           return (-1);
       }
   }
   else {
       ACE_OS::sprintf(buf, "%s:0", id.data());
       ACE_INET_Addr addr(buf);   
       PANA_DeviceIdConverter::PopulateFromAddr(addr, payload);
   }
   
   return (0);
}

void PANA_XMLDataClient::print()
{
    ACE_DEBUG((LM_INFO, "     Client configuration\n"));
    ACE_DEBUG((LM_INFO, "          PAA IP Adress   : %s\n", payload.m_PaaIpAddress.data()));
    ACE_DEBUG((LM_INFO, "          PAA Port Number : %d\n", payload.m_PaaPortNumber));
    ACE_DEBUG((LM_INFO, "          PAA Mcast Addr  : %s\n", payload.m_PaaMcastAddress.data()));
}

int PANA_XMLDataClient::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "paa_ip_address";
   OD_Utl_XMLDataString paa_ip(tagName, payload.m_PaaIpAddress);
   paa_ip.populate(n->getFirstChild());

   tagName = "paa_port_number";
   OD_Utl_XMLDataUInt32 paa_port(tagName, payload.m_PaaPortNumber);
   paa_port.populate(n->getFirstChild());

   tagName = "paa_mcast_address";
   OD_Utl_XMLDataString paa_mcast(tagName, payload.m_PaaMcastAddress);
   paa_mcast.populate(n->getFirstChild());

   print();

   tagName = "isp_info";
   PANA_XMLProviderInfo isp(tagName, payload.m_IspInfo);
   isp.banner() = "ISP Info";
   isp.populate(n->getFirstChild());

   return (0);
}

void PANA_XMLDataAuthAgent::print()
{
    ACE_DEBUG((LM_INFO, "     Auth Agent configuration\n"));
    ACE_DEBUG((LM_INFO, "          Use Cookie      : %d\n", payload.m_UseCookie));
    ACE_DEBUG((LM_INFO, "          Session-Lifetime: %d\n", payload.m_SessionLifetime));
    ACE_DEBUG((LM_INFO, "          Grace Period    : %d\n", payload.m_GracePeriod));
    ACE_DEBUG((LM_INFO, "          Multicast Addr  : %s\n", payload.m_McastAddress.data()));
}

int PANA_XMLDataAuthAgent::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "use_cookie";
   OD_Utl_XMLDataUInt32 ck(tagName, payload.m_UseCookie);
   ck.populate(n->getFirstChild());

   tagName = "session_lifetime";
   OD_Utl_XMLDataUInt32 sessLifetime(tagName, payload.m_SessionLifetime);
   sessLifetime.populate(n->getFirstChild());

   tagName = "grace_period";
   OD_Utl_XMLDataUInt32 gracePeriod(tagName, payload.m_GracePeriod);
   gracePeriod.populate(n->getFirstChild());

   tagName = "mcast_address";
   OD_Utl_XMLDataString mcast(tagName, payload.m_McastAddress);
   mcast.populate(n->getFirstChild());

   tagName = "carry_device_id";
   OD_Utl_XMLDataUInt32 carryId(tagName, payload.m_CarryDeviceId);
   carryId.populate(n->getFirstChild());

   print();

   tagName = "nap_info";
   PANA_XMLProviderInfo nap(tagName, payload.m_NapInfo);
   nap.banner() = "NAP Info";
   nap.populate(n->getFirstChild());

   ACE_UINT32 ndx = 0;
   char buf[32];

   tagName = "isp_info";
   DOMNode *sibling = n->getFirstChild();
   while (sibling != NULL) {

     if (sibling->getNodeType() == DOMNode::ELEMENT_NODE) {

         char *c_str = XMLString::transcode(sibling->getNodeName());

         if (XMLString::compareString(c_str, tagName.c_str()) == 0) {

             PANA_CfgProviderInfo *pInfo;
             ACE_NEW_NORETURN(pInfo, PANA_CfgProviderInfo);

             PANA_XMLProviderInfo isp(tagName, *pInfo);
	     ACE_OS::sprintf(buf, "%s #%d", "ISP Info", ++ ndx);
             isp.banner() = buf;
             if (isp.populate(sibling) == 0) {
	         payload.m_IspInfo.push_back(pInfo);
	     }
             else {
                 delete pInfo;
             }
         }

         XMLString::release(&c_str);
      }
      sibling = sibling->getNextSibling();
   }

   tagName = "ep_id";
   sibling = n->getFirstChild();
   while (sibling != NULL) {

     if (sibling->getNodeType() == DOMNode::ELEMENT_NODE) {

         char *c_str = XMLString::transcode(sibling->getNodeName());

         if (XMLString::compareString(c_str, tagName.c_str()) == 0) {

             PANA_DeviceId *devId = new PANA_DeviceId;
             PANA_XMLDeviceId pId(tagName, *devId);
             if (pId.populate(sibling) == 0) {
	             payload.m_EpIdList.push_back(devId);                 
             }
             else {
                 delete devId;
             }
         }
         XMLString::release(&c_str);
      }
      sibling = sibling->getNextSibling();
   }
   return (0);
}

int PANA_XMLData::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "general";
   PANA_XMLDataGeneral general(tagName, payload.m_General);
   general.populate(n->getFirstChild());

   tagName = "client";
   PANA_XMLDataClient client(tagName, payload.m_PaC);
   client.populate(n->getFirstChild());

   tagName = "auth_agent";
   PANA_XMLDataAuthAgent aa(tagName, payload.m_PAA);
   aa.populate(n->getFirstChild());

   return (0);
}

PANA_CfgManager::PANA_CfgManager()
{
}

PANA_CfgManager::~PANA_CfgManager()
{
    // do nothing
}

void PANA_CfgManager::open(std::string &cfg_file)
{
    std::string cfgRoot = "pana_configuration";

    OD_Utl_XMLTreeParser parser;
    PANA_XMLData configData(cfgRoot, m_Data);

    if (parser.open(cfg_file, configData) != 0) {
        throw (PANA_Exception(PANA_Exception::CONFIG_ERROR, 
                              "Fatal: Unable to parse XML config file"));
    }
}

void PANA_CfgManager::close()
{
   // free up allocated provider list
   PANA_CfgProviderInfo *i;
   PANA_CfgProviderList &ispList = m_Data.m_PAA.m_IspInfo;

   while (! ispList.empty()) {
      i = ispList.front();
      if (i) {
         delete i;
      }
      ispList.pop_front();
   }
}
