/**
 * @file    receiver_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_MT_BIDIR_RECEIVER_I_H
#define TAOX11_MT_BIDIR_RECEIVER_I_H

#include "receiverS.h"
#include <mutex>

class Receiver_i final
  : public virtual CORBA::servant_traits<Receiver>::base_type
{
public:
  Receiver_i () = default;

  uint32_t get_event_count ();

  virtual void receive_payload (const ::Receiver::Payload &payload) override;

private:
  std::mutex mutex_;
  uint32_t message_count_ {};
  uint32_t byte_count_ {};
};

#endif /* TAOX11_MUXING_RECEIVER_H */
