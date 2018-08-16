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

// victor fajardo: simple sample server 

#include "diameter_api.h"

class AAA_SampleServer : public DiameterServerAuthSession {
        // AAA serve session derived from DiameterServerAuthSession.
        // It provides for all the functionality of a diameter 
        // server session. Note that the server session factory
        // is responsible for instantiating this object
    public:
        AAA_SampleServer(AAA_Task &task,
                         diameter_unsigned32_t id,
                         bool endOnSuccess = false) :
           DiameterServerAuthSession(task, id),
           m_EndOnSuccess(endOnSuccess) {
        }
        virtual void SetAuthSessionState
        (DiameterScholarAttribute<diameter_unsigned32_t> &authState)
        {
            // optional override, called by the library to set 
            // the auth state. Note that this overrides the 
            // settings in the configuration file or applications 
            // sending an auth session state AVP
            authState = DIAMETER_SESSION_NO_STATE_MAINTAINED;
        }
        virtual void SetSessionTimeout
        (DiameterScholarAttribute<diameter_unsigned32_t> &timeout)
        {
            // optional override, called by the library so 
            // this server can dictate the session timeout 
            // to the client. If not overridden, the value 
            // in the config file is used
            timeout = 0;
        }
        virtual void SetAuthLifetimeTimeout
        (DiameterScholarAttribute<diameter_unsigned32_t> &timeout)
        {
            // optional override, called by the library so 
            // this server can dictate the session timeout 
            // to the client. If not overridden, the value 
            // in the config file is used
            timeout = 2;
        }
        virtual void SetAuthGracePeriodTimeout
        (DiameterScholarAttribute<diameter_unsigned32_t> &timeout)
        {
            // optional override, called by the library so 
            // this server can dictate the auth grace period
            // to the client. If not overridden, the value 
            // in the config file is used
            timeout = 2;
        }
        virtual AAAReturnCode ReAuthenticate(diameter_unsigned32_t rcode) {
            // optional override, called by the library so 
            // this server is informed that the client has
            // responded to the server initiated re-auth
            // request. The result code from the client
            // is passed as a parameter to this funciton.
            AAA_LOG((LM_INFO, "(%P|%t) **** client responded to re-auth ****\n"));
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode RequestMsg(DiameterMsg &msg) {

            // all request messages are handled by this function. 
            // This function can retrun the following values:
            // a. AAA_ERR_SUCCESS - client is successfully authenticated
            // b. AAA_ERR_INCOMPLETE - auth not yet completed, muti-round 
            //                         message trip exchange
            // c. AAA_ERR_FAILURE - client authentication failed

            AAA_LOG((LM_INFO, "(%P|%t) Request message received\n"));
            DiameterMsgHeaderDump::Dump(msg);

            DiameterIdentityAvpContainerWidget oHostAvp(msg.acl);
            DiameterIdentityAvpContainerWidget oRealmAvp(msg.acl);
            DiameterUtf8AvpContainerWidget uNameAvp(msg.acl);
            DiameterUInt32AvpContainerWidget authAppIdAvp(msg.acl);
            DiameterEnumAvpContainerWidget reAuthAvp(msg.acl);

            diameter_identity_t *host = oHostAvp.GetAvp(DIAMETER_AVPNAME_ORIGINHOST);
            diameter_identity_t *realm = oRealmAvp.GetAvp(DIAMETER_AVPNAME_ORIGINREALM);
            diameter_utf8string_t *uname = uNameAvp.GetAvp(DIAMETER_AVPNAME_USERNAME);
            diameter_unsigned32_t *authAppId = authAppIdAvp.GetAvp(DIAMETER_AVPNAME_AUTHAPPID);
            diameter_enumerated_t *reAuth = reAuthAvp.GetAvp(DIAMETER_AVPNAME_REAUTHREQTYPE);

            if (host) {
                AAA_LOG((LM_INFO, "(%P|%t) From Host: %s\n", host->c_str()));
            }
            if (realm) {
                AAA_LOG((LM_INFO, "(%P|%t) From Realm: %s\n", realm->c_str()));
            }
            if (uname) {
                AAA_LOG((LM_INFO, "(%P|%t) From User: %s\n", uname->c_str()));
            }
            if (authAppId) {
                AAA_LOG((LM_INFO, "(%P|%t) Auth Application Id: %d\n", *authAppId));
            }
            if (reAuth) {
                AAA_LOG((LM_INFO, "(%P|%t) Re-Auth Request type: %d\n", *reAuth));
	    }

            // Send answer back
            return TxAuthenticationAnswer();
        }
        virtual AAAReturnCode AnswerMsg(DiameterMsg &msg) {
            // all answer messages are handled by this function.
            // AAA servers normally will receive answer messages
            // in the open state

            // all answer messages are handled by this function. 
            // This function can retrun the following values:
            // a. AAA_ERR_SUCCESS - client has successfully responded
            //                      to server request
            // b. AAA_ERR_FAILURE - client failed. 
            AAA_LOG((LM_INFO, "(%P|%t) **** Answer message message received in server ****\n"));
            DiameterMsgHeaderDump::Dump(msg);
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode ErrorMsg(DiameterMsg &msg) {
            // all error messages are handled by this function.
            AAA_LOG((LM_INFO, "(%P|%t) **** Received message with error bit set ****\n"));
            return (AAA_ERR_SUCCESS);
	}
        virtual AAAReturnCode Success() {
            // notification of successful auth
            AAA_LOG((LM_INFO, "(%P|%t) **** user authorized ****\n"));
            if (m_EndOnSuccess) {
                End();
	    }
            else {
                // test server re-auth
                // ReAuth(AAA_SESSION_AUTHORIZE_AUTHENTICATE);
	    }
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode Disconnect() {
            // notification of completed STR/STA exchange
            AAA_LOG((LM_INFO, "(%P|%t) **** session disconnecting ****\n"));
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode ReClaimSession() {
            // notification of a stateless session being reclaimed
            // return AAA_ERR_SUCCESS if you wish to delete this server
            // session otherwise return AAA_ERR_FAILURE
            AAA_LOG((LM_INFO, "(%P|%t) **** session re-claim ****\n"));
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode SessionTimeout() {
            // notification of session timeout
            AAA_LOG((LM_INFO, "(%P|%t) **** session timeout ****\n"));
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode AuthorizationTimeout() {
            // notification of auth lifetime timeout
            AAA_LOG((LM_INFO, "(%P|%t) **** auth timeout ****\n"));
            return (AAA_ERR_SUCCESS);
        }
        virtual AAAReturnCode AbortSession() {
            // notification of completed ASR/ASA exchange
            AAA_LOG((LM_INFO, "(%P|%t) **** session aborted by server ****\n"));
            return (AAA_ERR_SUCCESS);
        }
        AAAReturnCode TxAuthenticationAnswer() {
            std::cout << "Sending answer message" << std::endl;

            DiameterMsgWidget msg(271, false, 167772153);

            DiameterUInt32AvpWidget authIdAvp(DIAMETER_AVPNAME_AUTHAPPID);
            DiameterUtf8AvpWidget unameAvp(DIAMETER_AVPNAME_USERNAME);
            DiameterEnumAvpWidget acctRecType(DIAMETER_AVPNAME_ACCTREC_TYPE);
            DiameterUInt32AvpWidget acctRecNum(DIAMETER_AVPNAME_ACCTREC_NUM);

            authIdAvp.Get() = 167772153; // my application id
            unameAvp.Get() = "username@alcatel-lucent.com";
            acctRecType.Get() = 2;
            acctRecNum.Get() = 2;

            msg()->acl.add(authIdAvp());
            msg()->acl.add(unameAvp());
            msg()->acl.add(acctRecType());
            msg()->acl.add(acctRecNum());

            DiameterMsgResultCode rcode(*msg());
            rcode.ResultCode(AAA_SUCCESS);

            Send(msg());
            return (AAA_ERR_SUCCESS);
        }
    private:
        bool m_EndOnSuccess;
};

// Server session factory. Unlike AAA clients, server
// sessions need to be created on demand. This factory
// is responsible for creating new server sessions
// based on incomming new request.
typedef DiameterServerSessionAllocator<AAA_SampleServer> 
        SampleServerAllocator;

int main(int argc, char *argv[])
{
   AAA_Task task;
   task.Start(5);

   // Application core is responsible for providing
   // peer connectivity between AAA entities
   DiameterApplication appCore(task, "config/isp.local.xml");
   SampleServerAllocator allocator(task, 167772153);
   appCore.RegisterServerSessionFactory(allocator);

   while (true) {
      std::cout << "Just wait here and let factory take care of new sessions" << std::endl;
      ACE_OS::sleep(10);
   }

   appCore.Close();
   task.Stop();
   return (0);
}






