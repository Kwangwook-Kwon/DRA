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

#ifndef __AAA_XML_DATA_H__
#define __AAA_XML_DATA_H__

#include "aaa_data_defs.h"
#include "aaa_route_table.h"
#include "aaa_peer_table.h"
#include "od_utl_xml_parser.h"

class AAA_XMLDataGeneral : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataGeneral(std::string &name, AAA_DataGeneral &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataGeneral &data);

   protected:
      AAA_DataGeneral &payload;
};

class AAA_XMLDataParser : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataParser(std::string &name, AAA_DataParser &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataParser &data);

   protected:
      AAA_DataParser &payload;
};

class AAA_XMLDataVendorSpecificApplicationId : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataVendorSpecificApplicationId(std::string &name,
                    AAA_DataVendorSpecificApplicationId &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataVendorSpecificApplicationId &e);

   protected:
      AAA_DataVendorSpecificApplicationId &payload;
};

class AAA_XMLDataPeer : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataPeer(std::string &name, AAA_DataPeer &data,
                      AAA_Task &t) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data),
         task(t) { }

      int svc(DOMNode *n);

      static void print(AAA_PeerEntry &e, bool label);

   protected:
      AAA_DataPeer &payload;
      AAA_Task &task;
};

class AAA_XMLDataRouteAppPeerEntry : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataRouteAppPeerEntry(std::string &name,
                                   AAA_RouteServerEntry &server) :
 	 OD_Utl_XMLElementParser(name),
	 payload(server) { }

      int svc(DOMNode *n);

      static void print(AAA_RouteServerEntry &server);

   protected:
      AAA_RouteServerEntry &payload;
};

class AAA_XMLDataRouteApplication : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataRouteApplication(std::string &name,
                                  AAA_RouteApplication &app) :
 	 OD_Utl_XMLElementParser(name),
	 payload(app) { }

      int svc(DOMNode *n);

      static void print(AAA_RouteApplication &a);

   protected:
      AAA_RouteApplication &payload;
};

class AAA_XMLDataRoute : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataRoute(std::string &name, AAA_RouteEntry &e) : 
 	 OD_Utl_XMLElementParser(name),
	 payload(e) { }

      int svc(DOMNode *n);

      static void print(AAA_RouteEntry &e);

   protected:
      AAA_RouteEntry &payload;
};

class AAA_XMLDataPeerTable : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataPeerTable(std::string &name,
                           AAA_Task &t) :
 	 OD_Utl_XMLElementParser(name),
         task(t) { }

      int svc(DOMNode *n);

   protected:
      AAA_Task &task;
};

class AAA_XMLDataRouteTable : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataRouteTable(std::string &name) :
          OD_Utl_XMLElementParser(name) { }

      int svc(DOMNode *n);
};

class AAA_XMLDataTransportMngt : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataTransportMngt(std::string &name,
                               AAA_DataTransportMngt &data,
                               AAA_Task &t) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data),
         task(t) { }

      int svc(DOMNode *n);

      static void print(AAA_DataTransportMngt &data);

   protected:
      AAA_DataTransportMngt &payload;
      AAA_Task &task;
};

class AAA_XMLDataAuthSession : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataAuthSession(std::string &name, AAA_DataAuthSession &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataAuthSession &data);

   protected:
      AAA_DataAuthSession &payload;
};

class AAA_XMLDataAcctSession : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataAcctSession(std::string &name, AAA_DataAcctSession &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataAcctSession &data);

   protected:
      AAA_DataAcctSession &payload;
};

class AAA_XMLDataSessionMngt : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataSessionMngt(std::string &name, AAA_DataSessionMngt &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataSessionMngt &data);

   protected:
      AAA_DataSessionMngt &payload;
};

class AAA_XMLDataLogFlags : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataLogFlags(std::string &name, AAA_DataLogFlags &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataLogFlags &data);

   protected:
      AAA_DataLogFlags &payload;
};

class AAA_XMLDataLogTargets : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataLogTargets(std::string &name, AAA_DataLogTragetFlags &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

      static void print(AAA_DataLogTragetFlags &data);

   protected:
      AAA_DataLogTragetFlags &payload;
};

class AAA_XMLDataLog : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataLog(std::string &name, AAA_DataLog &data) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data) { }

      int svc(DOMNode *n);

   protected:
      AAA_DataLog &payload;
};

class AAA_XMLDataRoot : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLDataRoot(std::string &name,
                      AAA_DataRoot &data,
                      AAA_Task &t) :
 	 OD_Utl_XMLElementParser(name),
	 payload(data),
         task(t) { }

      int svc(DOMNode *n);

   protected:
      AAA_DataRoot &payload;
      AAA_Task &task;
};

#endif // __AAA_XML_DATA_H__


