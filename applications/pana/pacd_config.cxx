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

#include <iostream>
#include "pacd_config.h"
#include "od_utl_xml_parser.h"

class PACD_XMLRoot : public OD_Utl_XMLElementParser {
   public:
      PACD_XMLRoot(std::string &name, PACD_Data &e) :
                   OD_Utl_XMLElementParser(name),
	           m_Payload(e) { }
      int svc(DOMNode *n);
      void print();
   protected:
      PACD_Data &m_Payload;
};

int PACD_XMLRoot::svc(DOMNode *n)
{
   std::string tagName;

   tagName ="pana_cfg_file";
   OD_Utl_XMLDataString cfgfile(tagName, m_Payload.m_PaCCfgFile);
   cfgfile.populate(n->getFirstChild());
   
   tagName ="username";
   OD_Utl_XMLDataString username(tagName, m_Payload.m_Username);
   username.populate(n->getFirstChild());

   tagName ="password";
   OD_Utl_XMLDataString passwd(tagName, m_Payload.m_Password);
   passwd.populate(n->getFirstChild());

   tagName ="secret";
   OD_Utl_XMLDataString secret(tagName, m_Payload.m_Secret);
   secret.populate(n->getFirstChild());

   tagName ="auth_script";
   OD_Utl_XMLDataString script(tagName, m_Payload.m_AuthScript);
   script.populate(n->getFirstChild());

   tagName = "dhcp_bootstrap";
   OD_Utl_XMLDataUInt32 dhcp(tagName, m_Payload.m_DhcpBootstrap);
   dhcp.populate(n->getFirstChild());

   tagName = "use_archie";
   OD_Utl_XMLDataUInt32 archie(tagName, m_Payload.m_UseArchie);
   archie.populate(n->getFirstChild());

   tagName = "auth_period";
   OD_Utl_XMLDataUInt32 auth(tagName, m_Payload.m_AuthPeriod);
   auth.populate(n->getFirstChild());

   tagName = "thread_count";
   OD_Utl_XMLDataUInt32 thread(tagName, m_Payload.m_ThreadCount);
   thread.populate(n->getFirstChild());

   print();

   return (0);
}

void PACD_XMLRoot::print()
{
   std::cout << "PACD configuration (ver 1.0.0)" 
             << std::endl;
   std::cout << "     PANA config file: " 
             << m_Payload.m_PaCCfgFile
             << std::endl;
   std::cout << "     Username        : " 
             << m_Payload.m_Username
             << std::endl;
   std::cout << "     Auth script     : " 
             << m_Payload.m_AuthScript
             << std::endl;
   std::cout << "     DHCP bootstrap  : ";
   std::cout <<  m_Payload.m_DhcpBootstrap;
   std::cout << std::endl;
   std::cout << "     Use Archie      : ";
   std::cout <<  m_Payload.m_UseArchie;
   std::cout << std::endl;
   std::cout << "     Auth Period     : ";
   std::cout <<  m_Payload.m_AuthPeriod;
   std::cout << std::endl;
   std::cout << "     Thread Count    : ";
   std::cout <<  m_Payload.m_ThreadCount;
   std::cout << std::endl;
}

int PACD_Config::Open(std::string &cfgfile)
{
    std::string cfgRoot = "configuration";
    OD_Utl_XMLTreeParser parser;
    PACD_XMLRoot configData(cfgRoot, m_Data);
    return parser.open(cfgfile, configData);
}

