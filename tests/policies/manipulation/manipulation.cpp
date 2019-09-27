/**
 * @file    manipulation.cpp
 * @author  Marcel Smit
 *
 * @brief   Stress test for policies
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testS.h"

#include "tao/x11/orb.h"
#include "tao/x11/messaging/messaging.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/Policy_ManagerC.h"
#include "tao/x11/Policy_CurrentC.h"
#include "tao/x11/Policy_ForwardC.h"
#include "testlib/taox11_testlog.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_time.h"

constexpr uint16_t ITERATIONS = 60;
uint16_t nthreads = 5;

enum class OPERATIONS
{
  ADD_OBJECT_POLICY,
  ADD_CURRENT_POLICY,
  ADD_MANAGER_POLICY,
  SET_OBJECT_POLICY,
  SET_CURRENT_POLICY,
  SET_MANAGER_POLICY,
  SAVE_CURRENT_POLICIES,
  RESTORE_CURRENT_POLICIES,
  SAVE_MANAGER_POLICIES,
  RESTORE_MANAGER_POLICIES,
  LAST_OPERATION
};

/// Using this to iterate over the OPERATIONS enum
OPERATIONS operator++(OPERATIONS& x) { return x = static_cast<OPERATIONS>(static_cast<uint16_t>(x) + 1); }
OPERATIONS operator*(OPERATIONS c) {return c;}
OPERATIONS begin(OPERATIONS ) {return OPERATIONS::ADD_OBJECT_POLICY;}
OPERATIONS end(OPERATIONS )   {return OPERATIONS::LAST_OPERATION;}

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        nthreads = std::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        TAOX11_TEST_DEBUG << "usage: -n <nthreads>" << std::endl;
        return false;
      }
  return true;
}

class Test_impl final
  : public virtual CORBA::servant_traits<Test>::base_type
{
public:
  Test_impl () = default;
  virtual ~Test_impl () = default;
private:
  Test_impl (const Test_impl&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (const Test_impl&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;
};

class Manipulation final
  : public ACE_Task_Base
{
public:
  Manipulation (IDL::traits<CORBA::ORB>::ref_type orb,
                IDL::traits<Test>::ref_type test,
                uint16_t niterations);

  virtual int svc ();

private:
  void perform_iteration (OPERATIONS operation,
                          CORBA::PolicyList &policies,
                          IDL::traits<CORBA::PolicyManager>::ref_type policy_manager,
                          IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current);

public:
  Manipulation (const Manipulation&) = delete;
  Manipulation (Manipulation&&) = delete;
  Manipulation& operator= (const Manipulation&) = delete;
  Manipulation& operator= (Manipulation&&) = delete;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<Test>::ref_type test_;
  uint16_t const niterations_;
};

Manipulation::Manipulation (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<Test>::ref_type test,
  const uint16_t niterations)
  :  orb_ (std::move(orb)),
     test_ (std::move(test)),
     niterations_ (niterations)
{
}

void
Manipulation::perform_iteration (OPERATIONS operation,
                                 CORBA::PolicyList  &policies,
                                 IDL::traits<CORBA::PolicyManager>::ref_type policy_manager,
                                 IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current)
{
  uint16_t type = 0;
  try
    {
      switch (operation)
        {
        case OPERATIONS::ADD_OBJECT_POLICY:
        case OPERATIONS::ADD_CURRENT_POLICY:
        case OPERATIONS::ADD_MANAGER_POLICY:
        case OPERATIONS::SET_OBJECT_POLICY:
        case OPERATIONS::SET_CURRENT_POLICY:
        case OPERATIONS::SET_MANAGER_POLICY:
          {
            for (type = 0; type < this->niterations_; ++type)
              {
                CORBA::Any any;
                uint32_t policy_type = 0;

                if (type <= 20)
                  {
                    TimeBase::TimeT value = 0;
                    any <<= value;

                    policy_type = Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE;
                  }
                else if (type > 21 && type < 41)
                  {
                    /// Since this is a TAO-specific extension, it's available
                    /// in the TAO namespace.
                    TAO::BufferingConstraint value;
                    any <<= value;

                    policy_type = TAO::BUFFERING_CONSTRAINT_POLICY_TYPE;
                  }
                else
                  {
                    Messaging::SyncScope value = Messaging::SYNC_NONE;
                    any <<= value;

                    policy_type = Messaging::SYNC_SCOPE_POLICY_TYPE;
                  }

                CORBA::PolicyList policy_list (1);
                policy_list[0] = this->orb_->create_policy (policy_type,  any);

                CORBA::SetOverrideType override_type = CORBA::SetOverrideType::SET_OVERRIDE;
                if (operation == OPERATIONS::ADD_OBJECT_POLICY  ||
                    operation == OPERATIONS::ADD_CURRENT_POLICY ||
                    operation == OPERATIONS::ADD_MANAGER_POLICY)
                  {
                    override_type = CORBA::SetOverrideType::ADD_OVERRIDE;
                  }

                if (operation == OPERATIONS::ADD_OBJECT_POLICY ||
                    operation == OPERATIONS::SET_OBJECT_POLICY)
                  {
                    IDL::traits<CORBA::Object>::ref_type tmp =
                      this->test_->_set_policy_overrides (policy_list, override_type);
                  }
                else if (operation == OPERATIONS::ADD_CURRENT_POLICY ||
                         operation == OPERATIONS::SET_CURRENT_POLICY)
                  {
                    policy_current->set_policy_overrides (policy_list, override_type);
                  }
                else
                  {
                    policy_manager->set_policy_overrides (policy_list, override_type);
                  }
                policy_list[0]->destroy ();
              }
          }
          break;
        case OPERATIONS::SAVE_CURRENT_POLICIES:
          {
            CORBA::PolicyTypeSeq types;
            policies = policy_current->get_policy_overrides (types);
          }
          break;
        case OPERATIONS::SAVE_MANAGER_POLICIES:
          {
            CORBA::PolicyTypeSeq types;
            policies = policy_manager->get_policy_overrides (types);
          }
          break;
        case OPERATIONS::RESTORE_CURRENT_POLICIES:
          {
            if (policies.size () != 0)
              {
                policy_current->set_policy_overrides (policies,
                  CORBA::SetOverrideType::SET_OVERRIDE);
              }
          }
          break;
        case OPERATIONS::RESTORE_MANAGER_POLICIES:
          {
            if (policies.size () != 0)
              {
                policy_manager->set_policy_overrides (policies,
                  CORBA::SetOverrideType::SET_OVERRIDE);
              }
          }
          break;
        default:
          TAOX11_TEST_ERROR << "ERROR: unknown operation" << std::endl;
          break;
        }
    }
  catch (const CORBA::Exception &)
    {
      // Ignore all exceptions
    }
}


int
Manipulation::svc ()
{
  TAOX11_TEST_DEBUG << "Thread " << x11_logger::threadid << " started" << std::endl;

  try
    {
      IDL::traits<CORBA::Object>::ref_type object =
        this->orb_->resolve_initial_references ("ORBPolicyManager");

      IDL::traits<CORBA::PolicyManager>::ref_type policy_manager =
        IDL::traits<CORBA::PolicyManager>::narrow (object);

      /// Explicitly check the outcome of the narrow
      if (!policy_manager)
        {
          TAOX11_TEST_ERROR << "ERROR : Unable to narrow CORBA::Object to a "
            << "PolicyManager reference" << std::endl;
          return 1;
        }

      // implicitly widen
      object = policy_manager;

      // (re-)narrow
      policy_manager =
        IDL::traits<CORBA::PolicyManager>::narrow (object);

      /// Explicitly check the outcome of the narrow
      if (!policy_manager)
        {
          TAOX11_TEST_ERROR << "Unable to (re-)narrow CORBA::Object to PolicyManager reference" << std::endl;
          return 1;
        }

      object = this->orb_->resolve_initial_references ("PolicyCurrent");

      IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current =
        IDL::traits<CORBA::PolicyCurrent>::narrow (object);

      /// Explicitly check the outcome of the narrow
      if (!policy_current)
        {
          TAOX11_TEST_ERROR << "ERROR : Unable to narrow CORBA::Object to a "
            << "PolicyCurrent reference" << std::endl;
          return 1;
        }

      // implicitly widen
      object = policy_current;

      // (re-)narrow
      policy_current =
        IDL::traits<CORBA::PolicyCurrent>::narrow (object);

      /// Explicitly check the outcome of the narrow
      if (!policy_current)
        {
          TAOX11_TEST_ERROR << "Unable to (re-)narrow CORBA::Object to PolicyCurrent reference" << std::endl;
          return 1;
        }

      for (uint16_t i {}; i != this->niterations_; ++i)
        {
          for (OPERATIONS op : OPERATIONS ())
            {
              CORBA::PolicyList policies;
              this->perform_iteration (op,
                                       policies,
                                       policy_manager,
                                       policy_current);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught unexpected CORBA exception: "
        << ex << std::endl;
      return 1;
    }

  TAOX11_TEST_DEBUG << "Thread " << x11_logger::threadid << " ready" << std::endl;

  return 0;
}

int main (int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type poa_object =
        orb->resolve_initial_references ("RootPOA");

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

      if (!root_poa)
        {
          TAOX11_TEST_PANIC << "("
            << x11_logger::processid << "|"
            << x11_logger::threadid << ") Panic: nil RootPOA"
            << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      CORBA::servant_traits<Test>::ref_type test_impl =
        CORBA::make_reference<Test_impl> ();

      PortableServer::ObjectId id =
        root_poa->activate_object (test_impl);

      IDL::traits<CORBA::Object>::ref_type object =
        root_poa->id_to_reference (id);

      IDL::traits<Test>::ref_type test =
        IDL::traits<Test>::narrow (object);

      if (!test)
        {
          TAOX11_TEST_ERROR << "ERROR: Object reference is nil" << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "Activating " << nthreads << " threads" << std::endl;

      Manipulation manipulation (orb, test, ITERATIONS);
      if (manipulation.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
      {
        TAOX11_TEST_ERROR << "ERROR: Cannot activate manipulation threads"
          << std::endl;
        return 1;
      }
      manipulation.thr_mgr ()->wait ();

      TAOX11_TEST_DEBUG << "Threads are finished" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();

      TAOX11_TEST_DEBUG << "Test is ready" << std::endl;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught unexpected CORBA exception: "
        << ex << std::endl;
      return 1;
    }

  return 0;
}
