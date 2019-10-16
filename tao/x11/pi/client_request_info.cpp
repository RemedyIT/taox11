/**
 * @file    client_request_info.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI ClientRequestInfo implementation
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
#include "tao/x11/pi/client_request_info.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_util.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    ClientRequestInfo::ClientRequestInfo (
        TAO_PI::ClientRequestInfo_ptr reqinf)
      : PortableInterceptor_impl::RequestInfo (reqinf),
        reqinf_ (reqinf)
    {
      // proxy set by RequestInfo
    }

    bool
    ClientRequestInfo::_is_a (const std::string& local_type_id)
    {
      return IDL::traits<PortableInterceptor::ClientRequestInfo>::base_type::_is_a (local_type_id);
    }

    IDL::traits<CORBA::Object>::ref_type
    ClientRequestInfo::target ()
    {
      IDL::traits<CORBA::Object>::ref_type rc {};
      try
      {
        TAO_CORBA::Object_var tao_obj = this->reqinf_->target ();
        if (!TAO_CORBA::is_nil (tao_obj.in ()))
          rc = CORBA::make_reference<CORBA::Object> (new Object_proxy (tao_obj._retn ()));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IDL::traits<CORBA::Object>::ref_type
    ClientRequestInfo::effective_target ()
    {
      IDL::traits<CORBA::Object>::ref_type rc {};
      try
      {
        TAO_CORBA::Object_var tao_obj = this->reqinf_->effective_target ();
        if (!TAO_CORBA::is_nil (tao_obj.in ()))
          rc = CORBA::make_reference<CORBA::Object> (new Object_proxy (tao_obj._retn ()));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IOP::TaggedProfile
    ClientRequestInfo::effective_profile ()
    {
      IOP::TaggedProfile rc {};
      try
      {
        TAO_IOP::TaggedProfile_var tao_tp =
            this->reqinf_->effective_profile ();
        rc = IOP::TaggedProfile (
              tao_tp->tag,
              seq_to_x11<CORBA::OctetSeq> (tao_tp->profile_data));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    CORBA::Any
    ClientRequestInfo::received_exception ()
    {
      CORBA::Any rc {};
      try
      {
        TAO_CORBA::Any_var tao_any = this->reqinf_->received_exception ();
        Any_Util::tao_any_to_x11_any (tao_any.in (), rc);
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    CORBA::RepositoryId
    ClientRequestInfo::received_exception_id ()
    {
      CORBA::RepositoryId rc {};
      try
      {
        TAO_CORBA::RepositoryId_var tao_id = this->reqinf_->received_exception_id ();
        if (!TAO_CORBA::is_nil(tao_id.in ()))
          rc = tao_id.in ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    /**
     * Conversion type for IOP::TaggedComponent
     */
    namespace {
      struct IOPTaggedComponentToX11
      {
        explicit IOPTaggedComponentToX11 (const TAO_IOP::TaggedComponent& tao_tc)
          : tao_tc_ (tao_tc)
        {}
        ~IOPTaggedComponentToX11 () = default;

        operator IOP::TaggedComponent () const
        {
          return IOP::TaggedComponent
              (this->tao_tc_.tag,
               seq_to_x11<CORBA::OctetSeq> (this->tao_tc_.component_data));
        }

        const TAO_IOP::TaggedComponent& tao_tc_;
      };
    }

    IOP::TaggedComponent
    ClientRequestInfo::get_effective_component (
        IOP::ComponentId id)
    {
      IOP::TaggedComponent rc {};
      try
      {
        TAO_IOP::TaggedComponent_var tao_tc =
            this->reqinf_->get_effective_component (id);
        rc = IOPTaggedComponentToX11 (tao_tc.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IOP::TaggedComponentSeq
    ClientRequestInfo::get_effective_components (
        IOP::ComponentId id)
    {
      IOP::TaggedComponentSeq rc {};
      try
      {
        TAO_IOP::TaggedComponentSeq_var tao_tcseq =
            this->reqinf_->get_effective_components (id);
        rc = convert_seq_to_x11<IOP::TaggedComponentSeq, IOPTaggedComponentToX11> (
                tao_tcseq.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IDL::traits<CORBA::Policy>::ref_type
    ClientRequestInfo::get_request_policy (
        CORBA::PolicyType type)
    {
      IDL::traits<CORBA::Policy>::ref_type rc {};
      try
      {
        TAO_CORBA::Policy_var tao_pol = this->reqinf_->get_request_policy (type);
        rc = CORBA::make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
              new Object_proxy (TAO_CORBA::Policy::_duplicate(tao_pol.in())));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    void
    ClientRequestInfo::add_request_service_context (
        const IOP::ServiceContext& service_context,
        bool replace)
    {
      try
      {
        TAO_IOP::ServiceContext tao_sc;
        tao_sc.context_id = service_context.context_id ();
        seq_from_x11 (tao_sc.context_data, service_context.context_data ());
        this->reqinf_->add_request_service_context (tao_sc, replace);
      }
      catch_tao_system_ex (_sx)
    }
  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // ClientRequestInfo
    template<>
    object_traits<PortableInterceptor::ClientRequestInfo>::ref_type
    object_traits<PortableInterceptor::ClientRequestInfo>::narrow (
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
          TAO_PI::ClientRequestInfo_ptr const riptr =
              TAO_PI::ClientRequestInfo::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (riptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::ClientRequestInfo> (riptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
