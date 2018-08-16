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

/* Author   : Victor I. Fajardo
 * Synopsis : Base class for accouting support
 */

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <sstream>
#include <string>
#include "aaa_log_facility.h"
#include "diameter_api.h"

#if defined(XERCES_HAS_CPP_NAMESPACE)
using namespace xercesc;
#endif

class AAAXstr
{
   public:
      AAAXstr(const char *transCode) {
  	  unicode = XMLString::transcode(transCode);
      }
      ~AAAXstr() {
	  XMLString::release(&unicode);
      }
      XMLCh *uniCode() { return unicode; }

   private:
      XMLCh *unicode;
};

#define X(code)  AAAXstr(code).uniCode()

class AAAXelement
{
   public:
      AAAXelement(DOMDocument *doc, DOMElement *parent, const char *name);
      ~AAAXelement();

      void SetText(const char *text);
      void SetText(ACE_UINT32 num);
      void SetText(ACE_UINT64 num);
      void SetText(diameter_uri_t &uri);

      void SetAttirbute(const char *name, const char *value);
      void SetAttirbute(const char *name, ACE_UINT32 num);
      void SetAttirbute(const char *name, ACE_UINT64 num);

      const DOMElement *GetElement() { return element; }

   private:
      DOMElement *element;
      DOMDocument *document;
};

class AAAXwriter
{
   public:
      AAAXwriter() { };

      const char *writeToString(AAAMessage *msg);

   protected:
      AAAReturnCode Walk(AAAAvpContainerList &avplist,
                         const DOMDocument *doc, 
                         const DOMElement *parent);
};

AAAXelement::AAAXelement(DOMDocument *doc, DOMElement *parent, const char *name) 
{
   document = doc;
   element = document->createElement(X(name));
   parent->appendChild(element);
}

AAAXelement::~AAAXelement() 
{
   // do nothing
}

void AAAXelement::SetText(const char *text) 
{
   DOMText* dataVal = document->createTextNode(X(text));
   element->appendChild(dataVal);
}

void AAAXelement::SetText(ACE_UINT32 num) 
{
   std::ostringstream holder;
   holder << num;
   SetText(holder.str().c_str());
}

void AAAXelement::SetText(ACE_UINT64 num) 
{
   std::ostringstream holder;
#if !WIN32
   holder << num;
#else
   holder << "undefined";
#endif
   SetText(holder.str().c_str());
}

void AAAXelement::SetAttirbute(const char *name, const char *value) 
{
   element->setAttribute(X(name), X(value));
}

void AAAXelement::SetAttirbute(const char *name, ACE_UINT32 num) 
{
   std::ostringstream holder;
   holder << num;
   element->setAttribute(X(name), X(holder.str().c_str()));
}

void AAAXelement::SetAttirbute(const char *name, ACE_UINT64 num) 
{
   std::ostringstream holder;
#if !WIN32
   holder << num;
#else
   holder << "undefined";
#endif
   element->setAttribute(X(name), X(holder.str().c_str()));
}

void AAAXelement::SetText(diameter_uri_t &uri) 
{
   std::ostringstream struri;

   struri << uri.fqdn << ":" << uri.port;

   switch (uri.transport) {
      case TRANSPORT_PROTO_TCP: struri << ";transport=tcp"; break;
      case TRANSPORT_PROTO_SCTP: struri << ";transport=sctp"; break;
      case TRANSPORT_PROTO_UDP: struri << ";transport=udp"; break;
   }

   switch (uri.protocol) {
      case AAA_PROTO_DIAMETER: struri << ";protocol=diameter"; break;
      case AAA_PROTO_RADIUS: struri << ";protocol=radius"; break;
      case AAA_PROTO_TACACSPLUS: struri << ";protocol=tacacsplus"; break;
   }

   switch (uri.scheme) {
      case AAA_SCHEME_AAA: struri << ";scheme=aaa"; break;
       case AAA_SCHEME_AAAS: struri << ";scheme=aaas"; break;
   }
   SetText(struri.str().c_str());
}

const char *AAAXwriter::writeToString(AAAMessage *msg)
{
   // Initialize the XML4C2 system.
   try {
      XMLPlatformUtils::Initialize();
   }
   catch(const XMLException& toCatch) {

      char *pMsg = XMLString::transcode(toCatch.getMessage());
      AAA_LOG(LM_ERROR, "(%P|%t) Accounting session converter intialization failed.\n");
      XMLString::release(&pMsg);

      return (NULL);
   }

   DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));

   DOMDocument* doc = impl->createDocument(0,            // root element namespace URI.
                                           X("Message"), // root element name
                                           0);           // document type object (DTD).

   DOMElement* msgElem = doc->getDocumentElement();

   AAAXelement version(doc, msgElem, "version");
   version.SetText(ACE_UINT32(msg->hdr.ver));

   AAAXelement flags(doc, msgElem, "flags");
   flags.SetAttirbute("request", ACE_UINT32(msg->hdr.flags.r));
   flags.SetAttirbute("proxiable", ACE_UINT32(msg->hdr.flags.p));
   flags.SetAttirbute("error", ACE_UINT32(msg->hdr.flags.e));
   flags.SetAttirbute("retrans", ACE_UINT32(msg->hdr.flags.t));

   AAAXelement code(doc, msgElem, "code");
   code.SetText(msg->hdr.code);

   AAAXelement appId(doc, msgElem, "appId");
   appId.SetText(msg->hdr.appId);

   AAAXelement HopId(doc, msgElem, "HopId");
   HopId.SetText(msg->hdr.hh);

   AAAXelement EndId(doc, msgElem, "EndId");
   EndId.SetText(msg->hdr.ee);

   AAAXelement Avps(doc, msgElem, "Avp");

   if (Walk(msg->acl, doc, Avps.GetElement()) == AAA_ERR_SUCCESS) {

      // get a serializer, an instance of DOMWriter
      DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(X("LS"));
      DOMWriter *theSerializer = reinterpret_cast<DOMImplementationLS*>(impl)->createDOMWriter();

      DOMNode *node = doc->getDocumentElement();
      XMLCh *unicode = theSerializer->writeToString(*node);

      char *output = XMLString::transcode(unicode);
      XMLString::release(&unicode);

      return (output);
   }
   return (NULL);
}

