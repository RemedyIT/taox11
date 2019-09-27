/**
 * @file    sender_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "sender_i.h"
#include "ace/Manual_Event.h"

Sender_i::Sender_i (uint32_t no_clients,
                    ACE_Manual_Event &event)
  : event_ (event)
  , receivers_ (0)
  , no_clients_ (no_clients)
  , payload_ (32768)
{
  this->receivers_.resize (no_clients);

  // Okay to have a magic number...
  this->payload_.resize (32768);

  for (uint32_t j = 0; j != this->payload_.size (); ++j)
    this->payload_[j] = (j % 256);
}

int32_t
Sender_i::receiver_object (IDL::traits<Receiver>::ref_type recv)
{
  std::lock_guard<std::mutex> __guard (this->mutex_);

  if (this->no_clients_ == this->last_index_)
    throw ::Sender::Table_Full ();

  this->receivers_[this->last_index_] = recv;

  ++this->last_index_;

  if (this->last_index_ == this->no_clients_)
    {
      this->event_.signal ();
    }

  return this->last_index_;
}


void
Sender_i::send_message ()
{
  uint32_t clnt = ++this->last_client_ - 1;
  if (clnt >= this->no_clients_)
    {
      clnt = this->no_clients_ - 1;
      this->last_client_ = 0;
    }

  this->receivers_[clnt]->receive_payload (this->payload_);
}
