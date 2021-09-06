/**
 * @file    client1.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "hello2C.h"
#include "hello1C.h"


#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int16_t result = 0;

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
          TAOX11_TEST_ERROR <<  "ERROR: client1 - CORBA::ORB_init (argc, argv) returned null ORB."
                       << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: client1 - string_to_object(<ior>) returned null reference."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "client1:retrieved object reference" << std::endl;

      IDL::traits<Test::Hello>::ref_type test_var = IDL::traits<Test::Hello>::narrow (obj);
      if (!test_var)
        {
          TAOX11_TEST_ERROR << "ERROR: client1 - IDL::traits<Test::Hello>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "client1 - narrowed Test::Hello interface" << std::endl;

      Test::A::_ref_type i_a = test_var->get_iA();
      if (!i_a)
      {
        TAOX11_TEST_ERROR << "ERROR: client1 - Retrieve of iA returned null object." << std::endl;
        return 1;
      }

      Test::B::_ref_type i_b = test_var->get_iB();
      if (!i_b)
      {
        TAOX11_TEST_ERROR << "ERROR: client1 - Retrieve of iB returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_INFO << "Client 1: Sending synch messages B::op_B." << std::endl;

      const int32_t res_b = i_b->op_B (8);
      if (res_b != 108)
         {
           TAOX11_TEST_ERROR << "ERROR: client1 - return value op_B  not 108: "
                       << res_b << std::endl;
           result = 1;
         }

      _orb->destroy ();

    }
  catch (const std::exception& e)
      {
        TAOX11_TEST_ERROR << "client1 - exception caught: " << e << std::endl;
        return 1;
      }
  return result;
}
