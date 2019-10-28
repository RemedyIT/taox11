/**
 * @file    invalid_name.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA test for validating that we get a correct
 *          InvalidName exception
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "testlib/taox11_testlog.h"

int
main (int argc, char *argv[])
{
  int retval = 0;

  try
    {
      // Initialize the ORB
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      try
        {
          IDL::traits<CORBA::Object>::ref_type obj =
            orb->resolve_initial_references ("a_name_that_we_dont_support");

          TAOX11_TEST_ERROR << "ERROR: Didn't catch correct InvalidName exception";
          ++retval;
        }
      catch (const CORBA::ORB::InvalidName&)
        {
          TAOX11_TEST_DEBUG << "Catched correct InvalidName exception";
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << ex;
      ++retval;
    }

  return retval;
}
