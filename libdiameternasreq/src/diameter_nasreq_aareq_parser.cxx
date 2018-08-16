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

/* $Id: diameter_nasreq_aareq_parser.cxx,v 1.7 2006/03/16 17:01:47 vfajardo Exp $ */
/* 
   diameter_nasrq_aareq_parser.cxx
   Diameter AA-Request Parser
   Written by Yoshihiro Ohba
   Created May 1, 2004
*/

#include "diameter_nasreq_parser.hxx"

template<> void 
AA_RequestParser::parseAppToRaw()
{
  AA_RequestData &data = *getAppData();
  DiameterMsg &aaaMessage = *getRawData();

  DiameterDictionaryManager dm;
  DiameterAvpContainerManager cm;
  AAAAvpContainer *c;
                          
  AAACommandCode code;
  DiameterApplicationId appId;

  // Obtain Command Code and Application Identifier.
  if (!dm.getCommandCode("AA-Request", &code, &appId))
    {
      AAA_LOG((LM_ERROR, "[%N] Cannot find message in dictionary\n."));
      throw (DIAMETER_DICTIONARY_ERROR);
    }

  // Specify the header.
  diameter_hdr_flag flag = {1,1,0};
  DiameterMsgHeader hdr(1, 0, flag, code, appId, 0, 0);
  aaaMessage.hdr = hdr;

  if (data.AuthApplicationId.IsSet())
    {
      c = cm.acquire("AuthApplicationId");
      data.AuthApplicationId.CopyTo(*c, AAA_AVP_INTEGER32_TYPE);
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

  if (data.DestinationRealm.IsSet())
    {
      c = cm.acquire("DestinationRealm");
      data.DestinationRealm.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }

  if (data.AuthRequestType.IsSet())
    {
      c = cm.acquire("AuthRequestType");
      data.AuthRequestType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.DestinationHost.IsSet())
    { 
      c = cm.acquire("DestinationHost");
      data.DestinationHost.CopyTo(*c, AAA_AVP_DIAMID_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasIdentifier.IsSet())
    { 
      c = cm.acquire("NasIdentifier");
      data.NasIdentifier.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasIpAddress.IsSet())
    { 
      c = cm.acquire("NasIPAddress");
      data.NasIpAddress.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasIpv6Address.IsSet())
    { 
      c = cm.acquire("NasIPv6Address");
      data.NasIpv6Address.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasPort.IsSet())
    { 
      c = cm.acquire("NasPort");
      data.NasPort.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasPortId.IsSet())
    { 
      c = cm.acquire("NasPortId");
      data.NasPortId.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.NasPortType.IsSet())
    { 
      c = cm.acquire("NasPortType");
      data.NasPortType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
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
  if (data.UserPassword.IsSet())
    { 
      c = cm.acquire("UserPassword");
      data.UserPassword.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ServiceType.IsSet())
    { 
      c = cm.acquire("ServiceType");
      data.ServiceType.CopyTo(*c, AAA_AVP_ENUM_TYPE);
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
  if (data.CallbackNumber.IsSet())
    { 
      c = cm.acquire("CallbackNumber");
      data.CallbackNumber.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.CalledStationId.IsSet())
    { 
      c = cm.acquire("CalledStationId");
      data.CalledStationId.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.CallingStationId.IsSet())
    { 
      c = cm.acquire("CallingStationId");
      data.CallingStationId.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.OriginatingLineInfo.IsSet())
    { 
      c = cm.acquire("OriginatingLineInfo");
      data.OriginatingLineInfo.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ConnectInfo.IsSet())
    { 
      c = cm.acquire("ConnectInfo");
      data.ConnectInfo.CopyTo(*c, AAA_AVP_UTF8_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ChapAuth.IsSet())
    {
      c = cm.acquire("CHAPAuth");
      data.ChapAuth.CopyTo(*c);
      aaaMessage.acl.add(c);
    }
  if (data.ChapChallenge.IsSet())
    { 
      c = cm.acquire("CHAPChallenge");
      data.ChapChallenge.CopyTo(*c, AAA_AVP_STRING_TYPE);
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
      data.FramedInterfaceId.CopyTo(*c, AAA_AVP_UINTEGER32_TYPE);
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
  if (data.ArapPassword.IsSet())
    { 
      c = cm.acquire("ARAPPassword");
      data.ArapPassword.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ArapChallengeResponse.IsSet())
    { 
      c = cm.acquire("ARAPChallengeResponse");
      data.ArapChallengeResponse.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ArapSecurity.IsSet())
    { 
      c = cm.acquire("ARAPSecurity");
      data.ArapSecurity.CopyTo(*c, AAA_AVP_INTEGER32_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.ArapSecurityData.IsSet())
    { 
      c = cm.acquire("ARAPSecurityData");
      data.ArapSecurityData.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.LoginIpHost.IsSet())
    { 
      c = cm.acquire("LoginIPHost");
      data.LoginIpHost.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.LoginIpv6Host.IsSet())
    { 
      c = cm.acquire("LoginIPv6Host");
      data.LoginIpv6Host.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.LoginLatGroup.IsSet())
    { 
      c = cm.acquire("LoginLATGroup");
      data.LoginLatGroup.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.LoginLatNode.IsSet())
    { 
      c = cm.acquire("LoginLATNode");
      data.LoginLatNode.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.LoginLatPort.IsSet())
    { 
      c = cm.acquire("LoginLATPort");
      data.LoginLatPort.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.LoginLatService.IsSet())
    { 
      c = cm.acquire("LoginLATService");
      data.LoginLatService.CopyTo(*c, AAA_AVP_STRING_TYPE);
      aaaMessage.acl.add(c);
    }
  if (data.Tunneling.IsSet())
    {
      c = cm.acquire("Tunneling");
      data.Tunneling.CopyTo(*c);
      aaaMessage.acl.add(c);
    }
  if (data.ProxyInfo.size()>0)
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
AA_RequestParser::parseRawToApp()
{
  AA_RequestData &data = *getAppData();
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
      if (NasreqApplicationId
	  != (*c)[0]->dataRef(Type2Type<diameter_unsigned32_t>()))
	{
	  AAA_LOG((LM_ERROR, "[%N] Unexpected application id.\n"));
	  throw (DIAMETER_PAYLOAD_ERROR);
	}
    }
  if ((c = aaaMessage.acl.search("OriginHost")))
    {
      data.OriginHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginRealm")))
    {
      data.OriginRealm.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("DestinationRealm")))
    {
      data.DestinationRealm.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("AuthRequestType")))
    {
      data.AuthRequestType.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("DestinationHost")))
    {
      data.DestinationHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NasIdentifier")))
    {
      data.NasIdentifier.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NasIPAddress")))
    {
      data.NasIpAddress.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NasIPv6Address")))
    {
      data.NasIpv6Address.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NasPort")))
    {
      data.NasPort.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NasPortId")))
    {
      data.NasPortId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("NasPortType")))
    {
      data.NasPortType.CopyFrom(*c);
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
  if ((c = aaaMessage.acl.search("UserPassword")))
    {
      data.UserPassword.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ServiceType")))
    {
      data.ServiceType.CopyFrom(*c);
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
  if ((c = aaaMessage.acl.search("CallbackNumber")))
    {
      data.CallbackNumber.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("CalledStationId")))
    {
      data.CalledStationId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("CallingStationId")))
    {
      data.CallingStationId.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("OriginatingLineInfo")))
    {
      data.OriginatingLineInfo.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ConnectInfo")))
    {
      data.ConnectInfo.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("CHAPAuth")))
    {
      data.ChapAuth.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("CHAPChallenge")))
    {
      data.ChapChallenge.CopyFrom(*c);
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
  if ((c = aaaMessage.acl.search("FramedIPNetmask")))
    {
      data.FramedIpAddress.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("FramedMTU")))
    {
      data.FramedMtu = (*c)[0]->dataRef(Type2Type<diameter_unsigned32_t>());
    }
  if ((c = aaaMessage.acl.search("FramedProtocol")))
    {
      data.FramedProtocol.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ARAPPassword")))
    {
      data.ArapPassword.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ARAPChallengeResponse")))
    {
      data.ArapChallengeResponse.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ARAPSecurity")))
    {
      data.ArapSecurity.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("ARAPSecurityData")))
    {
      data.ArapSecurityData.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("LoginIPHost")))
    {
      data.LoginIpHost.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("LoginIPv6Host")))
    {
      data.LoginIpv6Host.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("LoginLATGroup")))
    {
      data.LoginLatGroup.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("LoginLATNode")))
    {
      data.LoginLatNode.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("LoginLATPort")))
    {
      data.LoginLatPort.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("LoginLATService")))
    {
      data.LoginLatService.CopyFrom(*c);
    }
  if ((c = aaaMessage.acl.search("Tunneling")))
    {
      data.Tunneling.CopyFrom(*c);
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

