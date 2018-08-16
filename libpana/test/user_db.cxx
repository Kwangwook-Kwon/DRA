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
#include "od_utl_xml_parser.h"
#include "user_db.h"

class AAA_XMLUserEntry : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLUserEntry(std::string &name, AAA_UserEntry &e) :
                       OD_Utl_XMLElementParser(name),
	               payload(e) { }
      int svc(DOMNode *n);
   protected:
      AAA_UserEntry &payload;
};

class AAA_XMLUserDb : public OD_Utl_XMLElementParser {
   public:
      AAA_XMLUserDb(std::string &name, AAA_UserEntryDb &data) :
                    OD_Utl_XMLElementParser(name),
	               payload(data) { }
      int svc(DOMNode *n);
   protected:
      AAA_UserEntryDb &payload;
};

int AAA_XMLUserEntry::svc(DOMNode *n)
{
   std::string tagName;

   tagName ="name";
   OD_Utl_XMLDataString name(tagName, payload.m_Username);
   name.populate(n->getFirstChild());
   
   tagName ="pass";
   OD_Utl_XMLDataString pass(tagName, payload.m_Passphrase);
   pass.populate(n->getFirstChild());

   tagName ="auth";
   std::string Type;
   OD_Utl_XMLDataString auth(tagName, Type);
   auth.populate(n->getFirstChild());

   if (Type == std::string("md5")) {
      payload.m_AuthType = 4;
   }
   else if (Type == std::string("archie")) {
      payload.m_AuthType = 100;
   }
   else {
      return (-1);
   }
   return (0);
}

int AAA_XMLUserDb::svc(DOMNode *n)
{
   std::string tagName = "user";
   DOMNode *sibling = n->getFirstChild();
   while (sibling != NULL) {
      if (sibling->getNodeType() == DOMNode::ELEMENT_NODE) {
          char *c_str = XMLString::transcode(sibling->getNodeName());
          if (XMLString::compareString(c_str, tagName.c_str()) == 0) {
              AAA_UserEntry *e = new AAA_UserEntry;
              AAA_XMLUserEntry entry(tagName, *e);
              if (entry.populate(sibling) == 0) {
                  payload.insert
                      (std::pair<std::string, AAA_UserEntry*>
                       (e->m_Username, e));
              }
          }
          XMLString::release(&c_str);
      }
      sibling = sibling->getNextSibling();
   }
   return (0);
}

int AAA_UserDb::open(std::string &cfgFile)
{
    std::string cfgRoot = "user_dbase";
    OD_Utl_XMLTreeParser parser;
    AAA_XMLUserDb configData(cfgRoot, m_UserDb);
    return parser.open(cfgFile, configData);
}

AAA_UserEntry *AAA_UserDb::lookup(const std::string &uname)
{
    AAA_UserEntryDbIterator i = m_UserDb.find(uname);
    return (i == m_UserDb.end()) ? NULL : i->second;
}

void AAA_UserDb::close()
{
    AAA_UserEntryDbIterator i;
    while (! m_UserDb.empty()) {
        i = m_UserDb.begin();
        delete i->second;
        m_UserDb.erase(i);
    }
}
