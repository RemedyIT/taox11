/**
 * @file    orb_registry.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ORB registry class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/orbproxy.h"
#include "tao/objectid.h"

#include "tao/x11/orb_registry.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/log.h"
#include "tao/x11/object_loader.h"
#include "tao/x11/exception_macros.h"

#include "ace/Dynamic_Service.h"

namespace TAOX11_NAMESPACE
{
  void
  ORB_Registry::register_user_service (
      std::string name,
      IDL::traits<CORBA::Object>::ref_type svcobj)
  {
    // lock
    std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

    user_services_map_t::iterator it = this->user_services_.find (name);
    if (it != this->user_services_.end ())
    {
      throw CORBA::ORB::InvalidName ();
    }
    this->user_services_.insert ({name, svcobj});
  }

  IDL::traits<CORBA::Object>::ref_type
  ORB_Registry::resolve_user_service (
      const std::string& name)
  {
    // lock
    std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

    user_services_map_t::iterator it = this->user_services_.find (name);
    if (it != this->user_services_.end ())
      return it->second;
    return {};
  }

  void
  ORB_Registry::unregister_user_service (const std::string& name)
  {
    // lock
    std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

    user_services_map_t::iterator it = this->user_services_.find (name);
    if (it == this->user_services_.end ())
    {
      throw CORBA::ORB::InvalidName ();
    }
    this->user_services_.erase (it);
  }

  IDL::traits<CORBA::Object>::ref_type
  ORB_Registry::resolve_tao_service (
      CORBA::object_reference<CORBA::ORB> orb,
      const std::string& name)
  {
    try {
      TAO_CORBA::Object_var tmp = orb->proxy ()->resolve_initial_references (name.c_str ());
      if (!TAO_CORBA::is_nil (tmp))
        {
          return CORBA::Object::_narrow (Object_proxy (TAO_CORBA::Object::_duplicate (tmp)));
        }
    }
    catch (const TAO_CORBA::ORB::InvalidName&) {
      throw CORBA::ORB::InvalidName ();
    }
    catch_tao_system_ex (_sx)
    return {};
  }

  IDL::traits<CORBA::Object>::ref_type
  ORB_Registry::resolve_initial_reference (
      CORBA::object_reference<CORBA::ORB> orb,
      const std::string& name)
  {
    TAOX11_LOG_TRACE("ORB_Registry::resolve_initial_reference ");
    // first check for known native X11 services
    if (name == TAO_OBJID_DYNANYFACTORY)
    {
      TAOX11_LOG_TRACE("ORB_Registry::resolve_initial_reference  name == TAO_OBJID_DYNANYFACTORY");

      if (this->dynamicany_factory_loader_ == nullptr)
      {
        Object_Loader *cfloader = this->resolve_object_loader ("TAOX11_DynamicAny_Loader");
        if (cfloader)
        {
          // lock
          std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

          this->dynamicany_factory_loader_ = cfloader;
        }
      }
      if (this->dynamicany_factory_loader_)
      {
        return this->dynamicany_factory_loader_->create_object (orb, 0, 0);
      }
      else
      {
        throw CORBA::ORB::InvalidName ();
      }
    }
    else if (name == TAO_OBJID_TYPECODEFACTORY)
    {
      if (this->typecode_factory_loader_ == nullptr)
      {
        Object_Loader *tcfloader = this->resolve_object_loader ("TAOX11_TypeCodeFactory_Loader");
        if (tcfloader)
        {
          // lock
          std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

          this->typecode_factory_loader_ = tcfloader;
        }
      }
      if (this->typecode_factory_loader_)
      {
        return this->typecode_factory_loader_->create_object (orb, 0, 0);
      }
      else
      {
        throw CORBA::ORB::InvalidName ();
      }
    }
    else if (name == TAO_OBJID_CODECFACTORY)
    {
      if (this->codec_factory_loader_ == nullptr)
      {
        Object_Loader *cfloader = this->resolve_object_loader ("TAOX11_CodecFactory_Loader");
        if (cfloader)
        {
          // lock
          std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

          this->codec_factory_loader_ = cfloader;
        }
      }
      if (this->codec_factory_loader_)
      {
        return this->codec_factory_loader_->create_object (orb, 0, 0);
      }
      else
      {
        throw CORBA::ORB::InvalidName ();
      }
    }
    else
    {
      // now check for user registered X11 services
      IDL::traits<CORBA::Object>::ref_type svcobj =
          this->resolve_user_service (name);

      if (svcobj)
      {
        return svcobj; // done
      }

      // fall back to resolving TAO services
      return ORB_Registry::resolve_tao_service (orb, name);
    }
    return {};
  }

  CORBA::ORB::ObjectIdList
  ORB_Registry::list_initial_references (
      CORBA::object_reference<CORBA::ORB> orb)
  {
    CORBA::ORB::ObjectIdList reflist;
    // first add the standard list as returned by TAO
    try {
      TAO_CORBA::ORB::ObjectIdList_var _tao_list = orb->proxy ()->list_initial_services ();

      reflist = seq_to_x11<CORBA::ORB::ObjectIdList> (_tao_list.in ());
    }
    catch_tao_system_ex (_sx)
    // now add the X11 user registered services
    {
      // lock
      std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

      for (const user_services_map_t::value_type& usrsvc_pair : this->user_services_)
      {
        reflist.push_back (usrsvc_pair.first);
      }
    }
    return reflist;
  }

  Object_Loader *
  ORB_Registry::resolve_object_loader (const std::string& name)
  {
    Object_Loader *obj_loader {};
    try {
      TAOX11_LOG_DEBUG ( "ORB_Registry::resolve_object_loader - Resolving object loader <"
            << name << ">");
      obj_loader =
        ACE_Dynamic_Service<Object_Loader>::instance (name.c_str ());
    }
    catch (...) {
      TAOX11_LOG_ERROR ("ORB_Registry::resolve_object_loader - "
            << "Unknown exception caught while resolving the "
            << "Object_Loader <" << name << ">");
    }
    return obj_loader;
  }

  AnyInsertAdapter *ORB_Registry::any_insert_adapter ()
  {
    if (this->any_insert_adapter_ == nullptr)
    {
      // lock
      std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

      if (this->any_insert_adapter_ == nullptr)
      {
        try {
          TAOX11_LOG_DEBUG ( "ORB_Registry::any_insert_adapter - Creating adapter with name <"
                << AnyInsertAdapter::concrete_adapter_name () << ">");
          this->any_insert_adapter_ =
            ACE_Dynamic_Service<AnyInsertAdapter>::instance (
                AnyInsertAdapter::concrete_adapter_name ().c_str ());
        }
        catch (...) {
          TAOX11_LOG_ERROR ("ORB_Registry::any_insert_adapter - "
                << "Unknown exception caught while creating the "
                << "Any Insert Adapter <"
                << AnyInsertAdapter::concrete_adapter_name ()
                << ">");
        }
        if (this->any_insert_adapter_ == nullptr)
        {
          TAOX11_LOG_ERROR ("ORB_Registry::any_insert_adapter - "
                << "Unable to create adapter: "
                << "Throwing a CORBA::INTERNAL exception");
          throw CORBA::INTERNAL ();
        }
      }
    }
    return this->any_insert_adapter_;
  }

#if !defined(CORBA_E_MICRO)
  ValueFactoryManagerAdapter *ORB_Registry::valuefactory_manager ()
  {
    if (this->vf_manager_ == nullptr)
    {
      std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

      if (this->vf_manager_ == nullptr)
      {
        try {
          TAOX11_LOG_DEBUG ( "ORB_Registry::valuefactory_manager - Creating manager with name <"
                << ValueFactoryManagerAdapter::concrete_adapter_name () << ">");
          this->vf_manager_ =
              ACE_Dynamic_Service<ValueFactoryManagerAdapter>::instance (
                  ValueFactoryManagerAdapter::concrete_adapter_name ().c_str ()
              );
        }
        catch (...) {
          TAOX11_LOG_ERROR ("ORB_Registry::valuefactory_manager - "
                << "Unknown exception caught while creating the "
                << "ValueFactory Manager <"
                << ValueFactoryManagerAdapter::concrete_adapter_name ()
                << ">");
        }

        if (this->vf_manager_ == nullptr)
        {
          TAOX11_LOG_ERROR ("ORB_Registry::valuefactory_manager - "
                << "Unable to create manager: "
                << "Throwing a CORBA::INTERNAL exception");
          throw CORBA::INTERNAL ();
        }
      }
    }

    return this->vf_manager_;
  }

  ValueTypeAdapter *ORB_Registry::value_type_adapter ()
  {
    if (this->vt_adapter_ == nullptr)
    {
      std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

      if (this->vt_adapter_ == nullptr)
      {
        try {
          TAOX11_LOG_DEBUG ( "ORB_Registry::value_type_adapter - Creating adapter with name <"
                << ValueTypeAdapter::concrete_adapter_name () << ">");
          this->vt_adapter_ =
              ACE_Dynamic_Service<ValueTypeAdapter>::instance (
                  ValueTypeAdapter::concrete_adapter_name ().c_str ()
              );
        }
        catch (...) {
          TAOX11_LOG_ERROR ("ORB_Registry::value_type_adapter - "
                << "Unknown exception caught while creating the "
                << "Value Type Adapter <"
                << ValueTypeAdapter::concrete_adapter_name ()
                << ">");
        }

        if (this->vt_adapter_ == nullptr)
        {
          TAOX11_LOG_ERROR ("ORB_Registry::value_type_adapter - "
                << "Unable to create adapter: "
                << "Throwing a CORBA::INTERNAL exception");
          throw CORBA::INTERNAL ();
        }
      }
    }

    return this->vt_adapter_;
  }

  PolicyFactoryManagerAdapter *ORB_Registry::policy_manager ()
  {
    if (this->policy_manager_ == nullptr)
    {
      std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

      if (this->policy_manager_ == nullptr)
      {
        try {
          TAOX11_LOG_DEBUG ( "ORB_Registry::policy_manager - Creating manager with name <"
                << PolicyFactoryManagerAdapter::concrete_adapter_name () << ">");
          this->policy_manager_ =
              ACE_Dynamic_Service<PolicyFactoryManagerAdapter>::instance (
                  PolicyFactoryManagerAdapter::concrete_adapter_name ().c_str ());
        }
        catch (...)
        {
          TAOX11_LOG_ERROR ("ORB_Registry::policy_manager - "
                << "Unknown exception caught while creating the "
                << "Policy Manager <"
                << PolicyFactoryManagerAdapter::concrete_adapter_name ()
                << ">");
        }
      }

      if (this->policy_manager_ == nullptr)
      {
        TAOX11_LOG_ERROR ("ORB_Registry::policy_manager - "
              << "Unable to create policy_manager: "
              << "Throwing a CORBA::INTERNAL exception");
        throw CORBA::INTERNAL ();
      }
    }

    return this->policy_manager_;
  }
#endif

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    IFRClientAdapter *ORB_Registry::ifr_client_adapter ()
    {
      if (this->ifr_client_adapter_ == nullptr)
      {
        std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

        if (this->ifr_client_adapter_ == nullptr)
        {
          try {
            TAOX11_LOG_DEBUG ( "ORB_Registry::ifr_client_adapter - Creating adapter with name <"
                  << IFRClientAdapter::concrete_adapter_name () << ">");
            this->ifr_client_adapter_ =
                ACE_Dynamic_Service<IFRClientAdapter>::instance (
                    IFRClientAdapter::concrete_adapter_name ().c_str ()
                );
          }
          catch (...) {
            TAOX11_LOG_ERROR ("ORB_Registry::ifr_client_adapter - "
                  << "Unknown exception caught while creating the "
                  << "IFR Client Adapter <"
                  << IFRClientAdapter::concrete_adapter_name ()
                  << ">");
          }

          if (this->ifr_client_adapter_ == nullptr)
          {
            TAOX11_LOG_ERROR ("ORB_Registry::ifr_client_adapter - "
                  << "Unable to create adapter: "
                  << "Throwing a CORBA::INTERNAL exception");
            throw CORBA::INTERNAL ();
          }
        }
      }

      return this->ifr_client_adapter_;
    }

    DynamicAdapter *ORB_Registry::dynamic_adapter ()
    {
      if (this->dynamic_adapter_ == nullptr)
      {
        std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

        if (this->dynamic_adapter_ == nullptr)
        {
          try {
            TAOX11_LOG_DEBUG ( "ORB_Registry::dynamic_adapter - Creating adapter with name <"
                  << DynamicAdapter::concrete_adapter_name () << ">");
            this->dynamic_adapter_ =
                ACE_Dynamic_Service<DynamicAdapter>::instance (
                    DynamicAdapter::concrete_adapter_name ().c_str ()
                );
          }
          catch (...) {
            TAOX11_LOG_ERROR ("ORB_Registry::dynamic_adapter - "
                  << "Unknown exception caught while creating the "
                  << "Dynamic Adapter <"
                  << DynamicAdapter::concrete_adapter_name ()
                  << ">");
          }

          if (this->dynamic_adapter_ == nullptr)
          {
            TAOX11_LOG_ERROR ("ORB_Registry::dynamic_adapter - "
                  << "Unable to create adapter: "
                  << "Throwing a CORBA::INTERNAL exception");
            throw CORBA::INTERNAL ();
          }
        }
      }

      return this->dynamic_adapter_;
    }

# if (TAO_HAS_MINIMUM_CORBA == 0)
    NVListAdapter *ORB_Registry::nvlist_adapter ()
    {
      if (this->nvlist_adapter_ == nullptr)
      {
        std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

        if (this->nvlist_adapter_ == nullptr)
        {
          try {
            TAOX11_LOG_DEBUG ( "ORB_Registry::nvlist_adapter - Creating adapter with name <"
                  << NVListAdapter::concrete_adapter_name () << ">");
            this->nvlist_adapter_ =
                ACE_Dynamic_Service<NVListAdapter>::instance (
                    NVListAdapter::concrete_adapter_name ().c_str ()
                );
          }
          catch (...) {
            TAOX11_LOG_ERROR ("ORB_Registry::nvlist_adapter - "
                  << "Unknown exception caught while creating the "
                  << "NVList Adapter <"
                  << NVListAdapter::concrete_adapter_name ()
                  << ">");
          }

          if (this->nvlist_adapter_ == nullptr)
          {
            TAOX11_LOG_ERROR ("ORB_Registry::nvlist_adapter - "
                  << "Unable to create adapter: "
                  << "Throwing a CORBA::INTERNAL exception");
            throw CORBA::INTERNAL ();
          }
        }
      }

      return this->nvlist_adapter_;
    }

    TypecodeFactoryAdapter *ORB_Registry::typecode_factory_adapter ()
    {
      if (this->typecode_factory_adapter_ == nullptr)
      {
        std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

        if (this->typecode_factory_adapter_ == nullptr)
        {
          try {
            TAOX11_LOG_DEBUG ( "ORB_Registry::typecode_factory_adapter - Creating adapter with name <"
                  << TypecodeFactoryAdapter::concrete_adapter_name () << ">");
            this->typecode_factory_adapter_ =
                ACE_Dynamic_Service<TypecodeFactoryAdapter>::instance (
                    TypecodeFactoryAdapter::concrete_adapter_name ().c_str ()
                );
          }
          catch (...) {
            TAOX11_LOG_ERROR ("ORB_Registry::typecode_factory_adapter - "
                  << "Unknown exception caught while creating the "
                  << "Typecode Factory Adapter <"
                  << TypecodeFactoryAdapter::concrete_adapter_name ()
                  << ">");
          }

          if (this->typecode_factory_adapter_ == nullptr)
          {
            TAOX11_LOG_ERROR ("ORB_Registry::typecode_factory_adapter - "
                  << "Unable to create adapter: "
                  << "Throwing a CORBA::INTERNAL exception");
            throw CORBA::INTERNAL ();
          }
        }
      }

      return this->typecode_factory_adapter_;
    }
# endif
#endif

  std::mutex ORB_Registry::lock_;
  std::weak_ptr<ORB_Registry> ORB_Registry::instance_;

  std::shared_ptr<ORB_Registry> ORB_Registry::instance (bool create)
  {
    // check for existing instance
    std::shared_ptr<ORB_Registry> orbreg = ORB_Registry::instance_.lock ();
    if (!orbreg && create)
    {
      // lock
      std::lock_guard<std::mutex> __guard (ORB_Registry::lock_);

      // check again
      if (ORB_Registry::instance_.expired ())
      {
        // create and register new instance
        orbreg = std::shared_ptr<ORB_Registry> (new ORB_Registry ());
        ORB_Registry::instance_ = orbreg;
      }
      else
      {
        // lock existing instance
        orbreg = ORB_Registry::instance_.lock ();
      }
    }
    // return instance
    return orbreg;
  }
} // namespace TAOX11_NAMESPACE
