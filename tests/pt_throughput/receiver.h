/**
 * @file    receiver.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef THROUGHPUT_RECEIVER_H
#define THROUGHPUT_RECEIVER_H

#include "testS.h"
#include "ace/OS_NS_time.h"

/// Implement the Test::Receiver interface
class Receiver_impl final
  : public virtual CORBA::servant_traits<Test::Receiver>::base_type
{
public:
  /// Constructor
  Receiver_impl ();

  // = The skeleton methods
  void receive_data (const Test::Message &message) override;

  void done () override;

private:
  /// The timestamp for the first message
  ACE_hrtime_t start_time_;

  /// The number of messages received
  size_t message_count_;

  /// The number of bytes received
  size_t byte_count_;

  /// The timestamp for the last message
  ACE_hrtime_t last_message_time_;

  /// The id for the last message
  uint32_t last_message_id_;

  Receiver_impl (const Receiver_impl&) = delete;
  Receiver_impl (Receiver_impl&&) = delete;
  Receiver_impl& operator= (const Receiver_impl&) = delete;
  Receiver_impl& operator= (Receiver_impl&&) = delete;
};

#endif /* THROUGHPUT_RECEIVER_H */
