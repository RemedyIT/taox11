/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "simpleC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

#include "bundle.h"

std::string ior = "file://server.ior";

bool
parse_args (int argc, char *argv[])
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
        TAOX11_TEST_ERROR << "usage: -k <ior>"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int
main(int argc, char* argv[])
{
  try
    {
      // Make some logging visible.
      X11_LOGGER::priority_mask (x11_logger::LP_INFO|x11_logger::LP_WARNING|x11_logger::LP_ALL_ERROR);
      X11_LOGGER::verbosity_mask (x11_logger::V_PRIO|x11_logger::V_CATEGORY|x11_logger::V_TIME);

      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: string_to_object(<ior>) returned null reference."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "retrieved object reference" << std::endl;

      IDL::traits<Simple>::ref_type simple_obj = IDL::traits<Simple>::narrow (obj);

      if (!simple_obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Simple>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed Simple interface" << std::endl;

      IDL::traits<Bundle>::ref_type bundle_i = CORBA::make_reference<BundleImpl> ();
      bundle_i->iInteger (0xDEAD);

      TAOX11_TEST_INFO << "Client: ";
      bundle_i->Display ();
      simple_obj->Display (bundle_i);

      TAOX11_TEST_INFO << "shutting down...";

      simple_obj->shutdown ();

      TAOX11_TEST_INFO << std::endl;

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }
  return 0;
}
