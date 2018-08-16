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


#include "aaa_xml_data.h"
#include "aaa_log_facility.h"
#include "aaa_peer_interface.h"

int AAA_XMLDataGeneral::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "product";
   OD_Utl_XMLDataString product(tagName, payload.product);
   product.populate(n->getFirstChild());

   tagName = "version";
   OD_Utl_XMLDataUInt32 version(tagName, payload.version);
   version.populate(n->getFirstChild());

   tagName = "vendor_id";
   OD_Utl_XMLDataUInt32 vendor_id(tagName, payload.vendor);
   vendor_id.populate(n->getFirstChild());

   AAA_ApplicationIdLst *idList[] = { &payload.supportedVendorIdLst,
                                      &payload.authAppIdLst,
                                      &payload.acctAppIdLst };
   char *idNames[] = { "supported_vendor_id",
                       "auth_application_id",
                       "acct_application_id" };
   for (int i=0; i<sizeof(idNames)/sizeof(char*); i++) {
   
       tagName = idNames[i];
       DOMNode *found, *sibling = n->getFirstChild();

       while (sibling) {
          diameter_unsigned32_t value;
          OD_Utl_XMLDataUInt32 id(tagName, value);
          if (id.populate(sibling, &found) == 0) {
             idList[i]->push_back(value); 
             sibling = found->getNextSibling();
          }
          else {
             break;
          }
       }
   }

   print(payload);

   tagName = "vendor_specific_application_id";
   DOMNode *found, *sibling = n->getFirstChild();

   while (sibling) {
      AAA_DataVendorSpecificApplicationId newId;
      AAA_XMLDataVendorSpecificApplicationId id(tagName, newId);
      if (id.populate(sibling, &found) == 0) {
         payload.vendorSpecificId.push_back(newId);
         sibling = found->getNextSibling();
      }
      else {
         break;
      }
   }
   
   return (0);
}

void AAA_XMLDataGeneral::print(AAA_DataGeneral &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)             Product : %s\n", 
                 data.product.data());
   AAA_LOG(LM_INFO, "(%P|%t)             Version : %d\n", 
                 data.version);
   AAA_LOG(LM_INFO, "(%P|%t)           Vendor Id : %d\n", 
                 data.vendor);

   AAA_ApplicationIdLst *idList[] = { &data.supportedVendorIdLst,
                                      &data.authAppIdLst,
                                      &data.acctAppIdLst };
   char *label[] = { "Supported Vendor",
                     "Auth Application",
                     "Acct Application" };
   for (int i=0;
        i < sizeof(idList)/sizeof(AAA_ApplicationIdLst*);
        i++) {
       AAA_ApplicationIdLst::iterator x = idList[i]->begin();
       for (; x != idList[i]->end(); x++) {
           AAA_LOG(LM_INFO, "(%P|%t)    %s : %d\n",
                   label[i], *x);
       }
   }
}

int AAA_XMLDataParser::svc(DOMNode *n)
{
   std::string tagName = "dictionary";
   OD_Utl_XMLDataString dictionary(tagName, payload.dictionary);
   dictionary.populate(n->getFirstChild());
  
   AAA_XMLDataParser::print(payload);
  
   return (0);
}

void AAA_XMLDataParser::print(AAA_DataParser &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)          Dictionary : %s\n", 
                 data.dictionary.data());  
}

int AAA_XMLDataVendorSpecificApplicationId::svc(DOMNode *n)
{
   std::string tagName = "auth_application_id";
   OD_Utl_XMLDataUInt32 auth(tagName, payload.authAppId);
   auth.populate(n->getFirstChild());
   
   tagName = "acct_application_id";
   OD_Utl_XMLDataUInt32 acct(tagName, payload.acctAppId);
   acct.populate(n->getFirstChild());
   
   tagName = "vendor_id";
   DOMNode *found, *sibling = n->getFirstChild();

   while (sibling) {
      diameter_unsigned32_t vendorId;
      OD_Utl_XMLDataUInt32 svid(tagName, vendorId);
      if (svid.populate(sibling, &found) == 0) {
         payload.vendorIdLst.push_back(vendorId); 
         sibling = found->getNextSibling();
      }
      else {
         break;
      }
   }
   
   print(payload);   
   return (0);
}

