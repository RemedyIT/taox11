/**
 * @file    client_i.cpp
 * @author  Martin Corino
 *
 * @brief   test servant implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "client_i.h"
#include "simple_impl.h"

#include "testlib/taox11_testlog.h"

void Client_i::send_string (IDL::traits<Test::StringBox>::ref_type sb)
{
  TAOX11_TEST_DEBUG << "ostream test: Client_i::send_string: received: " << sb <<  std::endl;
  TAOX11_TEST_DEBUG << "Client_i::send_string: received \""
        << sb->_value () << "\"" << std::endl;
}

IDL::traits<Test::StringBox>::ref_type Client_i::get_string ()
{
  return IDL::traits<Test::StringBox>::make_reference ("Back from server.");
}

void Client_i::get_event (IDL::traits<Test::Event>::ref_type& evt)
{
  evt = IDL::traits<Test::Event>::make_reference< Event_impl> (123, 254, "Event#1", 1.66);

  TAOX11_TEST_DEBUG << "Client_i::get_event:" << evt <<  std::endl;

  // Compile test ostringstream insertion
  std::ostringstream ss;
  ss << IDL::traits<Test::Event>::write (evt) << std::endl;

  evt->do_print ("server");
}

void Client_i::do_nothing (IDL::traits<Test::base_vt>::ref_type)
{
  // noop
}

void Client_i::shutdown ()
{
  this->orb_->shutdown (false);
}


