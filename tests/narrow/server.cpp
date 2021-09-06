/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testS.h"

#include "testlib/taox11_testlog.h"
#include "tao/x11/Policy_ManagerC.h"
#include "tao/x11/Policy_CurrentC.h"
#include "tao/x11/PolicyC.h"
#include "tao/x11/messaging/TAO_ExtC.h"
#include "tao/x11/portable_server/ServantActivatorC.h"
#include "tao/x11/portable_server/ServantLocatorC.h"

#include "tao/x11/portable_server/ThreadPolicyC.h"

class Hello_i final
  : public virtual CORBA::servant_traits<Hello>::base_type
{
public:
  Hello_i () = default;
  ~Hello_i () = default;
private:
  Hello_i (const Hello_i&) = delete;
  Hello_i (Hello_i&&) = delete;
  Hello_i& operator= (const Hello_i&) = delete;
  Hello_i& operator= (Hello_i&&) = delete;
};

template <typename OBJ>
class TestNarrow final
{
public:
  static int16_t test_narrow (const std::string& obj,
    IDL::traits<CORBA::Object>::ref_type hello);
};


template <typename OBJ>
int16_t
TestNarrow<OBJ>::test_narrow (const std::string& obj,
  IDL::traits<CORBA::Object>::ref_type hello)
{
  int16_t errors = 0;
  std::string test = "IDL::traits<"+obj+">::narrow (nullptr)";
  try
    {
      TAOX11_TEST_DEBUG << "Running " << test << std::endl;
      typename IDL::traits<OBJ>::ref_type null_ptr =
        IDL::traits<OBJ>::narrow (nullptr);
      if (null_ptr != nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR whilst running " << test
            << " Narrowing a nullptr didn't return a nullptr"
            << std::endl;
          ++errors;
        }
    }
  catch (const CORBA::Exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR whilst running " << test
        << ". Unexpected exception caught : " << e
        << std::endl;
      ++errors;
    }
  if (hello != nullptr)
    {
      test = "IDL::traits<"+obj+">::narrow (<hello_obj>)";
      try
        {
          TAOX11_TEST_DEBUG << "Running " << test << std::endl;
          typename IDL::traits<OBJ>::ref_type ptr =
            IDL::traits<OBJ>::narrow (hello);
          if (ptr != nullptr)
            {
              TAOX11_TEST_ERROR << "ERROR whilst running " << test
                << ". Return is not null after narrowing a strange object"
                << std::endl;
              ++errors;
            }
        }
      catch (const CORBA::Exception& e)
        {
          TAOX11_TEST_ERROR << "ERROR whilst running " << test
            << ". Unexpected exception caught : " << e
            << std::endl;
          ++errors;
        }
    }
  return errors;
}

