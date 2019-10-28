/**
 * @file    ior_interceptor_proxy.cpp
 * @author  Martin Corino
 *
 * @brief   TAO PI IORInterceptor proxy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// TAO dependencies
#include "tao/x11/tao_corba.h"
#include "tao/x11/pi/tao_pi.h"
#include "tao/x11/ior_interceptor/tao_ior_interceptor.h"
#include "tao/ObjRefTemplate/ObjectReferenceTemplate.h"

// TAOX11 dependencies
#include "tao/x11/ort/object_reference_template_impl.h"

#include "tao/x11/ior_interceptor/ior_interceptor_proxy.h"

#include "tao/x11/ior_interceptor/ior_info.h"

#include "tao/x11/user_exception_proxy_out.h"

#if TAO_HAS_INTERCEPTORS == 1

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    IORInterceptor_proxy::IORInterceptor_proxy (
        IDL::traits<PortableInterceptor::IORInterceptor>::ref_type x11_interceptor)
      : PortableInterceptor_impl::Interceptor_proxy (x11_interceptor),
        TAO_PI::IORInterceptor ()
    {
      // making sure to move AFTER ctor Interceptor_proxy
      this->x11_interceptor_ = std::move (x11_interceptor);
    }

    void
    IORInterceptor_proxy::establish_components (
        TAO_PI::IORInfo_ptr info)
    {
      try
      {
        IDL::traits<PortableInterceptor::IORInfo>::ref_type x11_info =
            CORBA::make_reference<PortableInterceptor_impl::IORInfo> (
                TAO_PI::IORInfo::_duplicate(info));
        this->x11_interceptor_->establish_components (x11_info);
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }
    void
    IORInterceptor_proxy::components_established (
        TAO_PI::IORInfo_ptr info)
    {
      try
      {
        IDL::traits<PortableInterceptor::IORInterceptor_3_0>::ref_type x11_v3_interceptor =
            IDL::traits<PortableInterceptor::IORInterceptor_3_0>::narrow (this->x11_interceptor_);
        if (x11_v3_interceptor)
        {
          IDL::traits<PortableInterceptor::IORInfo>::ref_type x11_info =
              CORBA::make_reference<PortableInterceptor_impl::IORInfo> (
                  TAO_PI::IORInfo::_duplicate(info));
          x11_v3_interceptor->components_established (x11_info);
        }
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }

    void
    IORInterceptor_proxy::adapter_manager_state_changed (
        const char * id,
        TAO_PI::AdapterState state)
    {
      try
      {
        IDL::traits<PortableInterceptor::IORInterceptor_3_0>::ref_type x11_v3_interceptor =
            IDL::traits<PortableInterceptor::IORInterceptor_3_0>::narrow (this->x11_interceptor_);
        if (x11_v3_interceptor)
        {
          x11_v3_interceptor->adapter_manager_state_changed (
              (id ? id : std::string()),
              state);
        }
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }

    /**
     * Conversion type for IOP::TaggedComponent
     */
    namespace {
      struct ObjectReferenceTemplateToX11
      {
        explicit ObjectReferenceTemplateToX11 (TAO_PI::ObjectReferenceTemplate* tao_ort)
          : tao_ort_ (tao_ort)
        {}
        ~ObjectReferenceTemplateToX11 () = default;

        operator IDL::traits<PortableInterceptor::ObjectReferenceTemplate>::ref_type () const
        {
          TAO_CORBA::add_ref (this->tao_ort_);
          return CORBA::make_reference<PortableInterceptor_impl::ObjectReferenceTemplate_impl> (
              this->tao_ort_);
        }

        TAO_PI::ObjectReferenceTemplate* tao_ort_ {};
      };
    }

    void
    IORInterceptor_proxy::adapter_state_changed (
        const TAO_PI::ObjectReferenceTemplateSeq& templates,
        TAO_PI::AdapterState state)
    {
      try
      {
        IDL::traits<PortableInterceptor::IORInterceptor_3_0>::ref_type x11_v3_interceptor =
            IDL::traits<PortableInterceptor::IORInterceptor_3_0>::narrow (this->x11_interceptor_);
        if (x11_v3_interceptor)
        {
          x11_v3_interceptor->adapter_state_changed (
              convert_seq_to_x11<
                  PortableInterceptor::ObjectReferenceTemplateSeq,
                  ObjectReferenceTemplateToX11> (templates),
              state);
        }
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
      catch (...)
      {
        throw TAO_CORBA::INTERNAL ();
      }
    }
  } // namespace PortableInterceptor_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAO_HAS_INTERCEPTORS == 1 */
