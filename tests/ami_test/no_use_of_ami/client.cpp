/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "helloC.h"



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
          TAOX11_TEST_ERROR <<  "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                       << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "client:retrieved object reference" << std::endl;

      IDL::traits<Test::B>::ref_type test_var = IDL::traits<Test::B>::narrow (obj);

      if (!test_var)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::B>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed Test::B interface" << std::endl;


      TAOX11_TEST_INFO << "Client: Sending synch message B::op_B." << std::endl;


      const int32_t res_b = test_var->op_B ( 8);

      TAOX11_TEST_INFO << "Client: After sending synch messages B::op_B, res = " << res_b << std::endl;

      test_var->shutdown ();

     _orb->destroy ();
    }
  catch (const std::exception& e)
      {
        TAOX11_TEST_ERROR << "exception caught: " << e.what() << std::endl;
        return 1;
      }
  return result;
}
