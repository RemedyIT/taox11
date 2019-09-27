/**
 * @file    servantbase.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core servant classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "tao/ORB_Core.h"

#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/servantbase.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/orb.h"
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
# include "tao/x11/ifr_client_adapter.h"
#endif

namespace TAOX11_NAMESPACE {

  namespace PortableServer {

    Servant::_shared_ptr_type Servant::_lock_shared ()
    {
      Servant::_shared_ptr_type svtref = this->self_ref_.lock ();
      if (!svtref)
      {
        svtref = Servant::_shared_ptr_type (this, Servant::dtor ());
        this->self_ref_ = svtref;
      }
      return svtref;
    }

    bool Servant::_is_a (const std::string& local_type_id)
    {
      return local_type_id == "IDL:omg.org/CORBA/Object:1.0"; // default
    }

    IDL::traits<PortableServer::POA>::ref_type Servant::_default_POA ()
    {
      TAO_CORBA::Object_var object = TAO_VERSIONED_NAMESPACE_NAME::TAO_ORB_Core_instance ()->root_poa ();
      TAO_PORTABLE_SERVER::POA_var _def_poa = TAO_PORTABLE_SERVER::POA::_narrow (object.in ());
      if (! TAO_CORBA::is_nil (_def_poa))
      {
        return CORBA::make_reference<PortableServer::POA_impl> (
            new POA_proxy (_def_poa._retn ()));
      }
      return nullptr;
    }

#if (TAO_HAS_MINIMUM_CORBA == 0)
    bool Servant::_non_existent ()
    {
      return false; // default
    }

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    /// Query the Interface Repository for the interface definition.
    CORBA::object_reference<CORBA::InterfaceDef> Servant::_get_interface ()
    {
      IFRClientAdapter *adapter = CORBA::ORB::ifr_client_adapter ();
      if (adapter)
      {
        return adapter->get_interface (this->_interface_repository_id ());
      }
      return {};
    }

    IDL::traits<TAOX11_CORBA::Object>::ref_type Servant::_get_component ()
    {
      return {};
    }
#endif

    /// Get the repository id.
    std::string Servant::_repository_id ()
    {
      return this->_interface_repository_id ();
    }
#endif

  } //namespace CORBA

} // namespace TAOX11_NAMESPACE