int
main (int argc, ACE_TCHAR *argv[])
{
  int16_t errors = 0;
  std::string test ("");
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (orb == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type poa_obj =
        orb->resolve_initial_references ("RootPOA");

      if (!poa_obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      CORBA::servant_traits<Hello>::ref_type hello_impl =
        CORBA::make_reference<Hello_i> ();

      PortableServer::ObjectId id = root_poa->activate_object (hello_impl);

      IDL::traits<CORBA::Object>::ref_type hello_obj =
        root_poa->id_to_reference (id);

      if (hello_obj == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
          return 1;
        }

      IDL::traits<Hello>::ref_type hello =
        IDL::traits<Hello>::narrow (hello_obj);

      if (hello == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Test::Hello>::narrow (hello_obj) returned null reference."
            << std::endl;
          return 1;
        }

      errors +=
        TestNarrow<PortableServer::POA>::test_narrow (
          "PortableServer::POA", hello_obj);
      errors +=
        TestNarrow<Hello>::test_narrow (
          "Hello", poa_obj); // Pass an unrelated object, ie NOT Hello.
      errors +=
        TestNarrow<CORBA::PolicyCurrent>::test_narrow (
          "CORBA::PolicyCurrent", hello_obj);
      errors +=
        TestNarrow<CORBA::PolicyManager>::test_narrow (
          "CORBA::PolicyManager", hello_obj);
      errors +=
        TestNarrow<PortableServer::POA>::test_narrow (
          "PortableServer::POA", hello_obj);
      errors +=
        TestNarrow<PortableServer::POAManager>::test_narrow (
          "PortableServer::POAManager", hello_obj);
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      errors +=
        TestNarrow<PortableServer::POAManagerFactory>::test_narrow (
          "PortableServer::POAManagerFactory", hello_obj);
#endif
      errors +=
        TestNarrow<PortableServer::Current>::test_narrow (
          "PortableServer::Current", hello_obj);
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      errors +=
        TestNarrow<PortableServer::AdapterActivator>::test_narrow (
          "PortableServer::AdapterActivator", hello_obj);
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      errors +=
        TestNarrow<PortableServer::ServantManager>::test_narrow (
          "PortableServer::ServantManager", hello_obj);
      errors +=
        TestNarrow<PortableServer::ServantActivator>::test_narrow (
          "PortableServer::ServantActivator", hello_obj);
      errors +=
        TestNarrow<PortableServer::ServantLocator>::test_narrow (
          "PortableServer::ServantLocator", hello_obj);
#endif
      errors +=
        TestNarrow<CORBA::Policy>::test_narrow (
          "CORBA::Policy", hello_obj);

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      // Narrow a Policy-related type to a CORBA::Policy.
      IDL::traits<PortableServer::ThreadPolicy>::ref_type tp_pol =
        root_poa->create_thread_policy  (
          PortableServer::ThreadPolicyValue::ORB_CTRL_MODEL);
      try
        {
          IDL::traits<CORBA::Policy>::ref_type pol =
            IDL::traits<CORBA::Policy>::narrow (tp_pol);
          if (pol == nullptr)
            {
              TAOX11_TEST_ERROR << "ERROR: Unable to narrow a ThreadPolicy to a "
                << "CORBA::Policy." << std::endl;
              ++errors;
            }
        }
      catch (const CORBA::Exception&)
        {
          TAOX11_TEST_ERROR << "ERROR: Unexpected exception caught whilst narrowing "
            << "a ThreadPolicy to a Policy" << std::endl;
          ++errors;
        }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      errors +=
        TestNarrow<PortableServer::ThreadPolicy>::test_narrow (
          "PortableServer::ThreadPolicy", hello_obj);
#endif
#if ! defined (CORBA_E_MICRO)
      errors +=
        TestNarrow<PortableServer::LifespanPolicy>::test_narrow (
          "PortableServer::LifespanPolicy", hello_obj);
      errors +=
        TestNarrow<PortableServer::IdUniquenessPolicy>::test_narrow (
          "PortableServer::IdUniquenessPolicy", hello_obj);
      errors +=
        TestNarrow<PortableServer::IdAssignmentPolicy>::test_narrow (
          "PortableServer::IdAssignmentPolicy", hello_obj);
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      errors +=
        TestNarrow<PortableServer::ImplicitActivationPolicy>::test_narrow (
          "PortableServer::ImplicitActivationPolicy", hello_obj);
      errors +=
        TestNarrow<PortableServer::ServantRetentionPolicy>::test_narrow (
          "PortableServer::ServantRetentionPolicy", hello_obj);
      errors +=
        TestNarrow<PortableServer::RequestProcessingPolicy>::test_narrow (
          "PortableServer::RequestProcessingPolicy", hello_obj);
#endif

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected exception caught: " << e << std::endl;
      return 1;
    }

  if (errors == 0)
    TAOX11_TEST_DEBUG << "OK: No errors found in test." << std::endl;
  else
    TAOX11_TEST_ERROR << "ERROR: <" << errors << "> error(s) found in tests."
      << std::endl;

  return errors;
}
