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

#ifndef __AAA_SESSION_INL__
#define __AAA_SESSION_INL__

template <class ATTRIBUTE>
AAAReturnCode AAA_Session<ATTRIBUTE>::TxDelivery
(std::auto_ptr<AAAMessage> msg) 
{
   AAA_IdentityAvpContainerWidget dHostAvp(msg->acl);
   AAA_IdentityAvpContainerWidget dRealmAvp(msg->acl);
   AAA_IdentityAvpContainerWidget orHostAvp(msg->acl);
   AAA_IdentityAvpContainerWidget orRealmAvp(msg->acl);
   AAA_Utf8AvpContainerWidget unameAvp(msg->acl);

   // resolve the destination host
   diameter_identity_t *dHost = dHostAvp.GetAvp
           (AAA_AVPNAME_DESTHOST);
   if (dHost == NULL) {
       if (! m_Attributes.DestinationHost().IsSet()) {
           AAA_ScholarAttribute<diameter_identity_t> dHostAttr;
           SetDestinationHost(dHostAttr);
           if (dHostAttr.IsSet()) {
               dHostAvp.AddAvp(AAA_AVPNAME_DESTHOST) = dHostAttr();
               m_Attributes.DestinationHost().Set(dHostAttr());
           }
       }
       else {
           dHostAvp.AddAvp(AAA_AVPNAME_DESTHOST) = 
               m_Attributes.DestinationHost()();
       }
   }
   else {
       m_Attributes.DestinationHost().Set(*dHost);	
   }

   // resolve the destination realm
   diameter_identity_t *dRealm = dRealmAvp.GetAvp
                   (AAA_AVPNAME_DESTREALM);
   if (dRealm == NULL) {
       if (! m_Attributes.DestinationRealm().IsSet()) {
           AAA_ScholarAttribute<diameter_identity_t> dRealmAttr;
           SetDestinationRealm(dRealmAttr);
           if (dRealmAttr.IsSet()) {
               dRealmAvp.AddAvp(AAA_AVPNAME_DESTREALM) = dRealmAttr();
               m_Attributes.DestinationRealm().Set(dRealmAttr());
           }
           else if (msg->hdr.flags.r) {
               AAA_LOG(LM_INFO, 
               "(%P|%t) No destination realm present in message\n");
               return (AAA_ERR_FAILURE);
           }
       }
       else {
           dRealmAvp.AddAvp(AAA_AVPNAME_DESTREALM) = 
               m_Attributes.DestinationRealm()();
       }
   }
   else {
       m_Attributes.DestinationRealm().Set(*dRealm);	
   }

   // gather username if any
   if (! m_Attributes.Username().IsSet()) {
       diameter_utf8string_t *uname = unameAvp.GetAvp
           (AAA_AVPNAME_USERNAME);
       if (uname == NULL) {
           AAA_ScholarAttribute<diameter_utf8string_t> unameAttr;
           SetUsername(unameAttr);
           if (unameAttr.IsSet()) {
               unameAvp.AddAvp(AAA_AVPNAME_USERNAME) = unameAttr();
               m_Attributes.Username() = unameAttr();
           }
       }
       else {
           m_Attributes.Username().Set(*uname);
       }
   }

   if (orHostAvp.GetAvp(AAA_AVPNAME_ORIGINHOST) == 0) {
       orHostAvp.AddAvp(AAA_AVPNAME_ORIGINHOST) = 
           AAA_CFG_TRANSPORT()->identity;
   }
   if (orRealmAvp.GetAvp(AAA_AVPNAME_ORIGINREALM) == 0) {
       orRealmAvp.AddAvp(AAA_AVPNAME_ORIGINREALM) = 
           AAA_CFG_TRANSPORT()->realm;
   }

   m_Attributes.SessionId().Set(*msg);
   m_Attributes.MsgIdTxMessage(*msg);

   AAA_MsgQuery query(*msg);
   if (query.IsRequest()) {
      return (AAA_MSG_ROUTER()->RequestMsg(msg, 0) ==
              AAA_ROUTE_RESULT_SUCCESS) ?
              AAA_ERR_SUCCESS : AAA_ERR_FAILURE;
   }
   else {
      return (AAA_MSG_ROUTER()->AnswerMsg(msg, 0) ==
              AAA_ROUTE_RESULT_SUCCESS) ?
              AAA_ERR_SUCCESS : AAA_ERR_FAILURE;
   }
}

template <class ATTRIBUTE>
AAAReturnCode AAA_Session<ATTRIBUTE>::Reset() 
{
   m_WaitOnResetEvent = true;
   return (AAA_ERR_SUCCESS);
}

template <class ATTRIBUTE>
void AAA_Session<ATTRIBUTE>::WaitOnReset(bool seed) 
{
   m_WaitOnResetEvent = seed;
   do {
      ACE_Time_Value tv(0, 100000);
      ACE_OS::sleep(tv);
  } while (! m_WaitOnResetEvent);
}

#endif



