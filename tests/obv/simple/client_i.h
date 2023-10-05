/**
 * @file    client_i.h
 * @author  Martin Corino
 *
 * @brief   test servant implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef __OBV_CLIENT_I_H_INCLUDED__
#define __OBV_CLIENT_I_H_INCLUDED__

#include "simpleS.h"

class Client_i final
  : public CORBA::servant_traits<Test::Client>::base_type
{
public:
  void send_string (IDL::traits<Test::StringBox>::ref_type sb) override;

  IDL::traits<Test::StringBox>::ref_type get_string () override;

  void get_event (IDL::traits<Test::Event>::ref_type& evt) override;

  void do_nothing (IDL::traits<Test::base_vt>::ref_type) override;

  void shutdown () override;

  using base_type = CORBA::servant_traits<Test::Client>::base_type;

  Client_i (CORBA::ORB::_ref_type orb)
   : base_type (),
     orb_ (std::move(orb))
  {}

  CORBA::ORB::_ref_type orb_;
private:
  Client_i (const Client_i&) = delete;
  Client_i (Client_i&&) = delete;
  Client_i& operator= (const Client_i&) = delete;
  Client_i& operator= (Client_i&&) = delete;
};

#endif
