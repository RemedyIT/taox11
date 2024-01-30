/**
 * @file    poamanager.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/AdapterActivatorC.h"

class MyAdapterActivator: public PortableServer::AdapterActivator
{
public:
  explicit MyAdapterActivator(IDL::traits<PortableServer::POAManager>::ref_type manager) :
      poa_manager_(std::move(manager))
  {
  }

  ~MyAdapterActivator() override = default;

  bool unknown_adapter(
      IDL::traits<PortableServer::POA>::ref_type parent,
      const std::string& name) override
  {
    TAOX11_TEST_DEBUG << "Invocation of unknown_adapter for <" << name << ">"
        << std::endl;
    const CORBA::PolicyList default_policies;
    IDL::traits<PortableServer::POA>::ref_type child = parent->create_POA(name,
        this->poa_manager_, default_policies);
    try
    {
      // shared pointer trap: Don't use 'this' in the_adapter invocation since the
      // ref count will start with 1 again and we need to increase it. Therefor use
      // the adapter_activator of the parent; this will cause the shared pointer to
      // increase the reference count by one.
      IDL::traits<PortableServer::AdapterActivator>::ref_type ta =
          parent->the_activator();
      if (ta != nullptr)
      {
        child->the_activator(ta);
      } else
      {
        TAOX11_TEST_ERROR
            << "ERROR: Unable to retrieve the Adapter Activator of the parent."
            << std::endl;
        child->destroy(true, true);
        return false;
      }
    } catch (const CORBA::Exception&)
    {
      child->destroy(true, true);

      return false;
    }

    return true;

  }

private:
  IDL::traits<PortableServer::POAManager>::ref_type poa_manager_;
};

int main(int argc, char *argv[])
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

    // implicitly widen
    obj = poa_manager;

    // (re-)narrow
    poa_manager =
      IDL::traits<PortableServer::POAManager>::narrow (obj);

    if (!poa_manager)
    {
      TAOX11_TEST_ERROR << "Unable to (re-)narrow PortableServer::POAManager from CORBA::Object reference"
          << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "Activate poa-managerer." << std::endl;
    poa_manager->activate();
    TAOX11_TEST_DEBUG << "Invoke poa-manager methods on activated poa-manager."
        << std::endl;
    std::string id = poa_manager->get_id();
    PortableServer::POAManager::State state = poa_manager->get_state();
    TAOX11_TEST_DEBUG << "State:" << state << std::endl;
    if (state != PortableServer::POAManager::State::ACTIVE)
    {
      TAOX11_TEST_ERROR << "State of activated poa-manager should be ACTIVE."
          << std::endl;
      return 1;
    }

    poa_manager->hold_requests(false);
    poa_manager->discard_requests(false);
    poa_manager->deactivate(false, false);

    TAOX11_TEST_DEBUG << "Invoke methods on deactivated poa-manager." << std::endl;
    std::string id2 = poa_manager->get_id();
    state = poa_manager->get_state();
    TAOX11_TEST_DEBUG << "State:" << state << std::endl;

    if (state != PortableServer::POAManager::State::INACTIVE)
    {
      TAOX11_TEST_ERROR << "State of deactivated poa-manager should be INACTIVE."
          << std::endl;
      return 1;
    }

    try
    {
      poa_manager->hold_requests(false);
      TAOX11_TEST_ERROR
          << "Invoke hold_requests on deactivated poa-manager should throw exception."
          << std::endl;
      return 1;
    } catch (const PortableServer::POAManager::AdapterInactive&)
    {
    }

    try
    {
      poa_manager->discard_requests(false);
      TAOX11_TEST_ERROR
          << "Invoke discard_requests on deactivated poa-manager should throw exception."
          << std::endl;
      return 1;
    } catch (const PortableServer::POAManager::AdapterInactive&)
    {
    }

    try
    {
      poa_manager->activate();
      TAOX11_TEST_ERROR
          << "Invoke activate on deactivated poa-manager should throw exception."
          << std::endl;
      return 1;
    } catch (const PortableServer::POAManager::AdapterInactive&)
    {
    }

    TAOX11_TEST_DEBUG << "done." << std::endl;
    _orb->destroy();
  } catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << ex << std::endl;
    return 1;
  }

  return 0;
}