void AAA_XMLDataVendorSpecificApplicationId::print
    (AAA_DataVendorSpecificApplicationId &e)
{
   AAA_LOG(LM_INFO, "(%P|%t)  Vendor Specific Id : Auth=%d, Acct=%d\n",
           e.authAppId, e.acctAppId);
   AAA_ApplicationIdLst::iterator i = e.vendorIdLst.begin();
   for (; i != e.vendorIdLst.end(); i++) {
       AAA_LOG(LM_INFO, "(%P|%t)                        Vendor=%d\n",
               (*i));
   }
}

int AAA_XMLDataPeer::svc(DOMNode *n)
{
   std::string tagName = "hostname";
   OD_Utl_XMLDataString product(tagName, payload.hostname);
   product.populate(n->getFirstChild());

   tagName = "port";
   OD_Utl_XMLDataUInt32 port(tagName, payload.port);
   port.populate(n->getFirstChild());
  
   tagName = "tls_enabled";
   OD_Utl_XMLDataUInt32 tls(tagName, payload.tls_enabled);
   tls.populate(n->getFirstChild());

   AAA_PeerManager mngr(task);
   if (mngr.Add(payload.hostname,
                payload.port,
                payload.tls_enabled,
                0,
                true)) {   
       AAA_Peer *newPeer = mngr.Lookup(payload.hostname);                                                   
       AAA_XMLDataPeer::print(*newPeer, true);
   }
   else {
       AAA_LOG(LM_INFO, "(%P|%t) WARING !!! - Unable to add peer: %s\n", 
               payload.hostname.data());
   }

   return (0);
}

void AAA_XMLDataPeer::print(AAA_PeerEntry &e, bool label)
{
   if (label) {
      AAA_LOG(LM_INFO, "(%P|%t)                Peer : Host = %s, Port = %d, TLS = %d\n", 
                    e.Data().m_Identity.data(), e.Data().m_Port, e.Data().m_TLS);
   }
   else {
      AAA_LOG(LM_INFO, "(%P|%t)                       Host = %s, Port = %d, TLS = %d\n", 
                    e.Data().m_Identity.data(), e.Data().m_Port, e.Data().m_TLS);
   }
}

int AAA_XMLDataRouteAppPeerEntry::svc(DOMNode *n)
{
   unsigned int metric;
   std::string sname;
   std::string tagName;

   tagName = "server";
   OD_Utl_XMLDataString server(tagName, sname);
   server.populate(n->getFirstChild());
   payload.Server() = sname;

   tagName = "metric";
   OD_Utl_XMLDataUInt32 met(tagName, metric);
   met.populate(n->getFirstChild());
   payload.Metric() = metric;

   AAA_XMLDataRouteAppPeerEntry::print(payload);
   
   return (0);
}

void AAA_XMLDataRouteAppPeerEntry::print(AAA_RouteServerEntry &server)
{
   AAA_LOG(LM_INFO, "(%P|%t)                          Server = %s, metric = %d\n", 
                 server.Server().data(), server.Metric());
}

