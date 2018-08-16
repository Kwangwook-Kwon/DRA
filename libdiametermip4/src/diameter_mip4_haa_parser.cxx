/* BEGIN_COPYRIGHT                                                        */
/*                                                                        */
/* OpenDiameter: Open-source software for the Diameter protocol           */
/*                                                                        */
/* Copyright (C) 2002-2007 Open Diameter Project	                  */
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
   diameter_mip4_ama_parser.cxx
   Diameter MIP AMA Answer Parser
   Written by Miriam Tauil
   Created May 27, 2004.
*/

#include "diameter_mip4_parser.hxx"

template<> void 
HAA_Parser::parseAppToRaw()
{
  HAA_Data &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  
  DiameterDictionaryManager dm;
  DiameterAvpContainerManager cm;
  AAAAvpContainer *c;
                          
  AAACommandCode code;
  DiameterApplicationId appId;

  // Obtain Command Code and Application Identifier.
  if (!dm.getCommandCode("HomeAgentMIPAnswer", &code, &appId))
    {
      AAA_LOG((LM_ERROR, "Cannot find Diameter message in dictionary\n."));
      throw (DIAMETER_DICTIONARY_ERROR);
      return;
    }

  // Specify the header.
  diameter_hdr_flag flag = {0,0,0};  // Answer
  DiameterMsgHeader haHdr(1, 0, flag, code, appId, 0, 0);
  aaaMessage.hdr = haHdr;

  if (data.AuthApplicationId.IsSet())
    {
      c = cm.acquire("AuthApplicationId");
      data.AuthApplicationId.CopyTo(*c, AAA_AVP_INTEGER32_TYPE);
      aaaMessage.acl.add(c);
    } 
  if (data.ResultCode.IsSet())
    { 
      c = cm.acquire("ResultCode");
      data.ResultCode.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
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
 if (data.AcctMultiSessionId.IsSet())
    { 
      c = cm.acquire("AcctMultiSessionId");
      data.AcctMultiSessionId.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.UserName.IsSet())
    { 
      c = cm.acquire("UserName");
      data.UserName.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  
  if (data.ErrorMessage.IsSet())
    { 
      c = cm.acquire("ErrorMessage");
      data.ErrorMessage.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ErrorReportingHost.IsSet())
    {
      c = cm.acquire("ErrorReportingHost");
      data.ErrorReportingHost.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.MipRegReply.IsSet())
    { 
      c = cm.acquire("MIPRegReply");
      data.MipRegReply.CopyTo(*c, AAA_AVP_STRING_TYPE); 
      aaaMessage.acl.add(c);
    }


 if (data.MipHomeAgentAddress.IsSet())				
    {
      c = cm.acquire("MIPHomeAgentAddress");
      data.MipHomeAgentAddress.CopyTo(*c, AAA_AVP_IPADDRESS_TYPE);
      aaaMessage.acl.add(c);
    } 
 if (data.MipMobileNodeAddress.IsSet())				
    {
      c = cm.acquire("MIPMobileNodeAddress");
      data.MipMobileNodeAddress.CopyTo(*c, AAA_AVP_IPADDRESS_TYPE);
      aaaMessage.acl.add(c);
    }
 if (data.MipFaToHaSpi.IsSet())				
    {
      c = cm.acquire("MIPFAtoHASPI");
      data.MipFaToHaSpi.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
 if (data.MipFaToMnSpi.IsSet())				
    {
      c = cm.acquire("MIPFAtoMNSPI");
      data.MipFaToMnSpi.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
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
  if (data.Avp.IsSet())
    {
      c = cm.acquire("AVP");
      data.Avp.CopyTo(*c, AAA_AVP_CUSTOM_TYPE);
      aaaMessage.acl.add(c);
    }
}

template<> void 
HAA_Parser::parseRawToApp()
{
  HAA_Data &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  data.Clear();

  AAAAvpContainer *c;
  if ((c = aaaMessage.acl.search("SessionId")))
    {
      data.SessionId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthApplicationId")))
    {
      data.AuthApplicationId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ResultCode")))
    {
      data.ResultCode.CopyFrom(*c);
    }

  if ((c = aaaMessage.acl.search("OriginHost")))
    {
      data.OriginHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginRealm")))
    {
      data.OriginRealm.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AcctMultiSessionId")))
    {
      data.AcctMultiSessionId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("UserName")))
    {
      data.UserName.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ErrorMessage")))
    {
      data.ErrorMessage.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ErrorReportingHost")))
    {
      data.ErrorReportingHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPRegReply")))
    {
      data.MipRegReply.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPHomeAgentAddress")))
    {
      data.MipHomeAgentAddress.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPMobileNodeAddress")))
    {
      data.MipMobileNodeAddress.CopyFrom(*c);
    }
 if ((c = aaaMessage.acl.search("MIPFAtoHASPI")))
    {
      data.MipFaToHaSpi.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MIPFAtoMNSPI")))
    {
      data.MipFaToMnSpi.CopyFrom(*c);
    }
 if ((c = aaaMessage.acl.search("OriginStateId")))
    {
      data.OriginStateId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ProxyInfo")))
    {
      data.ProxyInfo.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AVP")))
    {
      data.Avp.CopyFrom(*c);
    }
}
