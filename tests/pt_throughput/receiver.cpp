/**
 * @file    receiver.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "receiver.h"
#include "testlib/taox11_testlog.h"
#include "ace/High_Res_Timer.h"
#include "ace/Basic_Types.h"

Receiver_impl::Receiver_impl() :
  start_time_(0), message_count_(0), byte_count_(0), last_message_time_(0),
      last_message_id_(0)
{
}

void Receiver_impl::receive_data(const Test::Message &the_message)
{
  ACE_hrtime_t now = ACE_OS::gethrtime();
  if (this->message_count_ == 0)
  {
    this->start_time_ = now;
  }
  else
  {
    if (this->last_message_id_ > the_message.message_id())
    {
      TAOX11_TEST_ERROR
          << "ERROR: Receiver::receive_data, message out of sequence "
          << the_message.message_id() << " / " << this->last_message_id_
          << std::endl;
      return;
    }
    this->last_message_id_ = the_message.message_id();
  }
  ++this->message_count_;
  this->byte_count_ += the_message.the_payload().size();
  this->last_message_time_ = now;
}

void Receiver_impl::done()
{
  if (this->message_count_ == 0)
  {
    TAOX11_TEST_ERROR << "ERROR: Receiver::done, no messages received\n"
        << std::endl;
  }
  else
  {
    ACE_High_Res_Timer::global_scale_factor_type gsf = ACE_High_Res_Timer::global_scale_factor();

    ACE_hrtime_t elapsed_time = this->last_message_time_ - this->start_time_;

    // convert to microseconds
#if !defined ACE_LACKS_LONGLONG_T
    ACE_UINT32 usecs = ACE_UINT32(elapsed_time / gsf);
#else  /* ! ACE_LACKS_LONGLONG_T */
    ACE_UINT32 usecs = elapsed_time / gsf;
#endif /* ! ACE_LACKS_LONGLONG_T */

    if (usecs != 0)
    {
      double bytes = (1000000.0 * this->byte_count_) / usecs;
      double kbytes = bytes / 1024;
      double mbytes = kbytes / 1024;
      double mbits = bytes * 8 / 1000000;
      TAOX11_TEST_DEBUG << "Receiver " << bytes << " (bytes/sec), " << kbytes
          << " (Kb/sec) " << "\nReceiver " << mbytes << " (Mb/sec), " << mbits
          << " Mbits " << std::endl;
    }
  }

  try
  {
    IDL::traits<PortableServer::POA>::ref_type poa = this->_default_POA();
    PortableServer::ObjectId id = poa->servant_to_id (this->_lock ());
    poa->deactivate_object(id);
  }
  catch (const CORBA::Exception&)
  {
  }
}
