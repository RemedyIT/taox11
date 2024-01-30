/**
 * @file    portableserver_impl.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/objproxy.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/portable_server/portableserver_proxies.h"
#include "tao/x11/portable_server/servant_proxy.h"
#include "tao/x11/portable_server/servantbase.h"

#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/poa_policies_impl.h"
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
# include "tao/x11/portable_server/POAManagerFactoryC.h"
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
# include "tao/x11/portable_server/adapter_activator_cb.h"
# include "tao/x11/portable_server/servant_manager_cb.h"
# include "tao/x11/portable_server/servant_activator_cb.h"
# include "tao/x11/portable_server/servant_locator_cb.h"
# include "tao/x11/portable_server/AdapterActivatorC.h"
# include "tao/x11/portable_server/ServantManagerC.h"
# include "tao/x11/portable_server/ServantActivatorC.h"
# include "tao/x11/portable_server/ServantLocatorC.h"
#endif

#include "tao/x11/log.h"

#include <thread>

namespace TAOX11_NAMESPACE {

  namespace CORBA
  {
    template<>
    object_traits<PortableServer::POA>::ref_type
    object_traits<PortableServer::POA>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
        try
        {
          TAO_PORTABLE_SERVER::POA_var _pp =
            TAO_PORTABLE_SERVER::POA::_narrow (obj->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (_pp))
          {
            return IDL::traits<PortableServer::POA>::ref_type (new PortableServer::POA_impl (
              new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(_pp.in ()))));
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

    template<>
    object_traits<PortableServer::POAManager>::ref_type
    object_traits<PortableServer::POAManager>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
        try
        {
          TAO_PORTABLE_SERVER::POAManager_var _pp =
            TAO_PORTABLE_SERVER::POAManager::_narrow (obj->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (_pp))
          {
            return IDL::traits<PortableServer::POAManager>::ref_type (
              new PortableServer::POAManager_impl (
                new POAManager_proxy (
                  TAO_PORTABLE_SERVER::POAManager::_duplicate(_pp.in ()))));
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    template<>
    object_traits<PortableServer::POAManagerFactory>::ref_type
    object_traits<PortableServer::POAManagerFactory>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
        try
        {
          TAO_PORTABLE_SERVER::POAManagerFactory_var _pp =
              TAO_PORTABLE_SERVER::POAManagerFactory::_narrow (obj->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (_pp))
          {
            return IDL::traits<PortableServer::POAManagerFactory>::ref_type (
              new PortableServer::POAManagerFactory_impl (
                new POAManagerFactory_proxy (
                  TAO_PORTABLE_SERVER::POAManagerFactory::_duplicate(_pp.in ()))));
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }
#endif

    template<>
    object_traits<PortableServer::Current>::ref_type
    object_traits<PortableServer::Current>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
        try
        {
          TAO_PORTABLE_SERVER::Current_var _pp =
            TAO_PORTABLE_SERVER::Current::_narrow (obj->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (_pp))
          {
            return IDL::traits<PortableServer::Current>::ref_type (
              new PortableServer::POA_Current_impl (
                new POA_Current_proxy (
                  TAO_PORTABLE_SERVER::Current::_duplicate(_pp.in ()))));
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    template<>
    object_traits<PortableServer::AdapterActivator>::ref_type
    object_traits<PortableServer::AdapterActivator>::narrow (
       object_traits<CORBA::Object>::ref_type obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }

        TAO_PORTABLE_SERVER::AdapterActivator_var _aa =
          TAO_PORTABLE_SERVER::AdapterActivator::_narrow (obj->get_proxy ()->get ());
        if (!TAO_CORBA::is_nil (_aa.in ()))
        {
          return
            dynamic_cast<PortableServer::Adapter_Activator_Callback*> (_aa.in ())->get_impl ();
        }
      }
      return nullptr;
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    template<>
    object_traits<PortableServer::ServantManager>::ref_type
    object_traits<PortableServer::ServantManager>::narrow (
       object_traits<CORBA::Object>::ref_type obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }

        TAO_PORTABLE_SERVER::ServantManager_var _sa =
          TAO_PORTABLE_SERVER::ServantManager::_narrow (obj->get_proxy ()->get ());
        if (!TAO_CORBA::is_nil (_sa.in ()))
        {
          return
            dynamic_cast< ::TAOX11_NAMESPACE::PortableServer::Servant_Manager_Callback*> (_sa.in ())->get_impl ();
        }
      }
      return nullptr;
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    template<>
    object_traits<PortableServer::ServantActivator>::ref_type
    object_traits<PortableServer::ServantActivator>::narrow (object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }

        TAO_PORTABLE_SERVER::ServantActivator_var _sa =
          TAO_PORTABLE_SERVER::ServantActivator::_narrow (obj->get_proxy ()->get ());
        if (!TAO_CORBA::is_nil (_sa.in ()))
        {
          return
            dynamic_cast< ::TAOX11_NAMESPACE::PortableServer::Servant_Activator_Callback*> (_sa.in ())->get_impl ();
        }
      }
      return nullptr;
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    template<>
    object_traits<PortableServer::ServantLocator>::ref_type
    object_traits<PortableServer::ServantLocator>::narrow (
        object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }

        TAO_PORTABLE_SERVER::ServantLocator_var _sl =
          TAO_PORTABLE_SERVER::ServantLocator::_narrow (obj->get_proxy ()->get ());
        if (!TAO_CORBA::is_nil (_sl.in ()))
        {
          return
              dynamic_cast< ::TAOX11_NAMESPACE::PortableServer::Servant_Locator_Callback*> (_sl.in ())->get_impl ();
        }
      }
      return nullptr;
    }
#endif
  } // namespace CORBA

  namespace PortableServer
  {
    /*
     *  POA_impl implementation
     */

    POA_impl::POA_impl (POA_proxy_ptr op)
      : POA (),
        proxy_ (op)
    {
    }

    POA_proxy& POA_impl::_proxy ()
    {
      return *this->proxy_;
    }

    void
    POA_impl::destroy (bool etherealize_objects,
                       bool wait_for_completion)
    {
      this->_proxy ()->destroy (etherealize_objects, wait_for_completion);
    }

    PortableServer::ObjectId
    POA_impl::activate_object (CORBA::servant_reference<PortableServer::Servant> servant)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId_var oid =
            this->_proxy ()->activate_object (servant->get_proxy ());
        return seq_to_x11<PortableServer::ObjectId> (oid.in ());
      }
      catch (const TAO_PORTABLE_SERVER::POA::ServantAlreadyActive&) {
        throw POA::ServantAlreadyActive ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
      return PortableServer::ObjectId ();
    }

    void
    POA_impl::activate_object_with_id (const PortableServer::ObjectId & oid,
                                       CORBA::servant_reference<PortableServer::Servant> servant)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId _oid;
        seq_from_x11 (_oid, oid);
        this->_proxy ()->activate_object_with_id (_oid,
            servant->get_proxy ());
      }
      catch (const TAO_PORTABLE_SERVER::POA::ObjectAlreadyActive&) {
        throw POA::ObjectAlreadyActive ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::ServantAlreadyActive&) {
        throw POA::ServantAlreadyActive ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
    }

    void
    POA_impl::deactivate_object (const PortableServer::ObjectId & oid)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId _oid;
        seq_from_x11 (_oid, oid);
        this->_proxy ()->deactivate_object (_oid);
      }
      catch (const TAO_PORTABLE_SERVER::POA::ObjectNotActive&) {
        throw POA::ObjectNotActive ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
    }

    IDL::traits<CORBA::Object>::ref_type
    POA_impl::id_to_reference (const PortableServer::ObjectId & oid)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId _oid;
        seq_from_x11 (_oid, oid);
        TAO_CORBA::Object_var tmp = this->_proxy ()->id_to_reference (_oid);
        if (!TAO_CORBA::is_nil (tmp))
        {
          return TAOX11_NAMESPACE::CORBA::Object::_narrow (TAOX11_NAMESPACE::Object_proxy (TAO_CORBA::Object::_duplicate (tmp)));
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::ObjectNotActive&) {
        throw POA::ObjectNotActive ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    IDL::traits<POAManager>::ref_type
    POA_impl::the_POAManager ()
    {
      TAO_PORTABLE_SERVER::POAManager_var poaman = this->_proxy ()->the_POAManager ();
      if (!TAO_CORBA::is_nil (poaman))
      {
        return CORBA::make_reference<POAManager_impl> (
                new POAManager_proxy (TAO_PORTABLE_SERVER::POAManager::_duplicate(poaman.in ())));
      }
      return nullptr;
    }

#if ! defined (CORBA_E_MICRO)
    IDL::traits<POA>::ref_type
    POA_impl::create_POA (const std::string& adapter_name,
                          IDL::traits<PortableServer::POAManager>::ref_type a_POAManager,
                          const TAOX11_NAMESPACE::CORBA::PolicyList& policies)
    {
      CORBA::object_reference<PortableServer::POAManager_impl> poaman_impl =
          CORBA::object_traits<PortableServer::POAManager_impl>::narrow (a_POAManager);

      TAO_CORBA::PolicyList _pols;
      _pols.length (TAO_CORBA::ULong (policies.size ()));
      TAO_CORBA::ULong n (0);

      TAOX11_LOG_DEBUG ( "Copying X11 policies to TAO policies");
      for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
      {
        _pols[n++] =
            TAO_CORBA::Policy::_narrow (_pol->get_proxy ()->get ());
      }
      try {
        TAOX11_LOG_DEBUG ( "Creating TAO POA");
        TAO_PORTABLE_SERVER::POA_var poa =
            this->_proxy ()->create_POA (adapter_name.c_str (),
                                         (poaman_impl ?
                                             poaman_impl->_proxy ().get () :
                                             TAO_PORTABLE_SERVER::POAManager::_nil ()),
                                         _pols);
        if (!TAO_CORBA::is_nil (poa.in ()))
        {
          return CORBA::make_reference<POA_impl> (
                  new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(poa.in ())));
        }
        TAOX11_LOG_ERROR ("ERROR: POA seems nil");
      }
      catch (const TAO_PORTABLE_SERVER::POA::AdapterAlreadyExists&) {
        throw POA::AdapterAlreadyExists ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::InvalidPolicy& _x) {
        throw POA::InvalidPolicy (_x.index);
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    IDL::traits<POA>::ref_type
    POA_impl::find_POA (const std::string& adapter_name,
                        bool activate_it)
    {
      TAO_PORTABLE_SERVER::POA_var poa;
      try
        {
          poa = this->_proxy ()->find_POA (adapter_name.c_str (),
                                           activate_it);
        }
      catch (const TAO_PORTABLE_SERVER::POA::AdapterNonExistent&) {
        throw POA::AdapterNonExistent ();
      }
      catch_tao_system_ex(_ex)

      if (!TAO_CORBA::is_nil (poa.in ()))
      {
        return CORBA::make_reference<POA_impl> (
                new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(poa.in ())));
      }
      return nullptr;
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    IDL::traits<PortableServer::ThreadPolicy>::ref_type
    POA_impl::create_thread_policy (TAOX11_NAMESPACE::PortableServer::ThreadPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::ThreadPolicy_var _pol =
            this->_proxy ()->create_thread_policy (static_cast <TAO_PORTABLE_SERVER::ThreadPolicyValue> (value));
        return CORBA::make_reference<PortableServer::ThreadPolicy_impl> (
            TAO_PORTABLE_SERVER::ThreadPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }
#endif

#if  ! defined (CORBA_E_MICRO)
    IDL::traits<PortableServer::LifespanPolicy>::ref_type
    POA_impl::create_lifespan_policy (TAOX11_NAMESPACE::PortableServer::LifespanPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::LifespanPolicy_var _pol =
            this->_proxy ()->create_lifespan_policy (static_cast <TAO_PORTABLE_SERVER::LifespanPolicyValue> (value));
        return CORBA::make_reference<PortableServer::LifespanPolicy_impl> (
            TAO_PORTABLE_SERVER::LifespanPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    IDL::traits<PortableServer::IdUniquenessPolicy>::ref_type
    POA_impl::create_id_uniqueness_policy (TAOX11_NAMESPACE::PortableServer::IdUniquenessPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::IdUniquenessPolicy_var _pol =
            this->_proxy ()->create_id_uniqueness_policy (static_cast <TAO_PORTABLE_SERVER::IdUniquenessPolicyValue> (value));
        return CORBA::make_reference<PortableServer::IdUniquenessPolicy_impl> (
            TAO_PORTABLE_SERVER::IdUniquenessPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type
    POA_impl::create_id_assignment_policy (TAOX11_NAMESPACE::PortableServer::IdAssignmentPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::IdAssignmentPolicy_var _pol =
            this->_proxy ()->create_id_assignment_policy (static_cast <TAO_PORTABLE_SERVER::IdAssignmentPolicyValue> (value));
        return CORBA::make_reference<PortableServer::IdAssignmentPolicy_impl> (
            TAO_PORTABLE_SERVER::IdAssignmentPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    IDL::traits<PortableServer::ImplicitActivationPolicy>::ref_type
    POA_impl::create_implicit_activation_policy (TAOX11_NAMESPACE::PortableServer::ImplicitActivationPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::ImplicitActivationPolicy_var _pol =
            this->_proxy ()->create_implicit_activation_policy (static_cast <TAO_PORTABLE_SERVER::ImplicitActivationPolicyValue> (value));
        return CORBA::make_reference<PortableServer::ImplicitActivationPolicy_impl> (
            TAO_PORTABLE_SERVER::ImplicitActivationPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    IDL::traits<PortableServer::ServantRetentionPolicy>::ref_type
    POA_impl::create_servant_retention_policy (TAOX11_NAMESPACE::PortableServer::ServantRetentionPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::ServantRetentionPolicy_var _pol =
            this->_proxy ()->create_servant_retention_policy (static_cast <TAO_PORTABLE_SERVER::ServantRetentionPolicyValue> (value));
        return CORBA::make_reference<PortableServer::ServantRetentionPolicy_impl> (
            TAO_PORTABLE_SERVER::ServantRetentionPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    // generated from c++/cli_hdr/operation.erb
    IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type
    POA_impl::create_request_processing_policy (TAOX11_NAMESPACE::PortableServer::RequestProcessingPolicyValue value)
    {
      try {
        TAO_PORTABLE_SERVER::RequestProcessingPolicy_var _pol =
            this->_proxy ()->create_request_processing_policy (static_cast <TAO_PORTABLE_SERVER::RequestProcessingPolicyValue> (value));
        return CORBA::make_reference<PortableServer::RequestProcessingPolicy_impl> (
            TAO_PORTABLE_SERVER::RequestProcessingPolicy::_narrow (_pol));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }
#endif

    std::string
    POA_impl::the_name()
    {
      TAO_CORBA::String_var safe = this->_proxy ()->the_name ();
      return safe.in();
    }

    IDL::traits<POA>::ref_type
    POA_impl::the_parent()
    {
      TAO_PORTABLE_SERVER::POA_var parentpoa = this->_proxy ()->the_parent ();
      if (!TAO_CORBA::is_nil (parentpoa))
      {
        return CORBA::make_reference<POA_impl> (
                new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(parentpoa.in ())));
      }
      return nullptr;
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    TAOX11_NAMESPACE::PortableServer::POAList
    POA_impl::the_children()
    {
      try {
        TAO_PORTABLE_SERVER::POAList_var poalst = this->_proxy ()->the_children ();
        TAOX11_NAMESPACE::PortableServer::POAList _poa_list;
        for (TAO_CORBA::ULong n = 0; n < poalst->length () ;++n)
        {
          _poa_list.push_back (
              CORBA::make_reference<PortableServer::POA_impl> (
                    new TAOX11_NAMESPACE::POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate (poalst[n]))));
        }
        return _poa_list;
      }
      catch_tao_system_ex(_ex)
      return TAOX11_NAMESPACE::PortableServer::POAList ();
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    IDL::traits<PortableServer::POAManagerFactory>::ref_type
    POA_impl::the_POAManagerFactory()
    {
      TAO_PORTABLE_SERVER::POAManagerFactory_var poamanfact = this->_proxy ()->the_POAManagerFactory ();
      if (!TAO_CORBA::is_nil (poamanfact.in ()))
      {
        return CORBA::make_reference<POAManagerFactory_impl> (
            new POAManagerFactory_proxy (TAO_PORTABLE_SERVER::POAManagerFactory::_duplicate(poamanfact.in ())));
      }
      return nullptr;
    }

    IDL::traits<PortableServer::AdapterActivator>::ref_type
    POA_impl::the_activator()
    {
      TAO_PORTABLE_SERVER::AdapterActivator_var av =
        this->_proxy()->the_activator ();
      if (!TAO_CORBA::is_nil (av.in ()))
        {
          return dynamic_cast<Adapter_Activator_Callback*> (av.in ())->get_impl ();
        }
      return nullptr;
    }

    void
    POA_impl::the_activator(IDL::traits<PortableServer::AdapterActivator>::ref_type _v)
    {
      TAO_PORTABLE_SERVER::AdapterActivator_var activator =
        new Adapter_Activator_Callback (_v);
      this->_proxy()->the_activator (activator.in ());
    }

    IDL::traits<PortableServer::ServantManager>::ref_type
    POA_impl::get_servant_manager ()
    {
      try
      {
        TAO_PORTABLE_SERVER::ServantManager_var mngr =
          this->_proxy()->get_servant_manager ();

        TAO_PORTABLE_SERVER::ServantActivator_var sa =
          TAO_PORTABLE_SERVER::ServantActivator::_narrow(mngr);
        if (! TAO_CORBA::is_nil (sa.in ()))
          {
            return dynamic_cast<Servant_Activator_Callback*>(sa.in ())->get_impl();
          }
        TAO_PORTABLE_SERVER::ServantLocator_var sl =
          TAO_PORTABLE_SERVER::ServantLocator::_narrow(mngr);
        if (! TAO_CORBA::is_nil (sl.in ()))
          {
            return dynamic_cast<Servant_Locator_Callback*>(sl.in ())->get_impl();
          }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    void
    POA_impl::set_servant_manager (IDL::traits<PortableServer::ServantManager>::ref_type imgr)
    {
      try
      {
        // there are cases in which a null ptr results in an exception. To leave this behavior
        // intact, provide a null ptr to the TAO POA.
        if (!imgr)
          this->_proxy()->set_servant_manager(TAO_PORTABLE_SERVER::ServantActivator::_nil ());

        const IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantActivator>::ref_type sa =
          IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantActivator>::narrow (imgr);
        if (sa != nullptr)
          {
            TAOX11_LOG_DEBUG ("Successfully narrowed given ServantManager to a ServantActivator.");
            TAO_PORTABLE_SERVER::ServantActivator_var tao_sa =
              new ::TAOX11_NAMESPACE::PortableServer::Servant_Activator_Callback (sa);
            this->_proxy()->set_servant_manager(tao_sa.in ());
            return;
          }
        const IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantLocator>::ref_type sl =
          IDL::traits<::TAOX11_NAMESPACE::PortableServer::ServantLocator>::narrow (imgr);
        if (sl != nullptr)
          {
            TAOX11_LOG_DEBUG ("Successfully narrowed given ServantManager to a ServantLocator.");
            TAO_PORTABLE_SERVER::ServantLocator_var tao_sl =
              new ::TAOX11_NAMESPACE::PortableServer::Servant_Locator_Callback (sl);
            this->_proxy()->set_servant_manager(tao_sl.in ());
            return;
          }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
    }

    CORBA::servant_reference<PortableServer::Servant>
    POA_impl::get_servant ()
    {
      try {
        TAO_PORTABLE_SERVER::ServantBase_var _srv = this->_proxy ()->get_servant ();
        if (!TAO_CORBA::is_nil (_srv))
        {
          return dynamic_cast<TAOX11_NAMESPACE::Servant_proxy_ptr> (_srv.in ())->_get_skel()->_lock ();
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::NoServant&) {
        throw POA::NoServant ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    void
    POA_impl::set_servant (CORBA::servant_reference<PortableServer::Servant> p_servant)
    {
      try {
        this->_proxy ()->set_servant (p_servant->get_proxy ());
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
    }
#endif

    IDL::traits<CORBA::Object>::ref_type
    POA_impl::create_reference (const TAOX11_NAMESPACE::CORBA::RepositoryId& intf)
    {
      try {
        TAO_CORBA::Object_ptr obj = this->_proxy ()->create_reference (intf.c_str ());
        if (!TAO_CORBA::is_nil (obj))
        {
          return TAOX11_NAMESPACE::CORBA::Object::_narrow (TAOX11_NAMESPACE::Object_proxy (obj));
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

#if !defined (CORBA_E_MICRO)
    IDL::traits<CORBA::Object>::ref_type
    POA_impl::create_reference_with_id (const TAOX11_NAMESPACE::PortableServer::ObjectId& oid,
                                        const TAOX11_NAMESPACE::CORBA::RepositoryId& intf)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId tao_oid;
        TAOX11_NAMESPACE::seq_from_x11 (tao_oid, oid);
        TAO_CORBA::Object_ptr obj = this->_proxy ()->create_reference_with_id (tao_oid, intf.c_str ());
        if (!TAO_CORBA::is_nil (obj))
        {
          return TAOX11_NAMESPACE::CORBA::Object::_narrow (TAOX11_NAMESPACE::Object_proxy (obj));
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }
#endif

    TAOX11_NAMESPACE::PortableServer::ObjectId
    POA_impl::servant_to_id (CORBA::servant_reference<PortableServer::Servant> p_servant)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId_var tao_oid =
            this->_proxy ()->servant_to_id (p_servant->get_proxy ());
        return seq_to_x11<TAOX11_NAMESPACE::PortableServer::ObjectId> (tao_oid.in ());
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::ServantNotActive&) {
        throw POA::ServantNotActive ();
      }
      catch_tao_system_ex(_ex)
      return TAOX11_NAMESPACE::PortableServer::ObjectId ();
    }

    IDL::traits<CORBA::Object>::ref_type
    POA_impl::servant_to_reference (CORBA::servant_reference<PortableServer::Servant> p_servant)
    {
      try {
        TAO_CORBA::Object_var tmp =
            this->_proxy ()->servant_to_reference (p_servant->get_proxy ());
        if (!TAO_CORBA::is_nil (tmp.in ()))
        {
          return TAOX11_NAMESPACE::CORBA::Object::_narrow (
              TAOX11_NAMESPACE::Object_proxy (TAO_CORBA::Object::_duplicate (tmp.in ())));
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::ServantNotActive&) {
        throw POA::ServantNotActive ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    CORBA::servant_reference<PortableServer::Servant>
    POA_impl::reference_to_servant (IDL::traits<CORBA::Object>::ref_type reference)
    {
      try {
        TAO_PORTABLE_SERVER::ServantBase_var _srv =
            this->_proxy ()->reference_to_servant (reference->get_proxy ()->get ());
        if (!TAO_CORBA::is_nil (_srv))
        {
          return dynamic_cast<TAOX11_NAMESPACE::Servant_proxy_ptr> (_srv.in ())->_get_skel()->_lock ();
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongAdapter&) {
        throw POA::WrongAdapter ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::ObjectNotActive&) {
        throw POA::ObjectNotActive ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    TAOX11_NAMESPACE::PortableServer::ObjectId
    POA_impl::reference_to_id (IDL::traits<CORBA::Object>::ref_type reference)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId_var tao_oid =
            this->_proxy ()->reference_to_id (reference->get_proxy ()->get ());
        return seq_to_x11<TAOX11_NAMESPACE::PortableServer::ObjectId> (tao_oid.in ());
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongAdapter&) {
        throw POA::WrongAdapter ();
      }
      catch_tao_system_ex(_ex)
      return TAOX11_NAMESPACE::PortableServer::ObjectId ();
    }

    CORBA::servant_reference<PortableServer::Servant>
    POA_impl::id_to_servant (const TAOX11_NAMESPACE::PortableServer::ObjectId& oid)
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId _oid;
        seq_from_x11 (_oid, oid);
        TAO_PORTABLE_SERVER::ServantBase_var _srv =
            this->_proxy ()->id_to_servant (_oid);
        if (!TAO_CORBA::is_nil (_srv))
        {
          return dynamic_cast<TAOX11_NAMESPACE::Servant_proxy_ptr> (_srv.in ())->_get_skel()->_lock ();
        }
      }
      catch (const TAO_PORTABLE_SERVER::POA::WrongPolicy&) {
        throw POA::WrongPolicy ();
      }
      catch (const TAO_PORTABLE_SERVER::POA::ObjectNotActive&) {
        throw POA::ObjectNotActive ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    TAOX11_NAMESPACE::CORBA::OctetSeq
    POA_impl::id()
    {
      try {
        TAO_CORBA::OctetSeq_var tao_id =
            this->_proxy ()->id ();
        return seq_to_x11<TAOX11_NAMESPACE::CORBA::OctetSeq> (tao_id.in ());
      }
      catch_tao_system_ex(_ex)
      return TAOX11_NAMESPACE::CORBA::OctetSeq ();
    }

    /*
     *  POAManager_impl implementation
     */

    POAManager_impl::POAManager_impl (POAManager_proxy_ptr op)
      : Object (),
        POAManager (),
        proxy_ (op)
    {
    }

    POAManager_proxy& POAManager_impl::_proxy ()
    {
      return *this->proxy_;
    }

    void
    POAManager_impl::activate ()
    {
      try {
        this->_proxy ()->activate ();
      }
      catch (const TAO_PORTABLE_SERVER::POAManager::AdapterInactive&) {
        throw POAManager::AdapterInactive ();
      }
      catch_tao_system_ex(_ex)
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
    void
    POAManager_impl::hold_requests (bool wait_for_completion)
    {
      try {
        this->_proxy ()->hold_requests (wait_for_completion);
      }
      catch (const TAO_PORTABLE_SERVER::POAManager::AdapterInactive&) {
        throw POAManager::AdapterInactive ();
      }
      catch_tao_system_ex(_ex)
    }

    void
    POAManager_impl::discard_requests (bool wait_for_completion)
    {
      try {
        this->_proxy ()->discard_requests (wait_for_completion);
      }
      catch (const TAO_PORTABLE_SERVER::POAManager::AdapterInactive&) {
        throw POAManager::AdapterInactive ();
      }
      catch_tao_system_ex(_ex)
    }

    void
    POAManager_impl::deactivate (bool etherealize_objects,
                                 bool wait_for_completion)
    {
      try {
        this->_proxy ()->deactivate (etherealize_objects, wait_for_completion);
      }
      catch (const TAO_PORTABLE_SERVER::POAManager::AdapterInactive&) {
        throw POAManager::AdapterInactive ();
      }
      catch_tao_system_ex(_ex)
    }
#endif

    POAManager::State
    POAManager_impl::get_state ()
    {
      return static_cast<POAManager::State> (this->_proxy ()->get_state ());
    }

    std::string
    POAManager_impl::get_id ()
    {
      TAO_CORBA::String_var tao_id = this->_proxy ()->get_id ();
      return tao_id.in ();
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    /*
     *  POAManagerFactory_impl implementation
     */

    POAManagerFactory_impl::POAManagerFactory_impl (POAManagerFactory_proxy_ptr op)
      : Object (),
        POAManagerFactory (),
        proxy_ (op)
    {
    }

    POAManagerFactory_proxy& POAManagerFactory_impl::_proxy ()
    {
      return *this->proxy_;
    }

    IDL::traits<PortableServer::POAManager>::ref_type
    POAManagerFactory_impl::create_POAManager (const std::string& id,
                                               const TAOX11_NAMESPACE::CORBA::PolicyList& policies)
    {
      TAO_CORBA::PolicyList _pols;
      _pols.length (TAO_CORBA::ULong (policies.size ()));
      TAO_CORBA::ULong n (0);
      for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
      {
        _pols[n++] =
            TAO_CORBA::Policy::_narrow (_pol->get_proxy ()->get ());
      }
      try {
        TAO_PORTABLE_SERVER::POAManager_var poaman =
            this->_proxy ()->create_POAManager (id.c_str (),
                                                _pols);
        if (!TAO_CORBA::is_nil (poaman))
        {
          return CORBA::make_reference<POAManager_impl> (
                  new POAManager_proxy (TAO_PORTABLE_SERVER::POAManager::_duplicate(poaman.in ())));
        }
      }
      catch (const TAO_PORTABLE_SERVER::POAManagerFactory::ManagerAlreadyExists&) {
        throw TAOX11_NAMESPACE::PortableServer::POAManagerFactory::ManagerAlreadyExists ();
      }
      catch (const TAO_CORBA::PolicyError& _x) {
        throw TAOX11_NAMESPACE::CORBA::PolicyError (static_cast<TAOX11_NAMESPACE::CORBA::PolicyErrorCode> (_x.reason));
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    TAOX11_NAMESPACE::PortableServer::POAManagerFactory::POAManagerSeq
    POAManagerFactory_impl::list ()
    {
      try {
        TAO_PORTABLE_SERVER::POAManagerFactory::POAManagerSeq_var poamanlst = this->_proxy ()->list ();
        POAManagerSeq _poaman_list;
        for (TAO_CORBA::ULong n = 0; n < poamanlst->length () ;++n)
        {
          _poaman_list.push_back (
              CORBA::make_reference<PortableServer::POAManager_impl> (
                      new TAOX11_NAMESPACE::POAManager_proxy (TAO_PORTABLE_SERVER::POAManager::_duplicate (poamanlst[n]))));
        }
        return _poaman_list;
      }
      catch_tao_system_ex(_ex)
      return POAManagerSeq ();
    }

    IDL::traits<PortableServer::POAManager>::ref_type
    POAManagerFactory_impl::find (const std::string& id)
    {
      try {
        TAO_PORTABLE_SERVER::POAManager_var poaman =
            this->_proxy ()->find (id.c_str ());
        if (!TAO_CORBA::is_nil (poaman))
        {
          return CORBA::make_reference<POAManager_impl> (
                  new POAManager_proxy (TAO_PORTABLE_SERVER::POAManager::_duplicate(poaman.in ())));
        }
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }
#endif

    /*
     *  POA_Current_impl implementation
     */

    POA_Current_impl::POA_Current_impl (POA_Current_proxy_ptr op)
      : Current (),
        proxy_ (op)
    {
    }

    POA_Current_proxy& POA_Current_impl::_proxy ()
    {
      return *this->proxy_;
    }

    IDL::traits<PortableServer::POA>::ref_type
    POA_Current_impl::get_POA ()
    {
      try {
        TAO_PORTABLE_SERVER::POA_var poa =
            this->_proxy ()->get_POA ();
        if (!TAO_CORBA::is_nil (poa))
        {
          return CORBA::make_reference<POA_impl> (
                  new POA_proxy (TAO_PORTABLE_SERVER::POA::_duplicate(poa.in ())));
        }
      }
      catch (const TAO_PORTABLE_SERVER::Current::NoContext&) {
        throw Current::NoContext ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    // generated from c++/cli_hdr/operation.erb
    TAOX11_NAMESPACE::PortableServer::ObjectId
    POA_Current_impl::get_object_id ()
    {
      try {
        TAO_PORTABLE_SERVER::ObjectId_var oid =
            this->_proxy ()->get_object_id ();
        return seq_to_x11<PortableServer::ObjectId> (oid.in ());
      }
      catch (const TAO_PORTABLE_SERVER::Current::NoContext&) {
        throw Current::NoContext ();
      }
      catch_tao_system_ex(_ex)
      return PortableServer::ObjectId ();
    }

    // generated from c++/cli_hdr/operation.erb
    IDL::traits<CORBA::Object>::ref_type
    POA_Current_impl::get_reference ()
    {
      try {
        TAO_CORBA::Object_var obj =
            this->_proxy ()->get_reference ();
        if (!TAO_CORBA::is_nil (obj))
        {
          return TAOX11_NAMESPACE::CORBA::Object::_narrow (TAOX11_NAMESPACE::Object_proxy (TAO_CORBA::Object::_duplicate (obj)));
        }
      }
      catch (const TAO_PORTABLE_SERVER::Current::NoContext&) {
        throw Current::NoContext ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

    // generated from c++/cli_hdr/operation.erb
    CORBA::servant_reference<PortableServer::Servant>
    POA_Current_impl::get_servant ()
    {
      try {
        TAO_PORTABLE_SERVER::Servant _srv = this->_proxy ()->get_servant ();
        if (!TAO_CORBA::is_nil (_srv))
        {
          return dynamic_cast<TAOX11_NAMESPACE::Servant_proxy_ptr> (_srv)->_get_skel()->_lock ();
        }
      }
      catch (const TAO_PORTABLE_SERVER::Current::NoContext&) {
        throw Current::NoContext ();
      }
      catch_tao_system_ex(_ex)
      return nullptr;
    }

  } // namespace PortableServer
} // namespace TAOX11_NAMESPACE
