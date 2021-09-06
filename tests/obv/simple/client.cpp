/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "simpleC.h"
#include "simple_impl.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -k <ior> ";
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
        return 1;
      }


      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      Test::Client::_ref_type client = IDL::traits<Test::Client>::narrow (obj);

      if (!client)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Client>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed Client interface" << std::endl;

      TAOX11_TEST_DEBUG << "registering Event valuetype factory" << std::endl;

      IDL::traits<Test::Event>::factory_ref_type evt_factory = CORBA::make_reference<Event_init_impl> ();

      _orb->register_value_factory (evt_factory->_obv_repository_id (), evt_factory);

      TAOX11_TEST_DEBUG << "calling client->send_string()" << std::endl;

      client->send_string (CORBA::make_reference< Test::StringBox> ("Hello world!"));

      TAOX11_TEST_DEBUG << "client->get_string () returned " << client->get_string ()->_value () << std::endl;

      TAOX11_TEST_DEBUG << "calling client->get_event()" << std::endl;

      IDL::traits<Test::Event>::ref_type evt;
      client->get_event (evt);

      TAOX11_TEST_DEBUG << "ostream test: client->get_event() returned successfully with " << evt << std::endl;

      evt->do_print ("client");

      IDL::traits<Test::Event>::weak_ref_type w_evt =
        evt.weak_reference ();
      IDL::traits<Test::Event>::ref_type evt_w = w_evt.lock ();

      evt_w->do_print ("client_weak");

      if (w_evt.expired ())
      {
         TAOX11_TEST_ERROR << "ERROR: test_handler_weak is expired."<< std::endl;
         return 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "test_handler_weak isn't expired."<< std::endl;
      }

      IDL::traits<Event_impl>::ref_type i_evt = IDL::traits<Event_impl>::narrow (evt);
      if (i_evt)
        i_evt->do_print ("client implementation");
      else
      {
        TAOX11_TEST_ERROR << "ERROR: failed to narrow valuetype implementation."<< std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "shutting down...";

      client->shutdown ();

      TAOX11_TEST_DEBUG << std::endl;

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    }
  return 0;
}
