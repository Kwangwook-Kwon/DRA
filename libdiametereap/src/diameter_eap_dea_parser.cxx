/* BEGIN_COPYRIGHT                                                        */
/*                                                                        */
/* Open Diameter: Open-source software for the Diameter and               */
/*                Diameter related protocols                              */
/*                                                                        */
/* Copyright (C) 2002-2007 Open Diameter Project                          */
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

/* $Id: diameter_eap_dea_parser.cxx,v 1.15 2006/03/16 17:01:32 vfajardo Exp $ */
/* 
   diameter_eap_dea_parser.cxx
   Diameter EAP Answer Parser
   Written by Yoshihiro Ohba
   Created December 8, 2003.
*/

#include "diameter_eap_parser.hxx"

template<> void 
DEA_Parser::parseAppToRaw()
{
  DEA_Data &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  
  DiameterDictionaryManager dm;
  DiameterAvpContainerManager cm;
  AAAAvpContainer *c;
                          
  AAACommandCode code;
  DiameterApplicationId appId;

  // Obtain Command Code and Application Identifier.
  if (!dm.getCommandCode("DiameterEAPAnswer", &code, &appId))
    {
      AAA_LOG((LM_ERROR, "Cannot find Diameter message in dictionary\n."));
      throw (DIAMETER_DICTIONARY_ERROR);
      return;
    }

  // Specify the header.
  diameter_hdr_flag flag = {0,0,0};  // Answer
  DiameterMsgHeader hdr(1, 0, flag, code, appId, 0, 0);
  aaaMessage.hdr = hdr;

  if (data.AuthApplicationId.IsSet())
    {
      c = cm.acquire("AuthApplicationId");
      data.AuthApplicationId.CopyTo(*c, AAA_AVP_INTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.AuthRequestType.IsSet())
    {
      c = cm.acquire("AuthRequestType");
      data.AuthRequestType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.OriginStateId.IsSet())
    { 
      c = cm.acquire("OriginStateId");
      data.OriginStateId.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.PortLimit.IsSet())
    { 
      c = cm.acquire("PortLimit");
      data.PortLimit.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.UserName.IsSet())
    { 
      c = cm.acquire("UserName");
      data.UserName.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.EapPayload.IsSet())
    { 
      c = cm.acquire("EAPPayload");
      data.EapPayload.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.EapReissuedPayload.IsSet())
    { 
      c = cm.acquire("EAPReissuedPayload");
      data.EapReissuedPayload.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.EapMasterSessionKey.IsSet())
    { 
      c = cm.acquire("EAPMasterSessionKey");
      data.EapMasterSessionKey.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.AccountingEapAuthMethod.IsSet())
    { 
      c = cm.acquire("AccountingEapAuthMethod");
      data.AccountingEapAuthMethod.CopyTo(*c, AAA_AVP_UINTEGER64_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ServiceType.IsSet())
    { 
      c = cm.acquire("ServiceType");
      data.ServiceType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ResultCode.IsSet())
    { 
      c = cm.acquire("ResultCode");
      data.ResultCode.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.MultiRoundTimeOut.IsSet())
    { 
      c = cm.acquire("MultiRoundTimeOut");
      data.MultiRoundTimeOut.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ConfigurationToken.IsSet())
    { 
      c = cm.acquire("ConfigurationToken");
      data.ConfigurationToken.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.AcctInterimInterval.IsSet())
    { 
      c = cm.acquire("AcctInterimInterval");
      data.AcctInterimInterval.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
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
  if (data.ReAuthRequestType.IsSet())
    { 
      c = cm.acquire("ReAuthRequestType");
      data.ReAuthRequestType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ReplyMessage.IsSet())
    { 
      c = cm.acquire("ReplyMessage");
      data.ReplyMessage.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FilterId.IsSet())
    { 
      c = cm.acquire("FilterId");
      data.FilterId.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.CallbackId.IsSet())
    { 
      c = cm.acquire("CallbackId");
      data.CallbackId.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedAppletalkLink.IsSet())
    { 
      c = cm.acquire("FramedAppletalkLink");
      data.FramedAppletalkLink.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedAppletalkNetwork.IsSet())
    { 
      c = cm.acquire("FramedAppletalkNetwork");
      data.FramedAppletalkNetwork.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedAppletalkZone.IsSet())
    { 
      c = cm.acquire("FramedAppletalkZone");
      data.FramedAppletalkZone.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedIpv6Route.IsSet())
    { 
      c = cm.acquire("FramedIPv6Route");
      data.FramedIpv6Route.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedIpv6Pool.IsSet())
    { 
      c = cm.acquire("FramedIPv6Pool");
      data.FramedIpv6Pool.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedPool.IsSet())
    { 
      c = cm.acquire("FramedPool");
      data.FramedPool.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedIpxNetwork.IsSet())
    { 
      c = cm.acquire("FramedIPXNetwork");
      data.FramedIpxNetwork.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedRoute.IsSet())
    { 
      c = cm.acquire("FramedRoute");
      data.FramedRoute.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedRouting.IsSet())
    { 
      c = cm.acquire("FramedRouting");
      data.FramedRouting.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.RedirectHost.IsSet())
    {
      c = cm.acquire("RedirectHost");
      data.RedirectHost.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.RedirectHostUsage.IsSet())
    { 
      c = cm.acquire("RedirectHostUsage");
      data.RedirectHostUsage.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }

  if (data.RedirectMaxCacheTime.IsSet())
    { 
      c = cm.acquire("RedirectMaxCacheTime");
      data.RedirectMaxCacheTime.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }

  if (data.IdleTimeout.IsSet())
    { 
      c = cm.acquire("IdleTimeout");
      data.IdleTimeout.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.State.IsSet())
    { 
      c = cm.acquire("State");
      data.State.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.AuthorizationLifetime.IsSet())
    { 
      c = cm.acquire("AuthorizationLifetime");
      data.AuthorizationLifetime.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.AuthGracePeriod.IsSet())
    { 
      c = cm.acquire("AuthGracePeriod");
      data.AuthGracePeriod.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.AuthSessionState.IsSet())
    { 
      c = cm.acquire("AuthSessionState");
      data.AuthSessionState.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.SessionTimeout.IsSet())
    { 
      c = cm.acquire("SessionTimeout");
      data.SessionTimeout.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.CallbackNumber.IsSet())
    { 
      c = cm.acquire("CallbackNumber");
      data.CallbackNumber.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.Class.IsSet())
    {
      c = cm.acquire("Class");
      data.Class.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedCompression.IsSet())
    { 
      c = cm.acquire("FramedCompression");
      data.FramedCompression.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedInterfaceId.IsSet())
    { 
      c = cm.acquire("FramedInterfaceId");
      data.FramedInterfaceId.CopyTo(*c, AAA_AVP_UINTEGER64_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedIpAddress.IsSet())
    { 
      c = cm.acquire("FramedIPAddress");
      data.FramedIpAddress.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedIpv6Prefix.IsSet())
    {
      c = cm.acquire("FramedIPv6Prefix");
      data.FramedIpv6Prefix.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedIpNetmask.IsSet())
    { 
      c = cm.acquire("FramedIPNetmask");
      data.FramedIpNetmask.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedMtu.IsSet())
    { 
      c = cm.acquire("FramedMTU");
      data.FramedMtu.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.FramedProtocol.IsSet())
    { 
      c = cm.acquire("FramedProtocol");
      data.FramedProtocol.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasFilterRule.IsSet())
    {
      c = cm.acquire("NASFilterRule");
      data.NasFilterRule.CopyTo(*c, AAA_AVP_IPFILTER_RULE_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.Tunneling.IsSet())
    {
      c = cm.acquire("Tunneling");
      data.Tunneling.CopyTo(*c);
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
DEA_Parser::parseRawToApp()
{
  DEA_Data &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  data.Clear();

  //  AAAAvpContainerEntry *e;
  AAAAvpContainer *c;
  if ((c = aaaMessage.acl.search("SessionId")))
    {
      data.SessionId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthApplicationId")))
    {
      data.AuthApplicationId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginHost")))
    {
      data.OriginHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginRealm")))
    {
      data.OriginRealm.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginStateId")))
    {
      data.OriginStateId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("PortLimit")))
    {
      data.PortLimit.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("UserName")))
    {
      data.UserName.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("EAPPayload")))
    {
      data.EapPayload.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("EAPReissuedPayload")))
    {
      data.EapReissuedPayload.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("EAPMasterSessionKey")))
    {
      data.EapMasterSessionKey.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AccountingEapAuthMethod")))
    {
      data.AccountingEapAuthMethod.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ServiceType")))
    {
      data.ServiceType.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ResultCode")))
    {
      data.ResultCode.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("MultiRoundTimeOut")))
    {
      data.MultiRoundTimeOut.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ConfigurationToken")))
    {
      data.ConfigurationToken.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AcctInterimInterval")))
    {
      data.AcctInterimInterval.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ErrorMessage")))
    {
      data.ErrorMessage.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ErrorReportingHost")))
    {
      data.ErrorReportingHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ReAuthRequestType")))
    {
      data.ReAuthRequestType.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FilterId")))
    {
      data.FilterId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("CallbackId")))
    {
      data.CallbackId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedAppletalkLink")))
    {
      data.FramedAppletalkLink.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedAppletalkNetwork")))
    {
      data.FramedAppletalkNetwork.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedAppletalkZone")))
    {
      data.FramedAppletalkZone.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedIPv6Route")))
    {
      data.FramedIpv6Route.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedIPv6Pool")))
    {
      data.FramedIpv6Pool.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedPool")))
    {
      data.FramedPool.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedIPXNetwork")))
    {
      data.FramedIpxNetwork.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedRoute")))
    {
      data.FramedRoute.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedRouting")))
    {
      data.FramedRouting.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("RedirectHost")))
    {
      data.RedirectHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("RedirectHostUsage")))
    {
      data.RedirectHostUsage.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("RedirectMaxCacheTime")))
    {
      data.RedirectMaxCacheTime.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("IdleTimeout")))
    {
      data.IdleTimeout.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("State")))
    {
      data.State.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthorizationLifetime")))
    {
      data.AuthorizationLifetime.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthGracePeriod")))
    {
      data.AuthGracePeriod.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthSessionState")))
    {
      data.AuthSessionState.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("SessionTimeout")))
    {
      data.SessionTimeout.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("CallbackNumber")))
    {
      data.CallbackNumber.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("Class")))
    {
      data.Class.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedCompression")))
    {
      data.FramedCompression.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedInterfaceId")))
    {
      data.FramedInterfaceId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedIPAddress")))
    {
      data.FramedIpAddress.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedIPv6Prefix")))
    {
      data.FramedIpv6Prefix.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedMTU")))
    {
      data.FramedMtu.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedProtocol")))
    {
      data.FramedProtocol.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NASFilterRule")))
    {
      data.NasFilterRule.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("Tunneling")))
    {
      data.Tunneling.CopyFrom(*c);
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

