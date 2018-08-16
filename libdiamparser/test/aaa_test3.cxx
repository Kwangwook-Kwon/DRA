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
/* $Id: test3.cxx,v 1.20 2004/06/17 21:07:50 yohba Exp $ */
/* 
   Sample program to show how to use diamparser library. 
   Written by Yoshihihiro Ohba (yohba@tari.toshiba.com)
   Modified by Santosh Kawade (skawade@mahindrabt.com) for thread-safe
   testing.
*/

#include <string>
#include <iostream>
#include <ace/OS.h>
#include <ace/INET_Addr.h>
#include <ace/Thread_Manager.h>
#include "diameter_parser.h"
using namespace std;

#define GET_DATA_REF(dataType, data, containerEntryPtr) \
        dataType &data = (containerEntryPtr)->dataRef(Type2Type<dataType>())

#define NUM_THREADS 100   
unsigned char rbuf[] = 
{ 
  0x01, 0x00, 0x00, 0x74, 0x90, 0x00, 0x01, 0x01, /* header(CER, T-bit set) */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* (length = 156-byte) */
  0x00, 0x00, 0x00, 0x0a,     
                          0x00, 0x00, 0x01, 0x08, /* Origin-Host */
#ifdef ERROR_CASE
  0x00, 0x00, 0x00, 0x13, 'a', 'a', 'a', '.', /* M-flag is off for err test */
#else
  0x40, 0x00, 0x00, 0x13, 'a', 'a', 'a', '.', 
#endif
  'b', 'b', 'b', '.', 'c', 'o', 'm', 0x00, 
  0x00, 0x00, 0x01, 0x28, 0x40, 0x00, 0x00, 0x0f, /* Origin-Realm */
  'a', 'a', 'a', '.', 'c', 'o', 'm',  0x00, 
  0x00, 0x00, 0x01, 0x01, 0x40, 0x00, 0x00, 0x0c, /* Host-IP-Address */
  0x0a, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x0a, /* Vendor-Id */
  0x40, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 0x00, 0x18, /* Product-Name */
  't', 'o', 's', 'h', 'i', 'b', 'a', '-',
  'd', 'i', 'a', 'm', 'e', 't', 'e', 'r',
  0x00, 0x00, 0x01, 0x16, 0x40, 0x00, 0x00, 0x0c, /* Origin-State-Id */
  0x00, 0x00, 0x2b, 0x67
};


char addr[] = {0x0a, 0x01, 0x01, 0x01};
char orhost_str[] = { "aaa.bbb.com"};

static void 
inet_convert(ACE_INET_Addr& ace_ipaddr,
             diameter_address_t& diam_ipaddr)
{
//   diam_ipaddr contains a raw IP address octet string in network byte order.
    ace_ipaddr.set_address((const char*)(diam_ipaddr.value.data()),
			   diam_ipaddr.value.size(), 0);
}

static void
print_header(DiameterMsgHeader &h)
{
  cout << "version = " << (int)h.ver << endl;
  cout << "length=" << h.length << "\n";
  cout << "flags(r,p,e,t)=(" 
       << (int)h.flags.r << "," 
       << (int)h.flags.p << ","
       << (int)h.flags.e << ","
       << (int)h.flags.t << ")" << endl;
  cout << "applicationId = " << h.appId << endl;
  cout << "h-h id = " << h.hh << endl;
  cout << "e-e id = " << h.ee << endl;
}