int AAA_XMLDataRouteApplication::svc(DOMNode *n)
{
   diameter_unsigned32_t value;
   std::string tagName, empty = "";

   tagName = "application_id";
   OD_Utl_XMLDataUInt32 appId(tagName, value);
   appId.populate(n->getFirstChild());
   payload.ApplicationId() = value;

   tagName = "vendor_id";
   OD_Utl_XMLDataUInt32 vendorId(tagName, value);
   vendorId.populate(n->getFirstChild());
   payload.VendorId() = value;

   print(payload);
   
   tagName = "peer_entry";
   DOMNode *found = NULL, *sibling = n->getFirstChild();

   while (sibling) {

      AAA_RouteServerEntry *newPeer;
      ACE_NEW_RETURN(newPeer, AAA_RouteServerEntry(empty), (-1));

      AAA_XMLDataRouteAppPeerEntry peer(tagName, *newPeer);
      if (peer.populate(sibling, &found) == 0) {
         if (payload.Servers().Lookup(newPeer->Server())) {
             AAA_LOG(LM_INFO, "(%P|%t)                              *Duplicate server (replacing previous)\n");
         }
         payload.Servers().Add(*newPeer);
         sibling = found->getNextSibling();
      }
      else {
	 delete newPeer;
         break;
      }
   }
   
   return (0);
}

void AAA_XMLDataRouteApplication::print(AAA_RouteApplication &a)
{
   AAA_LOG(LM_INFO, "(%P|%t)                       Application Id=%d, Vendor=%d\n",
           a.ApplicationId(), a.VendorId());
}

int AAA_XMLDataRoute::svc(DOMNode *n)
{
   unsigned int role; 
   std::string realm;
   std::string tagName;

   tagName = "realm";
   OD_Utl_XMLDataString rlm(tagName, realm);
   rlm.populate(n->getFirstChild());
   payload.Realm() = realm;

   tagName = "role";
   OD_Utl_XMLDataUInt32 rle(tagName, role);
   rle.populate(n->getFirstChild());
   payload.Action() = AAA_ROUTE_ACTION(role);

   tagName = "application";
   DOMNode *found = NULL, *sibling = n->getFirstChild();

   AAA_XMLDataRoute::print(payload);

   while (sibling) {

      AAA_RouteApplication *newApp;
      ACE_NEW_RETURN(newApp, AAA_RouteApplication, (-1));

      AAA_XMLDataRouteApplication app(tagName, *newApp);
      if (app.populate(sibling, &found) == 0) {
          if (payload.Lookup
              (newApp->ApplicationId(),
               newApp->VendorId())) {
             AAA_LOG(LM_INFO, "(%P|%t)                        *Duplicate app id (replacing previous)\n");
          }
          payload.Add(*newApp);
          sibling = found->getNextSibling();
      }
      else {
	 delete newApp;
         break;
      }
   }
   
   return (0);
}

void AAA_XMLDataRoute::print(AAA_RouteEntry &e)
{
   AAA_LOG(LM_INFO, "(%P|%t)              Route  : Realm = %s, Action = %d\n", 
                e.Realm().data(), e.Action());
}

int AAA_XMLDataPeerTable::svc(DOMNode *n)
{
   unsigned int etime; 
   std::string tagName = "expiration_time";
   OD_Utl_XMLDataUInt32 et(tagName, etime);
   et.populate(n->getFirstChild());
   AAA_PEER_TABLE()->ExpirationTime() = etime;

   tagName = "peer";
   DOMNode *found = NULL, *sibling = n->getFirstChild();

   AAA_LOG(LM_INFO, "(%P|%t)  Peer Table :\n");

   while (sibling) {

      AAA_DataPeer newPeer;
      AAA_XMLDataPeer peer(tagName, newPeer, task);
      if (peer.populate(sibling, &found) == 0) {
         sibling = found->getNextSibling();
      }
      else {
         break;
      }
   }
   return (0);
}