AAAReturnCode AAAXwriter::Walk(AAAAvpContainerList &avplist,
                               const DOMDocument *doc, 
                               const DOMElement *parent)
{
   std::list<AAAAvpContainer*>::iterator i;
   for (i = avplist.begin(); i != avplist.end(); i ++) {   

      AAAAvpContainer *c = *i;
      AAAAvpContainerEntry *entry;
      AAAXelement avp((DOMDocument*)doc, 
                      (DOMElement*)parent, 
                      c->getAvpName());

      size_t y;
      for (y = 0; y < c->size(); y++) {
  	 entry = (*c)[y];
         switch (entry->dataType()) {
	    case AAA_AVP_ADDRESS_TYPE:
	       {
	           diameter_address_t &val = entry->dataRef
                         (Type2Type<diameter_address_t>());
	           avp.SetText(val.value.data());
               }
	       break;
               break;
	    case AAA_AVP_STRING_TYPE:
	    case AAA_AVP_UTF8_STRING_TYPE:
	    case AAA_AVP_DIAMID_TYPE:
	       {
	           diameter_octetstring_t &val = entry->dataRef
                         (Type2Type<diameter_octetstring_t>());
	           avp.SetText(val.data());
               }
	       break;
	    case AAA_AVP_INTEGER32_TYPE:
	    case AAA_AVP_UINTEGER32_TYPE:
	    case AAA_AVP_ENUM_TYPE:
	       {
                  diameter_unsigned32_t &val = entry->dataRef
                        (Type2Type<diameter_unsigned32_t>());
	          avp.SetText(val);
               }
	       break;
	    case AAA_AVP_INTEGER64_TYPE:
	    case AAA_AVP_UINTEGER64_TYPE:
	       {
                  diameter_unsigned64_t &val = entry->dataRef
                        (Type2Type<diameter_unsigned64_t>());
	          avp.SetText(val);
               }
	       break;
  	    case AAA_AVP_GROUPED_TYPE:
	       {
                  diameter_grouped_t &val = entry->dataRef
                        (Type2Type<diameter_grouped_t>());
	          Walk(val, doc, avp.GetElement());
               }
	       break;
	    case AAA_AVP_DIAMURI_TYPE:
	       {
                  diameter_uri_t &val = entry->dataRef
                        (Type2Type<diameter_uri_t>());
	          avp.SetText(val);
               }
	       break;
	    default:
	       break;
	 }
      }   
   }

   return (AAA_ERR_SUCCESS);
}

/*!
 *  <Message>
 *     <version>value</version>
 *     <flags request="value" proxiable="value" error="value" retrans="value"></flags>
 *     <code>value</code>
 *     <appId>value</appId>
 *     <HopId>value</HopId>
 *     <EndId>value</EndId>
 *     <avp>
 *        <"avpname">value</avp>
 *          .
 *          .
 *        <"avpname">
 *           <"avpname">value</"avpname">
 *           <"avpname">value</"avpname">
 *               .
 *               .
 *           <"avpname">
 *              <"avpname">value</"avpname">
 *                 .
 *                 .
 *              </"avpname">
 *        </"avpname">
 *     </avp>
 *  </Message>
 */
AAAReturnCode AAAAccountingXMLRecTransformer::Convert(AAAMessage *msg)
{
   AAAXwriter writer;

   char *output = (char*)writer.writeToString(msg);

   if (output) {
      record = reinterpret_cast<void*>(output);
      record_size = ACE_OS::strlen(output);
   }
   else {
      record = NULL;
      record_size = 0;
   }

   return (AAA_ERR_SUCCESS);
}

AAAReturnCode AAAAccountingXMLRecTransformer::OutputRecord(AAAMessage *originalMessage)
{
   AAA_LOG(LM_DEBUG, "(%P|%t) Server: Default output record handler\n");

   if (record) {

      AAA_LOG(LM_DEBUG, "(%P|%t) Server: Resetting record holder\n");

      char * rec = (char*)record;
      XMLString::release(&rec);

      record = NULL;
      record_size = 0;
   }
   return (AAA_ERR_SUCCESS);
}

