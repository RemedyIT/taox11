/**
 * @file    request_info.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI RequestInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// TAO namespace dependencies
#include "tao/x11/tao_corba.h"
#include "tao/x11/objproxy.h"
#include "tao/AnyTypeCode/DynamicC.h"

#define TAO_DYN \
  TAO_VERSIONED_NAMESPACE_NAME::Dynamic

// X11 namespace dependencies
#include "tao/x11/pi/request_info.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_util.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    RequestInfo::RequestInfo (
        TAO_PI::RequestInfo_ptr reqinf)
      : reqinf_ (reqinf)
    {
      this->set_proxy (new Object_proxy (reqinf));
    }

    uint32_t
    RequestInfo::request_id ()
    {
      uint32_t rc {};
      try
      {
        rc = this->reqinf_->request_id ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    std::string
    RequestInfo::operation ()
    {
      std::string rc {};
      try
      {
        TAO_CORBA::String_var tao_str = this->reqinf_->operation ();
        if (!TAO_CORBA::is_nil (tao_str.in ()))
          rc = tao_str.in ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    /**
     * Conversion type for Dynamic::Parameter
     */
    namespace {
      struct DynParamToX11
      {
        explicit DynParamToX11 (const TAO_DYN::Parameter& tao_param)
          : tao_param_ (tao_param)
        {}
        ~DynParamToX11 () = default;

        operator Dynamic::Parameter () const
        {
          CORBA::Any x11_any;
          Any_Util::tao_any_to_x11_any (this->tao_param_.argument, x11_any);
          return Dynamic::Parameter
              (x11_any,
               static_cast<CORBA::ParameterMode> (this->tao_param_.mode));
        }

        const TAO_DYN::Parameter& tao_param_;
      };
    }

    ::TAOX11_NAMESPACE::Dynamic::ParameterList
    RequestInfo::arguments ()
    {
      Dynamic::ParameterList rc {};
      try
      {
        TAO_DYN::ParameterList_var tao_pl = this->reqinf_->arguments ();
        rc = convert_seq_to_x11<Dynamic::ParameterList, DynParamToX11> (tao_pl.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    /**
     * Conversion type for TAO TypeCode
     */
    namespace {
      struct TypeCodeToX11
      {
        explicit TypeCodeToX11 (TAO_CORBA::TypeCode_ptr tao_tc)
          : tao_tc_ (tao_tc)
        {}
        ~TypeCodeToX11 () = default;

        operator IDL::traits<CORBA::TypeCode>::ref_type () const
        {
          return CORBA::make_reference<CORBA::TypeCode_impl> (
                    TAO_CORBA::TypeCode::_duplicate(this->tao_tc_));
        }

        TAO_CORBA::TypeCode_ptr tao_tc_ {};
      };
    }

    Dynamic::ExceptionList
    RequestInfo::exceptions ()
    {
      Dynamic::ExceptionList rc {};
      try
      {
        TAO_DYN::ExceptionList_var tao_exl = this->reqinf_->exceptions ();
        rc = convert_seq_to_x11<Dynamic::ExceptionList, TypeCodeToX11> (tao_exl.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    Dynamic::ContextList
    RequestInfo::contexts ()
    {
      Dynamic::ContextList rc {};
      try
      {
        TAO_DYN::ContextList_var tao_ctxl = this->reqinf_->contexts ();
        rc = seq_to_x11<Dynamic::ContextList> (tao_ctxl.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    Dynamic::RequestContext
    RequestInfo::operation_context ()
    {
      Dynamic::RequestContext rc {};
      try
      {
        TAO_DYN::RequestContext_var tao_rc = this->reqinf_->contexts ();
        rc = seq_to_x11<Dynamic::RequestContext> (tao_rc.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    CORBA::Any
    RequestInfo::result ()
    {
      CORBA::Any rc {};
      try
      {
        TAO_CORBA::Any_var tao_any = this->reqinf_->result ();
        Any_Util::tao_any_to_x11_any (tao_any.in (), rc);
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    bool
    RequestInfo::response_expected ()
    {
      bool rc {};
      try
      {
        rc = this->reqinf_->response_expected ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    Messaging::SyncScope
    RequestInfo::sync_scope ()
    {
      Messaging::SyncScope rc {};
      try
      {
        rc = this->reqinf_->sync_scope ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    PortableInterceptor::ReplyStatus
    RequestInfo::reply_status ()
    {
      PortableInterceptor::ReplyStatus rc {};
      try
      {
        rc = this->reqinf_->reply_status ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IDL::traits<CORBA::Object>::ref_type
    RequestInfo::forward_reference ()
    {
      IDL::traits<CORBA::Object>::ref_type rc {};
      try
      {
        TAO_CORBA::Object_var tao_obj = this->reqinf_->forward_reference ();
        if (!TAO_CORBA::is_nil (tao_obj.in ()))
          rc = CORBA::make_reference<CORBA::Object> (new Object_proxy (tao_obj._retn ()));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    TAOX11_NAMESPACE::CORBA::Any
    RequestInfo::get_slot (
        ::TAOX11_NAMESPACE::PortableInterceptor::SlotId id)
    {
      CORBA::Any rc {};
      try
      {
        TAO_CORBA::Any_var tao_any = this->reqinf_->get_slot (id);
        Any_Util::tao_any_to_x11_any (tao_any.in (), rc);
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IOP::ServiceContext
    RequestInfo::get_request_service_context (
        IOP::ServiceId id)
    {
      IOP::ServiceContext rc {};
      try
      {
        TAO_IOP::ServiceContext_var tao_sc =
            this->reqinf_->get_request_service_context (id);
        rc = IOP::ServiceContext (
              tao_sc->context_id,
              seq_to_x11<CORBA::OctetSeq> (tao_sc->context_data));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IOP::ServiceContext
     RequestInfo::get_reply_service_context (
        IOP::ServiceId id)
    {
      IOP::ServiceContext rc {};
      try
      {
        TAO_IOP::ServiceContext_var tao_sc =
            this->reqinf_->get_reply_service_context (id);
        rc = IOP::ServiceContext (
              tao_sc->context_id,
              seq_to_x11<CORBA::OctetSeq> (tao_sc->context_data));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // ORBInitInfo
    template<>
    object_traits<PortableInterceptor::RequestInfo>::ref_type
    object_traits<PortableInterceptor::RequestInfo>::narrow (
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
          TAO_PI::RequestInfo_ptr const riptr =
              TAO_PI::RequestInfo::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (riptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::RequestInfo> (riptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
