/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/ior_table/ior_table.h"
#include "simple_test_i.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

std::string ior;
bool test_not_found = false;
bool test_null_locator = false;
int test_runtime = 10; // sec

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:n:r:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        test_not_found = std::atoi (get_opts.opt_arg ());
        break;
      case 'n':
        test_null_locator = std::atoi (get_opts.opt_arg ());
        break;
      case 'r':
        test_runtime = std::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -t <test_not_found> -n <test_null_locator>" << std::endl;
        return false;
      }
  return true;
}

IDL::traits<IORTable::Locator>::ref_type
test_narrow (IDL::traits<CORBA::Object>::ref_type object)
{
  try
  {
    IDL::traits<IORTable::Locator>::ref_type tmp =
      IDL::traits<IORTable::Locator>::narrow (nullptr);
    if (tmp != nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: No nullptr returned when narrowing a nullptr."
        << std::endl;
    }
  }
  catch (const CORBA::Exception &)
  {
    TAOX11_TEST_ERROR << "ERROR: Caught unexpected CORBA exception while "
      << "narrowing a CORBA::Object to a IORTable::Locator." << std::endl;
  }

  IDL::traits<IORTable::Locator>::ref_type ret;
  try
  {
    ret = IDL::traits<IORTable::Locator>::narrow (object);
    if (ret == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: Failed to narrow a valid CORBA::Object to "
        << "a IORTable::Locator." << std::endl;
    }
  }
  catch (const CORBA::Exception &)
  {
    TAOX11_TEST_ERROR << "ERROR: Caught unexpected CORBA exception whilst "
      << "narrowing a CORBA::Object to a IORTable::Locator." << std::endl;
  }
  return ret;
}

class Task final
  : public virtual IORTable::Locator
{
public:
  Task (IDL::traits<CORBA::ORB>::ref_type orb,
    bool test_not_found);
  ~Task () = default;

  std::string
  locate (const std::string &) override;

private:
  Task (const Task&) = delete;
  Task (Task&&) = delete;
  Task& operator= (const Task&) = delete;
  Task& operator= (Task&&) = delete;

  IDL::traits<CORBA::ORB>::ref_type orb_;
  bool test_not_found_;
};

Task::Task (IDL::traits<CORBA::ORB>::ref_type orb,
  bool test_not_found)
  : orb_ (orb)
  , test_not_found_ (test_not_found)
{
}

std::string
Task::locate (const std::string &id)
{
  if (this->test_not_found_)
    {
      TAOX11_TEST_DEBUG << "Task::locate - Throwing a NotFound exception."
        << std::endl;
      throw IORTable::NotFound ();
    }
  else
    {
      IDL::traits<CORBA::Object>::ref_type fwd = this->orb_->string_to_object (ior);
      const std::string &fwdString = this->orb_->object_to_string (fwd);

      TAOX11_TEST_DEBUG << "Task::locate - Forwarding client <" << id
        << "> to : " << fwdString << std::endl;

      return fwdString;
    }
}

int
main (int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (!poa_object)
        {
          TAOX11_TEST_ERROR
            << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
        root_poa->the_POAManager ();

      if (!poaman)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->the_POAManager () returned null object."
            << std::endl;
          return 1;
        }

      CORBA::servant_traits<Simple::SimpleTest>::ref_type test_i =
        CORBA::make_reference<Simple_Test_i> (orb);

      PortableServer::ObjectId id = root_poa->activate_object (test_i);

      IDL::traits<CORBA::Object>::ref_type test_obj =
        root_poa->id_to_reference (id);

      if (test_obj == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
          return 1;
        }

      IDL::traits<Simple::SimpleTest>::ref_type test =
        IDL::traits<Simple::SimpleTest>::narrow (test_obj);

      if (test == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Simple::SimpleTest>::narrow (test_obj) "
            << "returned null reference." << std::endl;
          return 1;
        }

      ior = orb->object_to_string (test);

      poaman->activate ();

      // Resolve the IORTable and set the locator
      IDL::traits<CORBA::Object>::ref_type ior_table =
        orb->resolve_initial_references("IORTable");

      IDL::traits<IORTable::Table>::ref_type local_ior_table =
        IDL::traits<IORTable::Table>::narrow (ior_table);

      if (local_ior_table == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: IOR table could not be resolved."
          << std::endl;
        return 1;
      }

      if (test_null_locator)
      {
        local_ior_table->set_locator (nullptr);
      }
      else
      {
        // Create locator.
        IDL::traits<IORTable::Locator>::ref_type tmp =
          CORBA::make_reference<Task> (orb, test_not_found);

        IDL::traits<IORTable::Locator>::ref_type task =
          test_narrow (tmp);
        local_ior_table->set_locator (task);
      }

      if (test_not_found || test_null_locator)
        {
          // Be sure to shutdown the ORB since the client
          // won't be able to.
          std::chrono::seconds tv (test_runtime);
          orb->run (tv);
        }
      else
        orb->run ();

      TAOX11_TEST_DEBUG << "Server - event loop finished" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected exception caught : "
        << ex << std::endl;
      return 1;
    }

  return 0;
}
