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

#ifndef __AAA_SESSION_FSM_H__
#define __AAA_SESSION_FSM_H__

#include "framework.h"
#include "diameter_parser_api.h"

template<class ARG, class DEBUG>
class AAA_SessionStateMachine :
    public AAA_StateMachineWithTimer<ARG>, AAA_Job
{
   public:
      virtual ~AAA_SessionStateMachine() {
          AAA_StateMachine<ARG>::Stop();
          do {
              ACE_Time_Value tv(0, 100000);
              ACE_OS::sleep(tv);
          } while (m_GroupedJob.Job().ExistBacklog());
          m_GroupedJob.Job().Flush(); 
          m_CurrentEventParam.reset();
      }    
      AAA_GroupedJob &Job() {
          return *m_GroupedJob.get();
      }
      AAA_State State() {
          return AAA_StateMachineWithTimer<ARG>::state;
      }
      bool IsRunning() {
          return AAA_StateMachine<ARG>::Running();
      }
      typedef struct {
          AAA_Event m_Event;
          std::auto_ptr<AAAMessage> m_RxMsg;
          std::auto_ptr<AAAMessage> m_TxMsg;
      } AAA_SessionEventParam;

   private:
      AAA_ProtectedPtrQueue<AAA_SessionEventParam> m_EventQueue;
      std::auto_ptr<AAA_SessionEventParam> m_CurrentEventParam;

   public:
      AAA_SessionEventParam *CurrentEventParam() {
	 return m_CurrentEventParam.get();
      }
      virtual void Notify(AAA_Event event) {
         if (AAA_StateMachineWithTimer<ARG>::Running()) {
             std::auto_ptr<AAA_SessionEventParam> 
                   p(new AAA_SessionEventParam);
             p->m_Event = event;
             m_EventQueue.Enqueue(p);
             Schedule(this);
         }
      }
      virtual void NotifyRx(AAA_Event event,
                            std::auto_ptr<AAAMessage> msg) {
         if (AAA_StateMachineWithTimer<ARG>::Running()) {
             std::auto_ptr<AAA_SessionEventParam> 
                   p(new AAA_SessionEventParam);
             p->m_Event = event;
             p->m_RxMsg = msg;
             m_EventQueue.Enqueue(p);
             Schedule(this);
         }
      }
      virtual void NotifyTx(AAA_Event event, 
                            std::auto_ptr<AAAMessage> msg) {
         if (AAA_StateMachineWithTimer<ARG>::Running()) {
             std::auto_ptr<AAA_SessionEventParam> 
                   p(new AAA_SessionEventParam);
             p->m_Event = event;
             p->m_TxMsg = msg;
             m_EventQueue.Enqueue(p);
             Schedule(this);
         }
      }
   protected:
      AAA_SessionStateMachine(AAA_Task &t,
                              AAA_StateTable<ARG> &table,
                              ARG &arg) :
          AAA_StateMachineWithTimer<ARG>
             (arg, table, *t.reactor()),
          m_GroupedJob(AAA_GroupedJob::Create(t.Job(),
	      (AAA_JobData*)this)) {
      }
      ACE_Mutex m_EventFsmMtx;
      virtual int Serve() {
         AAA_MutexScopeLock guard(m_EventFsmMtx);
         m_CurrentEventParam = m_EventQueue.Dequeue();
         m_Debug.DumpEvent(AAA_StateMachineWithTimer<ARG>::state, 
                           m_CurrentEventParam->m_Event);
         try {
             AAA_StateMachineWithTimer<ARG>::Event
                       (m_CurrentEventParam->m_Event);
         }
         catch (AAA_BaseException &err) {
             AAA_LOG(LM_ERROR, "(%P|%t) FSM error[%d]: %s\n",
                        err.Code(), err.Description().data());
         }
         catch (...) {
             AAA_LOG(LM_ERROR, "(%P|%t) Unknown exception in FSM\n");
         }
         m_CurrentEventParam.reset();
         return (0);
      }
      virtual int Schedule(AAA_Job* job, size_t backlogSize = 1) {
         if (! AAA_StateMachineWithTimer<ARG>::Running()) {
            return (-1);
         }
         return m_GroupedJob->Schedule(job, backlogSize);
      }
      virtual void Timeout(AAA_Event ev) {
         Notify(ev);
      }

   private:   
      AAA_JobHandle<AAA_GroupedJob> m_GroupedJob;       
      DEBUG m_Debug;
};

#endif /* __AAA_SESSION_FSM_H__ */

