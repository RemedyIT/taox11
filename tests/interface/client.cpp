/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include "stdexcept"

bool
test_nil_invocation ()
{
  int result = false;
  TAOX11_TEST_DEBUG << "Testing invocation on nil reference" << std::endl;

  try
  {
    IDL::traits<Test::iA>::ref_type ia;
    ia->do_something_A ();
  }
  catch (const CORBA::INV_OBJREF& e)
  {
    TAOX11_TEST_DEBUG << "Correct exception when invoking operation on nil reference: " << e << std::endl;
    result = true;
  }

  if (!result)
    TAOX11_TEST_ERROR << "ERROR: No exception when invoking operation on nil object reference" << std::endl;

  return result;
}

int main(int argc, char* argv[])
{
  int result = 0;
  try
    {
      // Explicitly first declare the ORB and assign it in a second step.
      IDL::traits<CORBA::ORB>::ref_type _orb;
      _orb = CORBA::ORB_init (argc, argv);

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

      IDL::traits<CORBA::Object>::ref_type obj2;
      std::swap (obj, obj2);
      std::swap (obj, obj2);
#if defined (ACE_HAS_CPP17)
      if (!std::is_swappable<IDL::traits<CORBA::Object>::ref_type>())
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<CORBA::Object>::ref_type is not swappable." << std::endl;
        return 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "IDL::traits<CORBA::Object>::ref_type is swappable." << std::endl;
      }
#endif /* ACE_HAS_CPP17 */

      if (!test_nil_invocation())
        ++result;

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      Test::iFactory::_ref_type factory = IDL::traits<Test::iFactory>::narrow (obj);

      if (!factory)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::iFactory>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed iFactory interface" << std::endl;

      // Test

      Test::iA::_ref_type ia = factory->get_iA ();

      if (!ia)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }

      ia->do_something_A ();

      TAOX11_TEST_DEBUG << "going to retrieve iB" << std::endl;

      Test::iA::_ref_type ia_b33 = factory->get_iB ();
      Test::iB::_ref_type ia_b = factory->get_iB ();
      if (!ia_b)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iB returned null object." << std::endl;
        return 1;
      }
      TAOX11_TEST_DEBUG << "going to call do_something_B" << std::endl;
      ia_b->do_something_B ();

      TAOX11_TEST_DEBUG << "going to call do_something_A on B reference" << std::endl;
      ia_b->do_something_A ();

      TAOX11_TEST_DEBUG << "going to retrieve iE" << std::endl;

      Test::iE::_ref_type ie = factory->get_iE ();
      if (!ie)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iE returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "going to call do_something_E on E reference" << std::endl;
      ie->do_something_E ();
      TAOX11_TEST_DEBUG << "going to call do_something_D on E reference" << std::endl;
      ie->do_something_D ();
      TAOX11_TEST_DEBUG << "going to call do_something_C on E reference" << std::endl;
      ie->do_something_C ();
      TAOX11_TEST_DEBUG << "going to call do_something_B on E reference" << std::endl;
      ie->do_something_B ();
      TAOX11_TEST_DEBUG << "going to call do_something_A on E reference" << std::endl;
      ie->do_something_A ();

      Test::iF::_ref_type i_f = factory->get_iF ();
      if (!i_f)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iF returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "going to call do_something_F on F reference" << std::endl;
      i_f->do_something_F ();
      TAOX11_TEST_DEBUG << "going to call do on F reference" << std::endl;
      i_f->_cxx_do();



      Test::iB::_ref_type ib = IDL::traits<Test::iB>::narrow (ia_b);
      if (!ib)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::iB>::narrow (ia_b) returned null object." << std::endl;
        return 1;
      }
      Test::iB::_ref_type ia_b3 = ib;
      Test::iA::_ref_type ia_b2 = ib;
      if (!ia_b2)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }
      ia_b2 = nullptr;

      ia = factory->get_iA_from_iE ();

      if (!ia)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }

      ia->do_something_A ();

      ie = IDL::traits<Test::iE>::narrow (ia);

      ie->do_something_E ();

      // Shouldn't compile, not related through inheritance
      //Test::iD id = ia_b2;
      //Test::iC ic = ia_b2;
      Test::iC::_ref_type ic2;
      Test::iA::_ref_type i333 = ic2;
      Test::iA::_ref_type i555 = i333;

      TAOX11_TEST_DEBUG << "shutting down...";
      factory->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR: exception caught: " << e.what () << std::endl;
      ++result;
    }
  return result;
}
