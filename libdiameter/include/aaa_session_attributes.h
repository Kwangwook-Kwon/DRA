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

#ifndef __AAA_SESSION_ATTRIBUTES_H__
#define __AAA_SESSION_ATTRIBUTES_H__

#define AAA_SESSION_DEBUG 0

#include "diameter_parser_api.h"

template <typename T>
class AAA_NegotiatedAttribute
{
 public:
  AAA_NegotiatedAttribute() : 
       isNegotiated(false) {
  }
  AAA_NegotiatedAttribute(T &val) : 
       value(val), 
       isNegotiated(false) {
  }
  virtual ~AAA_NegotiatedAttribute() {
  }
  inline void Clear() { 
      isNegotiated = false; 
  }
  inline void Set(T val) { 
      value=val; 
      isNegotiated=true; 
  }
  inline bool operator==(const T& v) const { 
      return value == v; 
  }
  inline T& operator()() { 
      return value; 
  }
  inline T& operator=(T v) { 
      value=v; 
      return value; 
  }
  inline bool& IsNegotiated() { 
      return isNegotiated; 
  }

 protected:
  T value;
  bool isNegotiated;
};

class DIAMETERBASEPROTOCOL_EXPORT AAA_SessionCounter {
     public:
        AAA_SessionCounter(diameter_unsigned32_t h = 0,
                           diameter_unsigned32_t l = 0) :
            m_High(h),
            m_Low(l) {
        }        
        diameter_unsigned32_t &High() {
            return m_High;
        }
        diameter_unsigned32_t &Low()  {
            return m_Low;
        }
        int operator=(AAA_SessionCounter &cntr) {
            High() = cntr.High();
            Low() = cntr.Low();
            return (true);
        }
        int operator==(AAA_SessionCounter &cntr) {
            return ((m_High == cntr.High()) &&
                    (m_Low == cntr.Low()));
        }
        int operator<(AAA_SessionCounter &cntr) {
           if (m_High < cntr.High()) {
              return (true);
           }
           else if ((m_High == cntr.High()) &&
                    (m_Low < cntr.Low())) {
              return (true);
           }
           return (false);
        }
        int operator++() {
           m_Low ++;
           if (m_Low == 0xffffffff) {
              m_Low = 0x0;
              m_High ++;
           }
           if (m_High == 0xffffffff) {
              m_High = 0;
           }
           return (true);
        }

     protected:
        diameter_unsigned32_t m_High;
        diameter_unsigned32_t m_Low;
};

class DIAMETERBASEPROTOCOL_EXPORT AAA_SessionId : public AAA_SessionCounter {
    public:
        AAA_SessionId() {
        }
        virtual ~AAA_SessionId() {
        }
        bool operator==(AAA_SessionId &id) {
            if ((AAA_SessionCounter&)(*this) ==
                (AAA_SessionCounter&)id) {
                if (DiameterId() == id.DiameterId()) {
                    if (OptionalValue().length() > 0) {
                        if (OptionalValue() == 
                            id.OptionalValue()) {
                            return true;
                        }
                    }
                    else {
                        return true;
                    }
                }
            }
            return false;
        }
        bool operator=(AAA_SessionId &id) {
            High() = id.High();
            Low() = id.Low();
            DiameterId() = id.DiameterId();
            OptionalValue() = id.OptionalValue();
            return true;
        }
        std::string &DiameterId() {
            return m_DiameterId;
        }
        std::string &OptionalValue() {
            return m_OptionalVal;
        }
        bool IsEmpty() {
            return (m_DiameterId.length() == 0) ? true : false;
        }
        AAAReturnCode Get(AAAMessage &msg);
        AAAReturnCode Set(AAAMessage &msg);
        void Dump(char *buf = NULL);
    private:
        std::string m_DiameterId;
        std::string m_OptionalVal;
};

