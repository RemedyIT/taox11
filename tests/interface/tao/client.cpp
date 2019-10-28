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

template<class T>
void TestEqual(T exp, T a)
{
  if (a != exp)
  {
    std::cout << "ERROR: value expected " << exp << " , received " << a
        << std::endl;
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

    Test::iFactory_var factory = Test::iFactory::_narrow (tmp.in());

    if (CORBA::is_nil(factory.in()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG, "Nil Test::iFactory reference <%s>\n", ior), 1);
    }

    std::cout << "narrowed iFactory interface" << std::endl;

    // Test
    {
      Test::iA_var ia = factory->get_iA ();

      if (CORBA::is_nil (ia))
      {
        std::cerr << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }

      ia->do_something_A ();


      std::cout << "going to retrieve iB" << std::endl;

      Test::iA_var ia_b33 = factory->get_iB ();
      ACE_UNUSED_ARG (ia_b33);
      Test::iB_var ia_b = factory->get_iB ();
      if (CORBA::is_nil (ia_b))
      {
        std::cerr << "ERROR: Retrieve of iB returned null object." << std::endl;
        return 1;
      }
      std::cout << "going to call do_something_B" << std::endl;
      ia_b->do_something_B ();

      std::cout << "going to call do_something_A on B reference" << std::endl;
      ia_b->do_something_A ();

      std::cout << "going to retrieve iE" << std::endl;

      Test::iE_var ie = factory->get_iE ();
      if (CORBA::is_nil (ie))
      {
        std::cerr << "ERROR: Retrieve of iE returned null object." << std::endl;
        return 1;
      }

      std::cout << "going to call do_something_E on E reference" << std::endl;
      ie->do_something_E ();
      std::cout << "going to call do_something_D on E reference" << std::endl;
      ie->do_something_D ();
      std::cout << "going to call do_something_C on E reference" << std::endl;
      ie->do_something_C ();
      std::cout << "going to call do_something_B on E reference" << std::endl;
      ie->do_something_B ();
      std::cout << "going to call do_something_A on E reference" << std::endl;
      ie->do_something_A ();

      Test::iF_var i_f = factory->get_iF ();
      if (CORBA::is_nil (i_f))
      {
        std::cerr << "ERROR: Retrieve of iF returned null object." << std::endl;
        return 1;
      }

      std::cout << "going to call do_something_F on F reference" << std::endl;
      i_f->do_something_F ();
      std::cout << "going to call _cxx_doon F reference" << std::endl;
      i_f->_cxx_do();

      Test::iB_var ib = Test::iB::_duplicate (Test::iB::_narrow (ia_b.in ()));
      if (CORBA::is_nil (ib))
      {
        std::cerr << "ERROR: Test::iB::narrow (ia_b) returned null object." << std::endl;
        return 1;
      }
      Test::iB_var ia_b3 = ib;
      Test::iA_var ia_b2 = ib.in ();
      if (CORBA::is_nil (ia_b2))
      {
        std::cerr << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }
      ia_b2 = Test::iA::_nil ();

      ia = factory->get_iA_from_iE ();

      if (CORBA::is_nil (ia))
      {
        std::cerr << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }

      ia->do_something_A ();

      ie = Test::iE::_narrow (ia);

      ie->do_something_E ();

      // Shouldn't compile, not related through inheritance
      //Test::iD id = ia_b2;
      //Test::iC ic = ia_b2;
      Test::iC_var ic2;
      Test::iA_var i333 = ic2.in ();
      Test::iA_var i555 = i333.in ();
    }

    std::cout << "shutting down...";
    factory->shutdown();
    orb->destroy();
    std::cout << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception("Exception caught:");
    return 1;
  }

  return result_;
}