int AAA_XMLDataRouteTable::svc(DOMNode *n)
{
   AAA_LOG(LM_INFO, "(%P|%t)  Route Table:\n");

   unsigned int expireTime;
   std::string empty = "";
   std::string tagName;
   
   tagName = "expire_time";
   OD_Utl_XMLDataUInt32 etime(tagName, expireTime);
   etime.populate(n->getFirstChild());
   AAA_ROUTE_TABLE()->ExpireTime(expireTime);

   tagName = "route";
   AAA_RouteEntry *newRte = NULL;
   
   DOMNode *found = NULL, *sibling = n->getFirstChild();
   while (sibling) {
       
      ACE_NEW_RETURN(newRte, AAA_RouteEntry(empty), (-1));

      AAA_XMLDataRoute route(tagName, *newRte);
      if (route.populate(sibling, &found) == 0) {
         if (AAA_ROUTE_TABLE()->Lookup(newRte->Realm())) {
             AAA_LOG(LM_INFO, "(%P|%t)                        *Duplicate route (replacing previous)\n");
         }
         AAA_ROUTE_TABLE()->Add(*newRte);
         sibling = found->getNextSibling();
      }
      else {
          delete newRte;
          break;
      }
   }

   ACE_NEW_RETURN(newRte, AAA_RouteEntry(empty), (-1));

   AAA_LOG(LM_INFO, "(%P|%t)  Default Route: \n");

   tagName = "default_route";   
   AAA_XMLDataRoute def_route(tagName, *newRte);
   if (def_route.populate(n->getFirstChild()) == 0) {
       AAA_ROUTE_TABLE()->DefaultRoute(*newRte);
   }
   else {
       delete newRte;
   }
   
   return (0);
}

int AAA_XMLDataTransportMngt::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "identity";
   OD_Utl_XMLDataString identity(tagName, payload.identity);
   identity.populate(n->getFirstChild());

   tagName = "realm";
   OD_Utl_XMLDataString realm(tagName, payload.realm);
   realm.populate(n->getFirstChild());

   tagName = "tcp_port";
   OD_Utl_XMLDataUInt32 tcp_port(tagName, payload.tcp_port);
   tcp_port.populate(n->getFirstChild());
  
   tagName = "tls_port";
   OD_Utl_XMLDataUInt32 tls_port(tagName, payload.tls_port);
   tls_port.populate(n->getFirstChild());
  
   tagName = "watchdog_timeout";
   OD_Utl_XMLDataUInt32 watchdog(tagName, payload.watchdog_timeout);
   watchdog.populate(n->getFirstChild());
  
   tagName = "retry_interval";
   OD_Utl_XMLDataUInt32 retry(tagName, payload.retry_interval);
   retry.populate(n->getFirstChild());

   tagName = "advertised_host_ip";
   DOMNode *found, *sibling = n->getFirstChild();

   while (sibling) {
      std::string host_ip;
      OD_Utl_XMLDataString hip(tagName, host_ip);      
      if (hip.populate(sibling, &found) == 0) {
      
         std::string testAddr(host_ip + ":0");
         ACE_INET_Addr hostAddr;
         
         if (! hostAddr.set(testAddr.data())) {
             payload.advertised_host_ip.push_back(testAddr); 
         }
         else {
             AAA_LOG(LM_INFO, "(%P|%t) WARNING: Invalid Host IP Addr [%s], will be ignored\n", 
                     host_ip.data());
         }
         sibling = found->getNextSibling();
      }
      else {
         break;
      }
   }
  
   AAA_XMLDataTransportMngt::print(payload);

   tagName = "peer_table";
   AAA_XMLDataPeerTable peerTable(tagName, task);
   peerTable.populate(n->getFirstChild());

   tagName = "route_table";
   AAA_XMLDataRouteTable routeTable(tagName);
   routeTable.populate(n->getFirstChild());

   return (0);
}

