#include "simppl/stub.h"

#include "simppl/dispatcher.h"

#include "simppl/detail/frames.h"
#include "simppl/detail/util.h"

#include <cstring>


namespace simppl
{
   
namespace ipc
{

StubBase::StubBase(const char* iface, const char* role, const char* boundname)
 : iface_(iface) 
 , role_(role)
 , id_(INVALID_SERVER_ID)
 , disp_(0)
 , fd_(-1)
 , current_sessionid_(INVALID_SESSION_ID)
{
   assert(iface);
   assert(role);
   assert(boundname && strlen(boundname) < sizeof(boundname_));
   strcpy(boundname_, boundname);
}


Dispatcher& StubBase::disp()
{
   assert(disp_);
   return *disp_;
}


bool StubBase::dispatcherIsRunning() const
{
   return disp_->isRunning();
}


uint32_t StubBase::sendRequest(detail::Parented& requestor, ClientResponseBase* handler, uint32_t id, const detail::Serializer& s)
{
   assert(disp_);
 
   detail::RequestFrame f(id_, id, current_sessionid_);
   f.payloadsize_ = s.size();
   f.sequence_nr_ = disp_->generateSequenceNr();
   
   if (genericSend(fd(), f, s.data()))
   {
      if (handler)
         disp_->addRequest(requestor, *handler, f.sequence_nr_, fd());
   }
   else
   {
      // no fire-and-forget?
      if (handler)
      {
         errno = EINVAL;
         TransportError* err = new TransportError(errno, f.sequence_nr_);
         
         detail::TransportErrorFrame ef(handler, err);
         ef.sequence_nr_ = f.sequence_nr_;
         
         (void)::write(disp_->selfpipe_[1], &ef, sizeof(ef));
      }
   }
   
   return f.sequence_nr_;
}


bool StubBase::isSignalRegistered(ClientSignalBase& sigbase) const
{
   assert(disp_);
   return disp_->isSignalRegistered(sigbase);
}


void StubBase::sendSignalRegistration(ClientSignalBase& sigbase)
{
   assert(disp_);
   
   detail::RegisterSignalFrame f(id_, sigbase.id(), disp_->generateId());
   f.payloadsize_ = 0;
   f.sequence_nr_ = disp_->generateSequenceNr();
   
   if (disp_->addSignalRegistration(sigbase, f.sequence_nr_))
      genericSend(fd(), f, 0);
}


void StubBase::connect()
{
   assert(disp_);
   disp_->connect(*this);
}


void StubBase::sendSignalUnregistration(ClientSignalBase& sigbase)
{
   assert(disp_);
   
   detail::UnregisterSignalFrame f(disp_->removeSignalRegistration(sigbase));
   
   if (f.registrationid_ != 0)
   {
      f.payloadsize_ = 0;
      f.sequence_nr_ = disp_->generateSequenceNr();
      
      genericSend(fd(), f, 0);
   }
}

}   // namespace ipc

}   // namespace simppl

