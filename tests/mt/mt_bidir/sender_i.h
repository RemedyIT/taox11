/**
 * @file    sender_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_MT_BIDIR_SENDER_H
#define TAOX11_MT_BIDIR_SENDER_H

#include "senderS.h"
#include <mutex>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Manual_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

class Sender_i
  : public virtual CORBA::servant_traits<Sender>::base_type
{
public:
  Sender_i (uint32_t no_clients,
            ACE_Manual_Event &event);

  virtual ~Sender_i () = default;

  virtual int32_t receiver_object (IDL::traits<Receiver>::ref_type recv) override;

  void send_message ();

private:
  std::mutex mutex_;

  ACE_Manual_Event &event_;

  typedef std::vector<IDL::traits<Receiver>::ref_type > receiver_list;

  receiver_list receivers_;

  uint32_t no_clients_;
  uint32_t last_client_ {};
  uint32_t last_index_ {};

  Receiver::Payload payload_;
};

#endif /* TAOX11_MT_BIDIR_SENDER_H */