void AAA_XMLDataTransportMngt::print(AAA_DataTransportMngt &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)            Identity : %s\n", 
                 data.identity.data());
   AAA_LOG(LM_INFO, "(%P|%t)               Realm : %s\n", 
                 data.realm.data());
   AAA_LOG(LM_INFO, "(%P|%t)          TCP Listen : %d\n", 
                 data.tcp_port);
   AAA_LOG(LM_INFO, "(%P|%t)          TLS Listen : %d\n", 
                 data.tls_port);
   AAA_LOG(LM_INFO, "(%P|%t)   Watch-Dog timeout : %d\n", 
                 data.watchdog_timeout);
   AAA_LOG(LM_INFO, "(%P|%t)      Retry interval : %d\n", 
                 data.retry_interval);
                 
   std::list<std::string>::iterator i = data.advertised_host_ip.begin();
   for (; i != data.advertised_host_ip.end(); i++) {
        AAA_LOG(LM_INFO, "(%P|%t)        Host IP Addr : %s\n", 
                       (*i).data());
   }
}

int AAA_XMLDataAuthSession::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "stateful";
   OD_Utl_XMLDataUInt32 stateful(tagName, payload.stateful);
   stateful.populate(n->getFirstChild());

   if (payload.stateful) {
       payload.stateful = AAA_SESSION_STATE_MAINTAINED;
   }
   else {
       payload.stateful = AAA_SESSION_NO_STATE_MAINTAINED;
   }

   tagName = "session_timeout";
   OD_Utl_XMLDataUInt32 session(tagName, payload.sessionTm);
   session.populate(n->getFirstChild());

   tagName = "lifetime_timeout";
   OD_Utl_XMLDataUInt32 lifetime(tagName, payload.lifetimeTm);
   lifetime.populate(n->getFirstChild());

   tagName = "grace_period_timeout";
   OD_Utl_XMLDataUInt32 grace(tagName, payload.graceTm);
   grace.populate(n->getFirstChild());

   tagName = "abort_retry_timeout";
   OD_Utl_XMLDataUInt32 abort(tagName, payload.abortRetryTm);
   abort.populate(n->getFirstChild());

   AAA_XMLDataAuthSession::print(payload);

   return (0);
}

void AAA_XMLDataAuthSession::print(AAA_DataAuthSession &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)       Stateful Auth : %d\n", 
                 data.stateful);
   AAA_LOG(LM_INFO, "(%P|%t)          Session(T) : %d\n", 
                 data.sessionTm);
   AAA_LOG(LM_INFO, "(%P|%t)         Lifetime(T) : %d\n", 
                 data.lifetimeTm);
   AAA_LOG(LM_INFO, "(%P|%t)            Grace(T) : %d\n", 
                 data.graceTm);
   AAA_LOG(LM_INFO, "(%P|%t)            Abort(T) : %d\n", 
                 data.abortRetryTm);
}

int AAA_XMLDataAcctSession::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "session_timeout";
   OD_Utl_XMLDataUInt32 session(tagName, payload.sessionTm);
   session.populate(n->getFirstChild());

   tagName = "interim_interval";
   OD_Utl_XMLDataUInt32 interim(tagName, payload.recIntervalTm);
   interim.populate(n->getFirstChild());

   tagName = "realtime";
   OD_Utl_XMLDataUInt32 realtime(tagName, payload.realtime);
   realtime.populate(n->getFirstChild());

   AAA_XMLDataAcctSession::print(payload);

   return (0);
}

void AAA_XMLDataAcctSession::print(AAA_DataAcctSession &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)          Session(T) : %d\n", 
                 data.sessionTm);
   AAA_LOG(LM_INFO, "(%P|%t)    Interim Interval : %d\n", 
                 data.recIntervalTm);
   AAA_LOG(LM_INFO, "(%P|%t)  Real-Time Required : %d\n", 
                 data.realtime);
}

int AAA_XMLDataSessionMngt::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "max_sessions";
   OD_Utl_XMLDataUInt32 maxSessions(tagName, payload.maxSessions);
   maxSessions.populate(n->getFirstChild());

   tagName = "auth_sessions";
   AAA_XMLDataAuthSession auth(tagName, payload.authSessions);
   auth.populate(n->getFirstChild());

   AAA_XMLDataSessionMngt::print(payload);

   tagName = "acct_sessions";
   AAA_XMLDataAcctSession acct(tagName, payload.acctSessions);
   acct.populate(n->getFirstChild());

   return (0);
}

