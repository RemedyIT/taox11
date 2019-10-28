/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// FUZZ: disable check_for_ace_log_categories

#include "testC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'k':
        ior = get_opts.opt_arg();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                "usage:  %s "
                "-k <ior> "
                "\n",
                argv [0]),
            false);
    }
  // Indicates successful parsing of the command line
  return true;
}

int result_ = 0;

void test_equal(const char * exp, const char * a)
{
  if (strcmp(a, exp) != 0)
  {
    std::cout << "ERROR: string expected '" << exp << "' , received '" << a
        << "'" << std::endl;
    ++result_;
  }
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args(argc, argv) == false)
      return 1;

    CORBA::Object_var tmp = orb->string_to_object(ior);

    Test::Hello_var hello = Test::Hello::_narrow (tmp.in());

    if (CORBA::is_nil(hello.in()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
              "Nil Test::Hello reference <%s>\n",
              ior),
          1);
    }
    std::cout << "narrowed interface" << std::endl;

    // Test
    {
      std::cout << "Test interface A." << std::endl;
      Test::iA_var ia = hello->get_iA();
      if (CORBA::is_nil (ia.in ()))
      {
        std::cerr << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }
      CORBA::String_var result = ia->do_something_A();
      test_equal(result.in (), "A.A");

      std::cout << "Test interface B." << std::endl;
      Test::iB_var ib = hello->get_iB();
      result = ib->do_something_A();
      test_equal(result.in (), "B.A");
      result = ib->do_something_B();
      test_equal(result.in (), "B.B");

      std::cout << "Test interface C." << std::endl;
      Test::iC_var ic = hello->get_iC();
      result = ic->do_something_A();
      test_equal(result.in (), "C.A");
      result = ic->do_something_B();
      test_equal(result.in (), "C.B");
      result = ic->do_something_C();
      test_equal(result.in (), "C.C");

      std::cout << "Test interface D." << std::endl;
      Test::iD_var id = hello->get_iD();
      result = id->do_something_D();
      test_equal(result.in (), "D.D");

      std::cout << "Test interface E." << std::endl;
      Test::iE_var ie = hello->get_iE();
      result = ie->do_something_A();
      test_equal(result.in (), "E.A");
      result = ie->do_something_B();
      test_equal(result.in (), "E.B");
      result = ie->do_something_C();
      test_equal(result.in (), "E.C");
      result = ie->do_something_D();
      test_equal(result.in (), "E.D");
      result = ie->do_something_E();
      test_equal(result.in (), "E.E");
    }

    std::cout << "shutting down...";
    hello->shutdown();

    std::cout << std::endl;
    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception("Exception caught:");
    result_++;
  }

  return result_;
}