class DIAMETERBASEPROTOCOL_EXPORT AAA_SessionAttributes : 
    public AAA_JobData 
{
    public:
        AAA_ScholarAttribute<std::string> &DestinationHost() {
            return m_DestinationHost;
        }
        AAA_ScholarAttribute<std::string> &DestinationRealm() {
            return m_DestinationRealm;
        }
        AAA_ScholarAttribute<std::string> &Username() {
            return m_Username;
        }
        AAA_NegotiatedAttribute<diameter_unsigned32_t> &SessionTimeout() {
            return m_SessionTimeout;
        }
        AAA_SessionId &SessionId() {
            return m_SessionId;
        }
        diameter_unsigned32_t &LastTxHopId() {
            return m_LastTxHopId;
        }
        diameter_unsigned32_t &LastTxEndId() {
            return m_LastTxEndId;
        }
        diameter_unsigned32_t &LastRxHopId() {
            return m_LastRxHopId;
        }
        diameter_unsigned32_t &LastRxEndId() {
            return m_LastRxEndId;
        }
        diameter_unsigned32_t &ApplicationId() {
            return m_ApplicationId;
        }
        void MsgIdTxMessage(AAAMessage &msg);
        bool MsgIdRxMessage(AAAMessage &msg);

    private:
        AAA_SessionId m_SessionId;            // session id (RTM)
        diameter_unsigned32_t m_LastTxHopId;
        diameter_unsigned32_t m_LastTxEndId;
        diameter_unsigned32_t m_LastRxHopId;
        diameter_unsigned32_t m_LastRxEndId;
        diameter_unsigned32_t m_ApplicationId;
        AAA_ScholarAttribute<std::string> m_DestinationHost;
        AAA_ScholarAttribute<std::string> m_DestinationRealm;
        AAA_ScholarAttribute<std::string> m_Username;
        AAA_NegotiatedAttribute<diameter_unsigned32_t> m_SessionTimeout;
};

class AAA_AuthSessionAttributes : 
    public AAA_SessionAttributes
{
    public:
        AAA_ScholarAttribute<AAA_ReAuthValue> &ReAuthRequestValue() {
            return m_ReAuthRequestValue;
        }
        AAA_NegotiatedAttribute<diameter_unsigned32_t> &AuthSessionState() {
            return m_AuthSessionState;
        }
        AAA_NegotiatedAttribute<diameter_unsigned32_t> &AuthLifetime() {
            return m_AuthLifetime;
        }
        AAA_NegotiatedAttribute<diameter_unsigned32_t> &AuthGrace() {
            return m_AuthGrace;
        }

    private:
        AAA_ScholarAttribute<AAA_ReAuthValue> m_ReAuthRequestValue;
        AAA_NegotiatedAttribute<diameter_unsigned32_t> m_AuthSessionState;
        AAA_NegotiatedAttribute<diameter_unsigned32_t> m_AuthLifetime;
        AAA_NegotiatedAttribute<diameter_unsigned32_t> m_AuthGrace;
};

class AAA_AcctSessionAttributes : 
    public AAA_SessionAttributes
{
    public:
        AAA_ScholarAttribute<diameter_unsigned64_t> &SubSessionId() {
            return m_SubSessionId;
        }
        AAA_NegotiatedAttribute<diameter_enumerated_t> &RealtimeRequired() {
            return m_RealtimeRequired;
        }
        AAA_NegotiatedAttribute<diameter_unsigned32_t> &InterimInterval() {
            return m_InterimInterval;
        }
        AAA_ScholarAttribute<diameter_unsigned32_t> &RecordType() {
            return m_RecordType;
        }
        AAA_ScholarAttribute<diameter_unsigned32_t> &RecordNumber() {
            return m_RecordNumber;
        }
        AAA_ScholarAttribute<diameter_octetstring_t> &RadiusAcctSessionId() {
            return m_RadiusAcctSessionId;
        }
        AAA_ScholarAttribute<diameter_utf8string_t> &MultiSessionId() {
            return m_MultiSessionId;
        }
        AAA_ScholarAttribute<bool> &BackwardCompatibility() {
            return m_BackwardCompatibilityEnabled;
        }

    private:
        AAA_ScholarAttribute<diameter_unsigned64_t> m_SubSessionId;
        AAA_NegotiatedAttribute<diameter_unsigned32_t> m_RealtimeRequired;
        AAA_NegotiatedAttribute<diameter_unsigned32_t> m_InterimInterval;
        AAA_ScholarAttribute<diameter_unsigned32_t> m_RecordType;
        AAA_ScholarAttribute<diameter_unsigned32_t> m_RecordNumber;
        AAA_ScholarAttribute<diameter_octetstring_t> m_RadiusAcctSessionId;
        AAA_ScholarAttribute<diameter_utf8string_t> m_MultiSessionId;
        AAA_ScholarAttribute<bool> m_BackwardCompatibilityEnabled;
};

#endif






































