void AAA_XMLDataSessionMngt::print(AAA_DataSessionMngt &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)            Max Sess : %d\n", 
                 data.maxSessions);
}

int AAA_XMLDataLogFlags::svc(DOMNode *n)
{
   std::string tagName;
   std::string debugFlg;
   std::string traceFlg;
   std::string infoFlg;

   tagName = "debug";
   OD_Utl_XMLDataString debug(tagName, debugFlg);
   debug.populate(n->getFirstChild());

   tagName = "trace";
   OD_Utl_XMLDataString trace(tagName, traceFlg);
   trace.populate(n->getFirstChild());

   tagName = "info";
   OD_Utl_XMLDataString info(tagName, infoFlg);
   info.populate(n->getFirstChild());

   payload.debug = (debugFlg == std::string("enabled")) ? true : false;
   payload.trace = (traceFlg == std::string("enabled")) ? true : false;
   payload.info  = (infoFlg == std::string("enabled")) ? true : false;

   AAA_XMLDataLogFlags::print(payload);

   return (0);
}

void AAA_XMLDataLogFlags::print(AAA_DataLogFlags &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)           Debug Log : %s\n", 
                 data.debug ? "enabled" : "disabled");
   AAA_LOG(LM_INFO, "(%P|%t)           Trace Log : %s\n", 
                 data.trace ? "enabled" : "disabled");
   AAA_LOG(LM_INFO, "(%P|%t)            Info Log : %s\n", 
                 data.info ? "enabled" : "disabled");
}

int AAA_XMLDataLogTargets::svc(DOMNode *n)
{
   std::string tagName;
   std::string consoleTarget;
   std::string syslogTarget;

   tagName = "console";
   OD_Utl_XMLDataString console(tagName, consoleTarget);
   console.populate(n->getFirstChild());

   tagName = "syslog";
   OD_Utl_XMLDataString syslog(tagName, syslogTarget);
   syslog.populate(n->getFirstChild());

   payload.console = (consoleTarget == std::string("enabled")) ? true : false;
   payload.syslog  = (syslogTarget == std::string("enabled")) ? true : false;

   AAA_XMLDataLogTargets::print(payload);

   return (0);
}

void AAA_XMLDataLogTargets::print(AAA_DataLogTragetFlags &data)
{
   AAA_LOG(LM_INFO, "(%P|%t)         Console Log : %s\n", 
                 data.console ? "enabled" : "disabled");
   AAA_LOG(LM_INFO, "(%P|%t)          Syslog Log : %s\n", 
                 data.syslog  ? "enabled" : "disabled");
}

int AAA_XMLDataLog::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "flags";
   AAA_XMLDataLogFlags flags(tagName, payload.flags);
   flags.populate(n->getFirstChild());

   tagName = "target";
   AAA_XMLDataLogTargets targets(tagName, payload.targets);
   targets.populate(n->getFirstChild());

   return (0);
}

int AAA_XMLDataRoot::svc(DOMNode *n)
{
   std::string tagName;

   tagName = "general";
   AAA_XMLDataGeneral general(tagName,
                              payload.general);
   general.populate(n->getFirstChild());

   tagName = "parser";
   AAA_XMLDataParser parser(tagName, payload.parser);
   parser.populate(n->getFirstChild());

   tagName = "transport_mngt";
   AAA_XMLDataTransportMngt transport(tagName,
                                      payload.transport,
                                      task);
   transport.populate(n->getFirstChild());

   tagName = "session_mngt";
   AAA_XMLDataSessionMngt session(tagName,
                                  payload.session);
   session.populate(n->getFirstChild());

   tagName = "log";
   AAA_XMLDataLog log(tagName, payload.log);
   log.populate(n->getFirstChild());

   return (0);
}