static void
rtest(unsigned char *buf, int size)
{
  unsigned int i, j;
  DiameterMsg msg;
  AAAMessageBlock *aBuffer;
  AAAAvpContainer* c;

  DiameterMsgHeaderParser hp;
  aBuffer = AAAMessageBlock::Acquire((char*)buf, size);
  hp.setRawData(aBuffer);
  hp.setAppData(&msg.hdr);
  hp.setDictData(DIAMETER_PARSE_STRICT);

  try {
    hp.parseRawToApp();
  }
  catch (DiameterErrorCode &st) 
    {
      cout << "header error" << endl;
      aBuffer->Release();
      exit(1);
    }

  print_header(msg.hdr);

  aBuffer->size(msg.hdr.length);
  DiameterMsgPayloadParser pp;
  pp.setRawData(aBuffer);
  pp.setAppData(&msg.acl);
  pp.setDictData(msg.hdr.getDictHandle());

  try {
    pp.parseRawToApp();
  }
  catch (DiameterErrorCode &st) {
    std::cout << "payload parser error" << endl;
      AAA_PARSE_ERROR_TYPE type;
      int code;
      std::string avp;
      st.get(type, code, avp);
      std::cout << "Error type=" << type << ", code=" 
		<< code << ", name=" << avp << std::endl;
    msg.acl.releaseContainers();
    aBuffer->Release();
    exit(1);
  }

  std::cout << "Disassemble " << msg.hdr.getCommandName() << " success." 
	    << std::endl;

  if ((c = msg.acl.search("OriginHost")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_identity_t, orhost, (*c)[i]);
	  std::cout << "Origin-Host = " << orhost.c_str() << std::endl;
	}
    }

  if ((c = msg.acl.search("OriginRealm")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_utf8string_t, orrealm, (*c)[i]);
	  std::cout << "OriginRealm = " << orrealm.c_str() << std::endl;
	}
    }

  if ((c = msg.acl.search("HostIPAddress")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_address_t, hostip, (*c)[i]);
	  ACE_INET_Addr addr;
	  inet_convert(addr, hostip);
	  std::cout << "HostIPAddress = " 
		    << addr.get_host_addr()
		    << std::endl;
	}
    }

  if ((c = msg.acl.search("VendorId")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_unsigned32_t, vid, (*c)[i]);
	  std::cout << "Vendor-Id = " << vid << std::endl;
	}
    }

  if ((c = msg.acl.search("ProductName")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_utf8string_t, product, (*c)[i]);
	  std::cout << "ProductName = " << product.c_str() << std::endl;
	}
    }

  if ((c = msg.acl.search("OriginStateId")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_unsigned32_t, orstatid, (*c)[i]);
	  std::cout << "OriginStateId = " << orstatid << std::endl;
	}
    }

  if ((c = msg.acl.search("SupportedVendorId")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_unsigned32_t, sup_vid, (*c)[i]);
	  std::cout << "SupportedVendorId = " << sup_vid << std::endl;
	}
    }

  if ((c = msg.acl.search("AuthApplicationId")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_integer32_t, auth_appid, (*c)[i]);
	  std::cout << "AuthApplicationId = " << auth_appid << std::endl;
	}
    }

  if ((c = msg.acl.search("AcctApplicationId")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_integer32_t, acct_appid, (*c)[i]);
	  std::cout << "AcctApplicationId = " << acct_appid << std::endl;
	}
    }

  if ((c = msg.acl.search("VendorSpecificApplicationId")))
    {
      AAAAvpContainer* cc;
      std::cout << "VendorSpecificApplicationId = ";
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(AAAAvpContainerList, acl, (*c)[i]);
	  if ((cc = acl.search("ProxyHost")))
	    {
	      for (j=0; j<cc->size(); j++)
		{
		  GET_DATA_REF(diameter_unsigned32_t, sup_vid, (*cc)[j]);
		  std::cout << "\t" << "ProxyHost = " << sup_vid << std::endl;
		}
	    }
	  if ((cc = acl.search("AuthApplicationId")))
	    {
	      for (j=0; j<cc->size(); j++)
		{
		  GET_DATA_REF(diameter_integer32_t, auth_appid, (*cc)[j]);
		  std::cout << "\t" << "AuthApplicationId = " << auth_appid 
			    << std::endl;
		}
	    }
	  if ((cc = acl.search("AcctApplicationId")))
	    {
	      for (j=0; j<cc->size(); j++)
		{
		  GET_DATA_REF(diameter_integer32_t, acct_appid, (*cc)[j]);
		  std::cout << "\t" << "AcctApplicationId = " << acct_appid 
			    << std::endl;
		}
	    }
	}
    }

  if ((c = msg.acl.search("FirmwareRevision")))
    {
      for (i=0; i<c->size(); i++)
	{
	  GET_DATA_REF(diameter_unsigned32_t, firm_rev, (*c)[i]);
	  std::cout << "FirmwareRevision = " << firm_rev << std::endl;
	}
    }

  if ((c = msg.acl.search("AVP")))
    {
      std::cout << "I'm not ready to process this AVP";
    }

  msg.acl.releaseContainers();
  aBuffer->Release();
}


