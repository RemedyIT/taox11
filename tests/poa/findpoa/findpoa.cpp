/**
 * @file    findpoa.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/AdapterActivatorC.h"

bool find_non_existant_POA(IDL::traits<PortableServer::POA>::ref_type parent,
    const std::string& child_poa_name, bool activate)
{
  // New environment.
  try
    {
      // Try to find child poa.
      IDL::traits<PortableServer::POA>::ref_type child_poa = parent->find_POA (
            child_poa_name, activate);
    }
  catch (const PortableServer::POA::AdapterNonExistent&)
    {
      TAOX11_TEST_DEBUG << "OK: Caught correct exception" << std::endl;
      return true;
    }

  TAOX11_TEST_ERROR << "ERROR: Caught incorrect exception" << std::endl;
  return false;
}

class MyAdapterActivator: public PortableServer::AdapterActivator
{
public:
  MyAdapterActivator (IDL::traits<PortableServer::POAManager>::ref_type manager) :
    poa_manager_ (manager)
  {
  }

  virtual ~MyAdapterActivator()
  {
  }

  virtual bool unknown_adapter (
    IDL::traits<PortableServer::POA>::ref_type parent,
    const std::string& name) override
  {
    TAOX11_TEST_DEBUG << "Invocation of unknown_adapter for <" << name << ">"
      << std::endl;
    const CORBA::PolicyList default_policies;
    IDL::traits<PortableServer::POA>::ref_type child = parent->create_POA (name,
    this->poa_manager_, default_policies);

    try
      {
        // shared pointer trap: Don't use 'this' in the_adapter invocation since the
        // ref count will start with 1 again and we need to increase it. Therefor use
        // the adapter_activator of the parent; this will cause the shared pointer to
        // increase the reference count by one.
        IDL::traits<PortableServer::AdapterActivator>::ref_type ta = parent->the_activator ();
        if (ta != nullptr)
          {
            child->the_activator (ta);
          }
        else
          {
            TAOX11_TEST_ERROR << "ERROR: Unable to retrieve the Adapter Activator of the parent." << std::endl;
            child->destroy (true, true);
            return false;
          }
      }
    catch (const CORBA::Exception&)
      {
        child->destroy(true, true);
        TAOX11_TEST_ERROR << "ERROR: Unable to retrieve the Adapter Activator of the parent." << std::endl;
        return false;
      }

    return true;
  }

private:
  IDL::traits<PortableServer::POAManager>::ref_type poa_manager_;
};

int main (int argc, char *argv[])
{

  // Initialize the ORB
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);
      IDL::traits<CORBA::Object>::ref_type obj = _orb->resolve_initial_references(
        "RootPOA");

      TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;
      // Narrow Object reference to RootPOA to a POA reference.
      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      TAOX11_TEST_DEBUG << "narrowed root POA interface" << std::endl;
      // Get the POAManager of the RootPOA.
      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager();
      poa_manager->activate();

      // Try to find a non-existant POA.  Since the Adapter Activator
      // has not been installed yet, this call should fail.
      if (!find_non_existant_POA (root_poa, "firstPOA", true) )
        return 1;

      // Get a TAO_Adapter_Activator reference
      TAOX11_TEST_DEBUG << "create poa activator" << std::endl;


      IDL::traits<PortableServer::AdapterActivator>::ref_type activator (
        CORBA::make_reference < MyAdapterActivator > (poa_manager) );
      root_poa->the_activator (activator);

      TAOX11_TEST_DEBUG << "test creation of firstPOA" << std::endl;
      std::string name = "firstPOA";
      CORBA::PolicyList pl;
      IDL::traits<PortableServer::POA>::ref_type created_poa =
        root_poa->create_POA (name, poa_manager, pl);
      if (created_poa == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: created POA <firstPOA> seems nil!" << std::endl;
          return 1;
        }
      else if (created_poa->the_name() != name)
        {
          TAOX11_TEST_ERROR << "ERROR: created POA <firstPOA> got wrong name <"
            << created_poa->the_name() << "!" << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "Now try to find firstPOA on the root POA" << std::endl;
      // Try to find a childPOA of RootPOA named firstPOA
      IDL::traits<PortableServer::POA>::ref_type first_poa = root_poa->find_POA (
            name, true);

      if (first_poa == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: Found POA <firstPOA> seems nil!" << std::endl;
          return 1;
        }
      else if (first_poa->the_name() != name)
        {
          TAOX11_TEST_ERROR << "ERROR: created POA <firstPOA> got wrong name <"
            << first_poa->the_name() << "!" << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "test find and create secondPOA on the root POA"
        << std::endl;
      name = "secondPOA";
      IDL::traits<PortableServer::POA>::ref_type second_poa = root_poa->find_POA (
            name, true);

      if (second_poa == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: Found POA <secondPOA> on root POA seems nil!"
            << std::endl;
          return 1;
        }
      else if (second_poa->the_name() != name)
        {
          TAOX11_TEST_ERROR << "ERROR: created POA <firstPOA> got wrong name <"
            << second_poa->the_name() << "!" << std::endl;
          return 1;
        }

      // Get a TAO_Adapter_Activator reference
      TAOX11_TEST_DEBUG << "create poa activator on first POA" << std::endl;
      IDL::traits<PortableServer::AdapterActivator>::ref_type activator_on_first =
        CORBA::make_reference < MyAdapterActivator > (poa_manager);
      first_poa->the_activator (activator_on_first);

      TAOX11_TEST_DEBUG << "test find and create secondPOA on the first POA"
        << std::endl;
      name = "secondPOA";
      IDL::traits<PortableServer::POA>::ref_type second_poa_on_first =
        first_poa->find_POA (name, true);
      if (second_poa_on_first == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: Found POA <secondPOA> on the first POA seems nil!"
            << std::endl;
          return 1;
        }

      // Try to find a non-existent POA.  Even though the Adapter
      // Activator has been installed, this call should fail because
      // the activate (if not found) flag is 0.
      TAOX11_TEST_DEBUG << "test find non existent POA" << std::endl;
      find_non_existant_POA (root_poa, "thirdPOA", false);

      _orb->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << ex << std::endl;
      return 1;
    }

  return 0;
}
