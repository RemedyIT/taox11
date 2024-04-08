/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");

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
        TAOX11_TEST_ERROR << "usage: -k <ior>"
              << std::endl;
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
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      fixed_type f;
      TAOX11_TEST_INFO << "Invoking std::to_string(f) : " << std::to_string (f) << std::endl;
      TAOX11_TEST_INFO << "Invoking f.to_string(Fixed) : " << f.to_string () << std::endl;
      TAOX11_TEST_INFO << "Streaming f : " << f << std::endl;

      if (f)
        {
          TAOX11_TEST_INFO << "f resolves as true" << std::endl;
        }
      // Checks the digits and scale traits
      if (IDL::traits<::V::pi_type>::digits () == 7)
        {
          TAOX11_TEST_INFO << "main - OK, correct number of digits found for "
            << "::V::pi_type" << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "main - ERROR, incorrect number of digits found for "
            "::V::pi_type. expected<7> - found <"
            << IDL::traits<::V::pi_type>::digits ()
            << ">" << std::endl;
        }
      if (IDL::traits<::V::pi_type>::scale () == 6)
        {
          TAOX11_TEST_INFO << "main - OK, correct scale found for "
            << "::V::pi_type" << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "main - ERROR, incorrect scale found for "
            "::V::pi_type. expected<6> - found <"
            << IDL::traits<::V::pi_type>::scale ()
            << ">" << std::endl;
        }

      if (IDL::traits<::pi_type>::digits () == 7)
        {
          TAOX11_TEST_INFO << "main - OK, correct number of digits found for "
            << "::pi_type" << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "main - ERROR, incorrect number of digits found for "
            "::pi_type. expected<7> - found <"
            << IDL::traits<::pi_type>::digits ()
            << ">" << std::endl;
        }
      if (IDL::traits<::pi_type>::scale () == 6)
        {
          TAOX11_TEST_INFO << "main - OK, correct scale found for "
            << "::pi_type" << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "main - ERROR, incorrect scale found for "
            "::pi_type. expected<7> - found <"
            << IDL::traits<::pi_type>::scale ()
            << ">" << std::endl;
        }
      pi_type p1 ("1234");
      TAOX11_TEST_INFO << "Printing p1 : " << p1 << std::endl;

      pi_type p2 = p1;
      TAOX11_TEST_INFO << "Printing p2 : " << p2 << std::endl;

      TAOX11_TEST_INFO << "Printing ::V::F::large : " << ::V::F::large << std::endl;
      TAOX11_TEST_INFO << "Printing ::V::F::pi : " << ::V::F::pi << std::endl;

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }
  return 0;
}
