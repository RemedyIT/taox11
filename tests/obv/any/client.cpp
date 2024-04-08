/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "anyC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: " << std::endl << "-k <ior> " << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int errors = 0;

  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    if (!orb)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
        << std::endl;
      return 1;
    }

    if (!parse_args (argc, argv))
    {
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj =
      orb->string_to_object (ior);

    if (!obj)
    {
      TAOX11_TEST_ERROR << "string_to_object failed." << std::endl;
      return 1;
    }

    // Create and register factories.
    IDL::traits<OBV_AnyTest::VA>::factory_ref_type va_factory =
      CORBA::make_reference< OBV_AnyTest::VA_init > ();

    orb->register_value_factory (va_factory->_obv_repository_id (),
      va_factory);

    IDL::traits<OBV_AnyTest::VB >::factory_ref_type vb_factory =
      CORBA::make_reference< OBV_AnyTest::VB_init > ();

    orb->register_value_factory (vb_factory->_obv_repository_id (),
      vb_factory);

    // Do local test
    IDL::traits<OBV_AnyTest::VA>::ref_type va1 =
      CORBA::make_reference< IDL::traits<OBV_AnyTest::VA>::obv_type > ();
    IDL::traits<OBV_AnyTest::VA>::ref_type va2 =
      CORBA::make_reference< IDL::traits<OBV_AnyTest::VA>::obv_type > ();

    uint32_t magic = 3145;

    va1->id (magic);
    va2->id (magic);

    CORBA::Any a1, a2;

    // Test both copying and non-copying version of operator<<=
    a1 <<= va1;

    IDL::traits<OBV_AnyTest::VA>::ref_type pva = va2;
    a2 <<= pva;

    IDL::traits<OBV_AnyTest::VA>::ref_type dst;

    if (!(a1 >>= dst) || dst->id () != magic)
    {
      TAOX11_TEST_ERROR << "ERROR - inserting any a1 into a valuetype"
        << std::endl;
      ++errors;
    }

    if (!(a2 >>= dst) || dst->id () != magic)
    {
      TAOX11_TEST_ERROR << "ERROR - inserting any a2 into a valuetype"
        << std::endl;
      ++errors;
    }

    // It should be possible to extract to a base type
    IDL::traits<OBV_AnyTest::VB>::ref_type vb1 =
      CORBA::make_reference< IDL::traits<OBV_AnyTest::VB >::obv_type > ();
    vb1->id (magic);

    a1 <<= vb1;
    IDL::traits<CORBA::ValueBase>::ref_type target;
    if (!(a1 >>= target))
    {
      TAOX11_TEST_ERROR << "ERROR - unable to extract to its base type"
        << std::endl;
      ++errors;
    }
    dst = IDL::traits<OBV_AnyTest::VA>::narrow (target);
    if (dst == 0 || dst->id () != magic)
    {
      TAOX11_TEST_ERROR << "ERROR - unable to narrow CORBA::ValueBase to its original"
        << std::endl;
      ++errors;
    }

    // Now do remote test
    IDL::traits<OBV_AnyTest::Test>::ref_type test =
      IDL::traits<OBV_AnyTest::Test >::narrow (obj);

    if (!test)
    {
      TAOX11_TEST_ERROR << "IDL::traits<OBV_AnyTest::Test>::narrow failed." << std::endl;
      return 1;
    }

    // STEP 1. Retrieving VA in an any
    CORBA::Any result = test->get_something (false);

    if (!(result >>= dst) || dst->id () != magic)
    {
      TAOX11_TEST_ERROR << "ERROR - remote test 1 failed: " << dst->id() << std::endl;
      ++errors;
    }

    // STEP 2.
    IDL::traits<OBV_AnyTest::VB>::ref_type dst_vb;
    result = test->get_something (true);

    if (!(result >>= dst_vb) || dst_vb->id () != magic)
    {
      TAOX11_TEST_ERROR << "ERROR - remote test 2 failed" << std::endl;
      ++errors;
    }

    // STEP 3. A sanity check demonstrating base-type pointer to
    // derived type allowed.
    IDL::traits<OBV_AnyTest::VA>::ref_type dst_va = test->get_vb ();
    if (dst_va->id () != magic)
    {
      TAOX11_TEST_ERROR << "ERROR - remote test 3 failed" << std::endl;
      ++errors;
    }

    // STEP 4. A VA is added as ValueBase to an Any. Extract this here
    result = test->get_base ();
    IDL::traits<CORBA::ValueBase>::ref_type dst_base;
    if (!(result >>= dst_base))
    {
      TAOX11_TEST_ERROR << "ERROR - remote test 4 failed: Unable to extract valuetype"
        << std::endl;
      ++errors;
    }
    else
    {
      dst = IDL::traits<OBV_AnyTest::VA>::narrow (dst_base);
      if (!dst || dst->id () != magic)
      {
        TAOX11_TEST_ERROR << "ERROR - remote test 4 failed: Unable to narrow to the original type"
          << std::endl;
        ++errors;
      }
    }

#if !defined (TAO_HAS_OPTIMIZED_VALUETYPE_MARSHALING)
    // @@ There's still a problem here with the optimized valuetype
    // marshaling and passing values through anys. The problem is
    // that while the Any in fact contains all of the required type
    // information, there is no way to share that with the
    // ValueBase::_tao_unmarshal_pre() which needs the type info in
    // order to select the appropriate value factory.

    // STEP 5. get a VB, but extract to a VA*.
    result = test->get_something (true);

    if (!(result >>= target))
      {
        TAOX11_TEST_ERROR << "ERROR - remote test 5 extraction failed" << std::endl;
        ++errors;
      }
    dst_va = IDL::traits<OBV_AnyTest::VA>::narrow (target);
    if (dst_va == nullptr || dst_va->id () != magic)
      {
        TAOX11_TEST_ERROR << "ERROR - remote test 5 failed" << std::endl;
        ++errors;
      }
#endif /* TAO_HAS_OPTIMIZED_VALUETYPE_MARSHALING */

    test->shutdown ();

    orb->destroy ();

    TAOX11_TEST_DEBUG << "client - test finished: <" << errors << "> issues were found."
      << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR - unexpected Exception caught in client: " << ex
      << std::endl;
    return 1;
  }

  return errors;
}
