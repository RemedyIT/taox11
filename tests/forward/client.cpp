/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"

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

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object ("file://test.ior");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
        return 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;
      }

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

      if (!hello)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object." << std::endl;
        return 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;
      }

      std::vector <IDL::traits<Test::Hello>::ref_type> foo_seq;
      foo_seq.resize(5);

      for (IDL::traits<Test::Hello>::ref_type& el_foo : foo_seq)
      {
        if (!el_foo)
          TAOX11_TEST_DEBUG << "null reference" << std::endl;
      }

      IDL::traits<Test::Bar>::ref_type bar = hello->get_bar ();

      IDL::traits<Test::Bar>::ref_type bar_in;
      IDL::traits<Test::Bar>::ref_type bar_out;
      IDL::traits<Test::Bar>::ref_type bar_inout;
      hello->with_bar (bar_in, bar_out, bar_inout);

      TAOX11_TEST_DEBUG << "shutting down...";
      if (hello)
        hello->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }
  return 0;
}
