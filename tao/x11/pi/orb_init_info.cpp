/**
 * @file    orb_init_info.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI ORBInitInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// TAO namespace dependencies
#include "tao/x11/orbproxy.h"
#include "tao/x11/objproxy.h"

// X11 namespace dependencies
#include "tao/x11/pi/orb_init_info.h"

#include "tao/x11/pi/client_request_interceptor_proxy.h"

#include "tao/x11/codecfactory/codecfactory_impl.h"

#include "tao/x11/log.h"
#include "tao/x11/orb.h"
#include "tao/x11/orb_registry.h"
#include "tao/x11/orb_constants.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {

#if TAO_HAS_INTERCEPTORS == 1

    // custom guard to manage SRI proxy pointers
    // cannot use regular _var as that pulls in code requiring
    // explicit linking to PI_Server which is not what we want
    struct sri_proxy_guard
    {
      TAO_PI::ServerRequestInterceptor* sri_ {};

      explicit sri_proxy_guard (TAO_PI::ServerRequestInterceptor* sri)
        : sri_ (sri)
      {}
      ~sri_proxy_guard ()
      {
        ORBInitInfo::get_sri_proxy_adapter ()
            ->release_server_request_interceptor_proxy (this->sri_);
      }
    };

    ServerRequestInterceptorProxyAdapter*
    ORBInitInfo::load_sri_proxy_adapter ()
    {
      try {
        return ACE_Dynamic_Service<ServerRequestInterceptorProxyAdapter>::instance (
            ServerRequestInterceptorProxyAdapter::concrete_adapter_name ().c_str ());
      }
      catch (...)
      {
        TAOX11_LOG_ERROR ("ORBInitInfo::load_sri_proxy_adapter - "
              << "Unknown exception caught while creating the "
              << "ServerRequestInterceptorProxyAdapter <"
              << ServerRequestInterceptorProxyAdapter::concrete_adapter_name ()
              << ">");
      }
      return nullptr;
    }

    ServerRequestInterceptorProxyAdapter*
    ORBInitInfo::get_sri_proxy_adapter ()
    {
      static ServerRequestInterceptorProxyAdapter* sri_proxy_adapter_ =
          ORBInitInfo::load_sri_proxy_adapter ();

      return sri_proxy_adapter_;
    }

    // custom guard to manage IORInterceptor proxy pointers
    // cannot use regular _var as that pulls in code requiring
    // explicit linking to IOR_Interceptor which is not what we want
    struct iori_proxy_guard
    {
      TAO_PI::IORInterceptor* iori_ {};

      explicit iori_proxy_guard (TAO_PI::IORInterceptor* iori)
        : iori_ (iori)
      {}
      ~iori_proxy_guard ()
      {
        ORBInitInfo::get_iorinterceptor_proxy_adapter ()
            ->release_ior_interceptor_proxy (this->iori_);
      }
    };

    IORInterceptorProxyAdapter*
    ORBInitInfo::load_iorinterceptor_proxy_adapter ()
    {
      try {
        return ACE_Dynamic_Service<IORInterceptorProxyAdapter>::instance (
            IORInterceptorProxyAdapter::concrete_adapter_name ().c_str ());
      }
      catch (...)
      {
        TAOX11_LOG_ERROR ("ORBInitInfo::load_iorinterceptor_proxy_adapter - "
              << "Unknown exception caught while creating the "
              << "IORInterceptorProxyAdapter <"
              << IORInterceptorProxyAdapter::concrete_adapter_name ()
              << ">");
      }
      return nullptr;
    }

    IORInterceptorProxyAdapter*
    ORBInitInfo::get_iorinterceptor_proxy_adapter ()
    {
      static IORInterceptorProxyAdapter* ii_proxy_adapter_ =
          ORBInitInfo::load_iorinterceptor_proxy_adapter ();

      return ii_proxy_adapter_;
    }

#endif

    ORBInitInfo::ORBInitInfo (
        TAO_PI::ORBInitInfo_3_1_ptr orbii)
      : orbii_ (orbii)
    {
      this->set_proxy (new Object_proxy (orbii));
    }

    CORBA::StringSeq
    ORBInitInfo::arguments ()
    {
      try {
        TAO_CORBA::StringSeq_var safe_args (this->orbii_->arguments ());
        return std::move (seq_to_x11<CORBA::StringSeq> (safe_args.in ()));
      }
      catch_tao_system_ex (_sx)
      return {};
    }

    std::string
    ORBInitInfo::orb_id ()
    {
      TAO_CORBA::String_var id = this->orbii_->orb_id ();
      return TAO_CORBA::is_nil (id.in ()) ? std::string () : id.in ();
    }

    IDL::traits<IOP::CodecFactory>::ref_type
    ORBInitInfo::codec_factory ()
    {
      try {
        TAO_IOP::CodecFactory_ptr tao_cf = this->orbii_->codec_factory ();
        if (!TAO_CORBA::is_nil (tao_cf))
        {
          return CORBA::make_reference< IOP_impl::CodecFactory > (tao_cf);
        }
      }
      catch_tao_system_ex (_sx)
      return {};
    }

    void
    ORBInitInfo::register_initial_reference (
        const ObjectId& id,
        IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type obj)
    {
      try
      {
        this->orbii_->register_initial_reference (
            id.c_str (),
            obj ? obj->get_proxy ()->get () : TAO_CORBA::Object::_nil ());
      }
      catch (const TAO_PI::ORBInitInfo_3_1::InvalidName&)
      {
        throw PortableInterceptor::ORBInitInfo_3_1::InvalidName ();
      }
      catch_tao_system_ex (_sx)
    }

    IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type
    ORBInitInfo::resolve_initial_references (
        const ObjectId& id)
    {
      IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type rc;
      try
      {
        TAO_CORBA::Object_var tao_obj = this->orbii_->resolve_initial_references (
            id.c_str ());
        if (!TAO_CORBA::is_nil (tao_obj.in ()))
          rc = CORBA::make_reference<CORBA::Object> (new Object_proxy (tao_obj._retn ()));
      }
      catch (const TAO_PI::ORBInitInfo_3_1::InvalidName&)
      {
        throw PortableInterceptor::ORBInitInfo_3_1::InvalidName ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    void
    ORBInitInfo::add_client_request_interceptor (
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ClientRequestInterceptor>::ref_type interceptor)
    {
#if TAO_HAS_INTERCEPTORS == 1
      TAO_PI::ClientRequestInterceptor_ptr tao_ptr;
      ACE_NEW_THROW_EX (tao_ptr,
                        PortableInterceptor_impl::ClientRequestInterceptor_proxy (
                            interceptor),
                        CORBA::NO_MEMORY (
                          TAO_CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      TAO_PI::ClientRequestInterceptor_var tao_interceptor = tao_ptr;
      try
      {
        this->orbii_->add_client_request_interceptor (tao_interceptor.in ());
      }
      catch_tao_system_ex (_sx)
#else
      X11_UNUSED_ARG (interceptor);
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    void
    ORBInitInfo::add_server_request_interceptor (
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ServerRequestInterceptor>::ref_type interceptor)
    {
#if TAO_HAS_INTERCEPTORS == 1
      if (this->get_sri_proxy_adapter ())
      {
        TAO_PI::ServerRequestInterceptor_ptr tao_interceptor =
            this->get_sri_proxy_adapter ()->create_server_request_interceptor_proxy (
                interceptor);
        sri_proxy_guard _guard (tao_interceptor);
        try
        {
          this->orbii_->add_server_request_interceptor (tao_interceptor);
        }
        catch_tao_system_ex (_sx)
      }
      else
      {
        throw CORBA::INTERNAL ();
      }
#else
      X11_UNUSED_ARG (interceptor);
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    void
    ORBInitInfo::add_ior_interceptor (
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::IORInterceptor>::ref_type interceptor)
    {
#if TAO_HAS_INTERCEPTORS == 1
      if (this->get_iorinterceptor_proxy_adapter ())
      {
        TAO_PI::IORInterceptor_ptr tao_interceptor =
            this->get_iorinterceptor_proxy_adapter ()->create_ior_interceptor_proxy (
                interceptor);
        iori_proxy_guard _guard (tao_interceptor);
        try
        {
          this->orbii_->add_ior_interceptor (tao_interceptor);
        }
        catch_tao_system_ex (_sx)
      }
      else
      {
        throw CORBA::INTERNAL ();
      }
#else
      X11_UNUSED_ARG (interceptor);
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    ::TAOX11_NAMESPACE::PortableInterceptor::SlotId
    ORBInitInfo::allocate_slot_id ()
    {
#if TAO_HAS_INTERCEPTORS == 1
      try
      {
        return this->orbii_->allocate_slot_id ();
      }
      catch_tao_system_ex (_sx)
      return {};
#else
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    void
    ORBInitInfo::register_policy_factory (
        ::TAOX11_NAMESPACE::CORBA::PolicyType /*type*/,
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::PolicyFactory>::ref_type /*policy_factory*/)
    {
      // not supported yet
      // requires extending PolicyFactoryManager(Adapter) with C++11 PolicyFactory registry
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    void
    ORBInitInfo::add_client_request_interceptor_with_policy (
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ClientRequestInterceptor>::ref_type interceptor,
        const CORBA::PolicyList& policies)
    {
#if TAO_HAS_INTERCEPTORS == 1
      TAO_PI::ClientRequestInterceptor_ptr tao_ptr;
      ACE_NEW_THROW_EX (tao_ptr,
                        PortableInterceptor_impl::ClientRequestInterceptor_proxy (
                            interceptor),
                        CORBA::NO_MEMORY (
                          TAO_CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      TAO_PI::ClientRequestInterceptor_var tao_interceptor = tao_ptr;

      TAO_CORBA::PolicyList tao_pols;
      tao_pols.length (TAO_CORBA::ULong (policies.size ()));
      TAO_CORBA::ULong n (0);

      for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
      {
        tao_pols[n++] =
            TAO_CORBA::Policy::_narrow (_pol->get_proxy ()->get ());
      }

      try
      {
        this->orbii_->add_client_request_interceptor_with_policy (tao_interceptor.in (), tao_pols);
      }
      catch_tao_system_ex (_sx)
#else
      X11_UNUSED_ARG (interceptor);
      X11_UNUSED_ARG (policies);
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    void
    ORBInitInfo::add_server_request_interceptor_with_policy (
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::ServerRequestInterceptor>::ref_type interceptor,
        const CORBA::PolicyList& policies)
    {
#if TAO_HAS_INTERCEPTORS == 1
      if (this->get_sri_proxy_adapter ())
      {
        TAO_PI::ServerRequestInterceptor_ptr tao_interceptor =
            this->get_sri_proxy_adapter ()->create_server_request_interceptor_proxy (
                interceptor);
        sri_proxy_guard _guard (tao_interceptor);

        TAO_CORBA::PolicyList tao_pols;
        tao_pols.length (TAO_CORBA::ULong (policies.size ()));
        TAO_CORBA::ULong n (0);

        for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
        {
          tao_pols[n++] =
              TAO_CORBA::Policy::_narrow (_pol->get_proxy ()->get ());
        }

        try
        {
          this->orbii_->add_server_request_interceptor_with_policy (tao_interceptor, tao_pols);
        }
        catch_tao_system_ex (_sx)
      }
      else
      {
        throw CORBA::INTERNAL ();
      }
#else
      X11_UNUSED_ARG (interceptor);
      X11_UNUSED_ARG (policies);
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    void
    ORBInitInfo::add_ior_interceptor_with_policy (
        IDL::traits<::TAOX11_NAMESPACE::PortableInterceptor::IORInterceptor>::ref_type interceptor,
        const CORBA::PolicyList& policies)
    {
#if TAO_HAS_INTERCEPTORS == 1
      if (this->get_iorinterceptor_proxy_adapter ())
      {
        TAO_PI::IORInterceptor_ptr tao_interceptor =
            this->get_iorinterceptor_proxy_adapter ()->create_ior_interceptor_proxy (
                interceptor);
        iori_proxy_guard _guard (tao_interceptor);

        TAO_CORBA::PolicyList tao_pols;
        tao_pols.length (TAO_CORBA::ULong (policies.size ()));
        TAO_CORBA::ULong n (0);

        for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
        {
          tao_pols[n++] =
              TAO_CORBA::Policy::_narrow (_pol->get_proxy ()->get ());
        }

        try
        {
          this->orbii_->add_ior_interceptor_with_policy (tao_interceptor, tao_pols);
        }
        catch_tao_system_ex (_sx)
      }
      else
      {
        throw CORBA::INTERNAL ();
      }
#else
      X11_UNUSED_ARG (interceptor);
      X11_UNUSED_ARG (policies);
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }

    IDL::traits<CORBA::ORB>::ref_type
    ORBInitInfo::_get_orb ()
    {
      try {
        TAO_CORBA::ORB_var _orb = this->orbii_->_get_orb ();
        if (!TAO_CORBA::is_nil (_orb))
        {
          ORB_Proxy_ptr opp = new ORB_Proxy (_orb._retn ());
          return CORBA::make_reference<TAOX11_NAMESPACE::CORBA::ORB> (opp);
        }
        throw CORBA::OBJECT_NOT_EXIST (0, CORBA::COMPLETED_NO);
      }
      catch_tao_system_ex (_sx)
      return {};
    }

  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // ORBInitInfo
    template<>
    object_traits<PortableInterceptor::ORBInitInfo>::ref_type
    object_traits<PortableInterceptor::ORBInitInfo>::narrow (
       object_traits<CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_PI::ORBInitInfo_3_1_ptr const orbiiptr =
              TAO_PI::ORBInitInfo_3_1::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (orbiiptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::ORBInitInfo> (orbiiptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }

    // ORBInitInfo_3_1
    template<>
    object_traits<PortableInterceptor::ORBInitInfo_3_1>::ref_type
    object_traits<PortableInterceptor::ORBInitInfo_3_1>::narrow (
       object_traits<CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_PI::ORBInitInfo_3_1_ptr const orbiiptr =
              TAO_PI::ORBInitInfo_3_1::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (orbiiptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::ORBInitInfo> (orbiiptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }

    // ClientRequestInterceptor
    template<>
    object_traits<PortableInterceptor::ClientRequestInterceptor>::ref_type
    object_traits<PortableInterceptor::ClientRequestInterceptor>::narrow (
       object_traits<CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }
      }

      return {};
    }

    // PolicyFactory
    template<>
    object_traits<PortableInterceptor::PolicyFactory>::ref_type
    object_traits<PortableInterceptor::PolicyFactory>::narrow (
       object_traits<CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }
      }

      return {};
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
