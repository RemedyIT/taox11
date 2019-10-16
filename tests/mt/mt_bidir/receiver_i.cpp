/**
 * @file    receiver_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "receiver_i.h"
#include "testlib/taox11_testlog.h"

void
Receiver_i::receive_payload (const ::Receiver::Payload &payload)
{
  std::lock_guard<std::mutex> __guard (this->mutex_);
  ++this->message_count_;
  this->byte_count_ += ACE_Utils::truncate_cast<uint32_t> (payload.size ());

  if (this->message_count_ % 25 == 0)
  {
    TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
          << "Received " << this->message_count_ << " messages" << std::endl;
  }
}

uint32_t
Receiver_i::get_event_count ()
{
  std::lock_guard<std::mutex> __guard (this->mutex_);
  return this->message_count_;
}