static void
stest(unsigned char *buf, int size)
{
  DiameterAvpContainerManager cm;
  DiameterAvpContainerEntryManager em;
  AAAAvpContainer *c_orhost = cm.acquire("OriginHost");
  AAAAvpContainer *c_orrealm = cm.acquire("OriginRealm");
  AAAAvpContainer *c_hostip = cm.acquire("HostIPAddress");
  AAAAvpContainer *c_vid = cm.acquire("VendorId");
  AAAAvpContainer *c_product = cm.acquire("ProductName");
  AAAAvpContainer *c_orstatid = cm.acquire("OriginStateId");
  AAAAvpContainerEntry *e;

  diameter_hdr_flag flag = {1,0,0,1};
  DiameterMsgHeader h(1, 0, flag, 257, 0, 1, 10);

  DiameterMsg msg;
  AAAMessageBlock *aBuffer;
  msg.hdr = h;
  DiameterMsgHeaderParser hp;
  aBuffer = AAAMessageBlock::Acquire((char*)buf, size);
  hp.setRawData(aBuffer);
  hp.setAppData(&msg.hdr);
  hp.setDictData(DIAMETER_PARSE_STRICT);

  /* Check validity of flags and gets a DiameterCommand structure */
  try {
    hp.parseAppToRaw();
  }
  catch (DiameterErrorCode &st)
    {
      std::cout << "header error" << std::endl;;
      aBuffer->Release();
      exit(1);
    }

  e = em.acquire(AAA_AVP_DIAMID_TYPE);
  GET_DATA_REF(diameter_identity_t, orhost, e);
  c_orhost->add(e);

  e = em.acquire(AAA_AVP_DIAMID_TYPE);
  GET_DATA_REF(diameter_utf8string_t, orrealm, e);
  c_orrealm->add(e);

  e = em.acquire(AAA_AVP_IPADDRESS_TYPE);
  GET_DATA_REF(diameter_address_t, hostip, e);
  c_hostip->add(e);

  e = em.acquire(AAA_AVP_UINTEGER32_TYPE);
  GET_DATA_REF(diameter_unsigned32_t, vid, e);
  c_vid->add(e);

  e = em.acquire(AAA_AVP_UTF8_STRING_TYPE);
  GET_DATA_REF(diameter_utf8string_t, product, e);
  c_product->add(e);

  e = em.acquire(AAA_AVP_UINTEGER32_TYPE);
  GET_DATA_REF(diameter_unsigned32_t, orstatid, e);
  c_orstatid->add(e);

  msg.acl.add(c_orhost);
  msg.acl.add(c_orrealm);
  msg.acl.add(c_hostip);
  msg.acl.add(c_vid);
  msg.acl.add(c_product);
  msg.acl.add(c_orstatid);

  /* set values */
  /* OriginHost */
  orhost.assign(orhost_str);

  /* OriginRealm */
  orrealm.assign("aaa.com");

  /* HostIPAddress */
  hostip.type = AAA_ADDRESS_IP;
  hostip.value.assign(addr, sizeof(addr));

  /* VendorId */
  vid = 1;

  /* ProductName */
  product.assign("toshiba-diameter");

  /* OriginStateId */
  orstatid = 1111;

  DiameterMsgPayloadParser pp;
  pp.setRawData(aBuffer);
  pp.setAppData(&msg.acl);
  pp.setDictData(msg.hdr.getDictHandle());

  try 
   { 
     pp.parseAppToRaw();
   }
  catch (DiameterErrorCode &st)
    {
      std::cout << "assemble failed" << std::endl;
      aBuffer->Release();
      msg.acl.releaseContainers();
      exit(1);
    }

  msg.hdr.length = aBuffer->wr_ptr() - aBuffer->base();

  /* The second calll of hp.set() sets the actual message length */
  aBuffer->wr_ptr(aBuffer->base());
  try {
    hp.parseAppToRaw();
  }
  catch (DiameterErrorCode &st)
    {
      std::cout << "header error" << std::endl;
      msg.acl.releaseContainers();
      aBuffer->Release();
      exit(1);
    }

  std::cout << "assemble " << msg.hdr.getCommandName() 
	    << " success. total length = " << msg.hdr.length << std::endl;
  msg.acl.releaseContainers();
  aBuffer->Release();
}

ACE_THR_FUNC_RETURN worker(void *p)
{
        int i = 10;
  unsigned char sbuf[1024];
        for(i = 0 ; i !=10 ; i++)
        {
                ACE_OS::memset(sbuf,'\0',1024);
                stest(sbuf, sizeof(sbuf));
		rtest(sbuf, sizeof(sbuf));
        }
        return 0;
}

int
main(int argc, char** argv)
{
  DiameterDictionaryManager dm;

  int grp_id = 0;

  ACE_Thread_Manager *threads = ACE_Thread_Manager::instance();

  // Start logging with specifying the use of syslog
  //  ACE_Log_Msg::instance()->open(argv[0], ACE_Log_Msg::SYSLOG);

  // Read dictionary file.
  dm.init("./config/dictionary.xml");
  grp_id = threads->spawn_n(NUM_THREADS, worker);
  if (grp_id < 1) {
      printf("Thread creation failed\n");
  }

  printf( " ************** Finished spawning threads \n");

  threads->wait_grp(grp_id);

  return 0;                              
}

