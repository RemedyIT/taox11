/**
 * @file    servant_proxy.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 TAO servant proxy base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/PortableInterceptor.h"
#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/basic_sarguments.h"
#include "tao/x11/portable_server/special_basic_sarguments.h"
#include "tao/x11/basic_arguments.h"
#include "tao/x11/special_basic_arguments.h"
#include "tao/PortableServer/UB_String_SArguments.h"
#include "tao/PortableServer/get_arg.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/portable_server/servant_proxy.h"
#include "tao/x11/portable_server/upcall_command.h"
#include "tao/x11/portable_server/stub_sarg_traits.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/stub_arg_traits.h"
#include "tao/x11/ifr_client_adapter_impl_base.h"
#include "tao/x11/orb.h"

namespace TAOX11_NAMESPACE {

  Servant_proxy::Servant_proxy (TAO_Operation_Table* optable)
  : TAO_VERSIONED_NAMESPACE_NAME::TAO_ServantBase (optable)
  {
  }

  void Servant_proxy::_add_ref ()
  {
    uint32_t const new_count = ++this->ref_count_;

    if (new_count == 2)
    {
      // someone else is referencing the proxy
      // add reference to skeleton to lock it
      this->skel_ref_ = this->_get_skel ()->_lock_shared ();
    }
  }

  void Servant_proxy::_remove_ref ()
  {
    uint32_t const new_count = --this->ref_count_;

    if (new_count == 1)
    {
      // remove reference to skeleton
      // if skeleton reference count reaches 0
      // it will destruct taking us with it
      this->skel_ref_.reset ();
    }
    else if (new_count == 0)
    {
      // skeleton reference removed because it is destructing
      delete this;
    }
  }

  /// Local implementation of the CORBA::Object::_is_a method.
  TAO_CORBA::Boolean Servant_proxy::_is_a (const char *logical_type_id)
  {
    return this->_get_skel ()->_is_a (logical_type_id);
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  /// Default _non_existent: always returns false.
  TAO_CORBA::Boolean Servant_proxy::_non_existent ()
  {
    return this->_get_skel ()->_non_existent ();
  }

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  /// Query the Interface Repository for the interface definition.
  TAO_CORBA::InterfaceDef_ptr Servant_proxy::_get_interface ()
  {
    TAOX11_CORBA::object_reference<TAOX11_CORBA::InterfaceDef> _ifdef =
        this->_get_skel ()->_get_interface ();
    if (_ifdef)
    {
      IFRClientAdapter_ImplBase *adapter = dynamic_cast<IFRClientAdapter_ImplBase *> (CORBA::ORB::ifr_client_adapter ());
      if (adapter)
      {
        return adapter->x11_interface_to_tao (_ifdef);
      }
    }
    return {};
  }

  /// Default _get_component: always returns CORBA::Object::_nil().
  TAO_CORBA::Object_ptr Servant_proxy::_get_component ()
  {
    TAOX11_IDL::traits<TAOX11_CORBA::Object>::ref_type cobj =
        this->_get_skel ()->_get_component ();
    return cobj ? TAO_CORBA::Object::_duplicate (cobj->get_proxy ()->get ()) : TAO_CORBA::Object::_nil ();
  }
#endif

  /// Get the repository id.
  char * Servant_proxy::_repository_id ()
  {
    return TAO_CORBA::string_dup (this->_get_skel ()->_repository_id ().c_str ());
  }
#endif /* TAO_HAS_MINIMUM_CORBA */

  const char* Servant_proxy::_interface_repository_id () const
  {
    return this->_get_skel ()->_interface_repository_id ().c_str ();
  }

  TAO_PORTABLE_SERVER::POA_ptr Servant_proxy::_default_POA ()
  {
    TAOX11_CORBA::object_reference< TAOX11_NAMESPACE::PortableServer::POA_impl> _poa_impl =
      TAOX11_CORBA::object_traits< TAOX11_NAMESPACE::PortableServer::POA_impl>::narrow (this->_get_skel ()->_default_POA ());
    return TAO_PORTABLE_SERVER::POA::_duplicate ((TAO_PORTABLE_SERVER::POA_ptr)_poa_impl->_proxy ());
  }
} // namespace TAOX11_NAMESPACE
