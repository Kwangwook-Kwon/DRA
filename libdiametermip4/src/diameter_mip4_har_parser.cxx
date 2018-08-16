/* BEGIN_COPYRIGHT                                                        */
/*                                                                        */
/* OpenDiameter: Open-source software for the Diameter protocol           */
/*                                                                        */
/* Copyright (C) 2002-2007 Open Diameter Project.                         */
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
/* 
   diameter_mip4_har_parser.cxx
   Diameter MIP HAR Request Parser
   Written by Miriam Tauil
   Created May 25, 2004.
*/

#include "diameter_mip4_parser.hxx"

template<> void 
HAR_Parser::parseAppToRaw()
{
  HAR_Data &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  
  DiameterDictionaryManager dm;
  DiameterAvpContainerManager cm;
  AAAAvpContainer *c;
                          
  AAACommandCode code;
  DiameterApplicationId appId;

  // Obtain Command Code and Application Identifier.
  if (!dm.getCommandCode("HomeAgentMIPRequest", &code, &appId)) 
    {
      AAA_LOG((LM_ERROR, "[%N] Cannot find message in dictionary\n."));
      throw (DIAMETER_DICTIONARY_ERROR);
    }

  // Specify the header.
  diameter_hdr_flag flag = {1,0,0};
  DiameterMsgHeader mnHdr(1, 0, flag, code, appId, 0, 0);
  aaaMessage.hdr = mnHdr;

  if (data.AuthApplicationId.IsSet())
    {
      c = cm.acquire("AuthApplicationId");
      data.AuthApplicationId.CopyTo(*c, AAA_AVP_INTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }

 if (data.AuthorizationLifetime.IsSet())
    { 
      c = cm.acquire("AuthorizationLifetime");
      data.AuthorizationLifetime.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  
  if (data.AuthSessionState.IsSet())
    { 
      c = cm.acquire("AuthSessionState");
      data.AuthSessionState.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.MipRegRequest.IsSet())			
    {
      c = cm.acquire("MIPRegRequest");
      data.MipRegRequest.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }

 
  if (data.OriginHost.IsSet())
    {
      c = cm.acquire("OriginHost");
      data.OriginHost.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }

  if (data.OriginRealm.IsSet())
    {
      c = cm.acquire("OriginRealm");
      data.OriginRealm.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }

  if (data.UserName.IsSet())
    { 
      c = cm.acquire("UserName");
      data.UserName.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
if (data.DestinationRealm.IsSet())
    {
      c = cm.acquire("DestinationRealm");
      data.DestinationRealm.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
	}
	
   if (data.MipFeatureVector.IsSet())
    { 
      c = cm.acquire("MIPFeatureVector");
      data.MipFeatureVector.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }	
	//optional AVPs

  if (data.DestinationHost.IsSet())
    { 
      c = cm.acquire("DestinationHost");
      data.DestinationHost.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.MipMnToHaMsa.IsSet())
    { 
      c = cm.acquire("MIPMNtoHAMSA");
      data.MipMnToHaMsa.CopyTo(*c); //, AAA_AVP_GROUPED_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.MipMnToFaMsa.IsSet())
    { 
      c = cm.acquire("MIPMNtoFAMSA");
      data.MipMnToFaMsa.CopyTo(*c);
      aaaMessage.acl.add(c);
    }
 if (data.MipHaToMnMsa.IsSet())
    { 
      c = cm.acquire("MIPHAtoMNMSA");
      data.MipHaToMnMsa.CopyTo(*c);
      aaaMessage.acl.add(c);
    }
  if (data.MipHaToFaMsa.IsSet())
    { 
      c = cm.acquire("MIPHAtoFAMSA");
      data.MipHaToFaMsa.CopyTo(*c);
      aaaMessage.acl.add(c);
    }

 if (data.MipMsaLifetime.IsSet())
    { 
      c = cm.acquire("MIPMsaLifetime");
      data.MipMsaLifetime.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  
if (data.MipOriginatingForeignAaa.IsSet())     
    {
      c = cm.acquire("MIPOriginatingForeignAAA");
      data.MipOriginatingForeignAaa.CopyTo(*c);
      aaaMessage.acl.add(c);
    }
 if (data.MipMobileNodeAddress.IsSet())		
    {
      c = cm.acquire("MIPMobileNodeAddress");
      data.MipMobileNodeAddress.CopyTo(*c, AAA_AVP_IPADDRESS_TYPE);
      aaaMessage.acl.add(c);
    }
 if (data.MipHomeAgentAddress.IsSet())		
    {
      c = cm.acquire("MIPHomeAgentAddress");
      data.MipHomeAgentAddress.CopyTo(*c, AAA_AVP_IPADDRESS_TYPE);
      aaaMessage.acl.add(c);
    } 
 if (data.MipAlgorithmType.IsSet())		
    {
      c = cm.acquire("MIPAlgorithmType");
      data.MipAlgorithmType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    } 
    if (data.MipFilterRule.IsSet())		
    {
      c = cm.acquire("MIPFilterRule");
      data.MipFilterRule.CopyTo(*c, AAA_AVP_IPFILTER_RULE_TYPE);
      aaaMessage.acl.add(c);
    } 

 if (data.OriginStateId.IsSet())
    { 
      c = cm.acquire("OriginStateId");
      data.OriginStateId.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ProxyInfo.IsSet())
    {
      c = cm.acquire("ProxyInfo");
      data.ProxyInfo.CopyTo(*c);
      aaaMessage.acl.add(c);
    }
  if (data.RouteRecord.IsSet())
    {
      c = cm.acquire("RouteRecord");
      data.RouteRecord.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.Avp.IsSet())
    {
      c = cm.acquire("AVP");
      data.Avp.CopyTo(*c, AAA_AVP_CUSTOM_TYPE);
      aaaMessage.acl.add(c);
    }
}

template<> void 
HAR_Parser::parseRawToApp()
{
  HAR_Data &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  data.Clear();


  AAAAvpContainer *c;
  if ((c = aaaMessage.acl.search("SessionId")))
    {
      data.SessionId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthApplicationId")))
    {
      if (Mip4ApplicationId
	  != (*c)[0]->dataRef(Type2Type<diameter_unsigned32_t>()))
	  {
	  AAA_LOG((LM_ERROR, "[%N] Unexpected application id.\n"));
	  throw (DIAMETER_PAYLOAD_ERROR);
	  }
    }
 if ((c = aaaMessage.acl.search("AuthorizationLifetime")))
    {
      data.AuthorizationLifetime.CopyFrom(*c);
    }
 
  if ((c = aaaMessage.acl.search("AuthSessionState")))
    {
      data.AuthSessionState.CopyFrom(*c);
    }
 if ((c = aaaMessage.acl.search("MIPRegRequest")))
    {
      data.MipRegRequest.CopyFrom(*c);
    }
 if ((c = aaaMessage.acl.search("OriginHost")))
    {
      data.OriginHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginRealm")))
    {
      data.OriginRealm.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("UserName")))
    {
      data.UserName.CopyFrom(*c);
    }

  if ((c = aaaMessage.acl.search("DestinationRealm")))
    {
      data.DestinationRealm.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPFeatureVector")))
    {
      data.MipFeatureVector.CopyFrom(*c);
    }

  //optional AVPs
  if ((c = aaaMessage.acl.search("DestinationHost")))
    {
      data.DestinationHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPMNtoHAMSA")))
    {
      data.MipMnToHaMsa.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPMNtoFAMSA")))
    {
      data.MipMnToFaMsa.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPHAtoMNMSA")))
    {
      data.MipHaToMnMsa.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPHAtoFAMSA")))
    {
      data.MipHaToFaMsa.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPMSALifetime")))
    {
      data.MipMsaLifetime.CopyFrom(*c);
    } 
  if ((c = aaaMessage.acl.search("MIPOriginatingForeignAAA")))
    {
      data.MipOriginatingForeignAaa.CopyFrom(*c);
    }
	
  if ((c = aaaMessage.acl.search("MIPMobileNodeAddress")))
    {
      data.MipMobileNodeAddress.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPHomeAgentAddress")))
    {
      data.MipHomeAgentAddress.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPAlgorithmType")))
    {
      data.MipAlgorithmType.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPFilterRule")))
    {
      data.MipFilterRule.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginStateId")))
    {
      data.OriginStateId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ProxyInfo")))
    {
      data.ProxyInfo.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("RouteRecord")))
    {
      data.RouteRecord.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AVP")))
    {
      data.Avp.CopyFrom(*c);
    }
}
