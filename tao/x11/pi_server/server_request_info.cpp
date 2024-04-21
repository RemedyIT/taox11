/**
 * @file    server_request_info.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI ServerRequestInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// TAO namespace dependencies
#include "tao/x11/base/tao_corba.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/sequence_conversion_t.h"

// X11 namespace dependencies
#include "tao/x11/pi_server/server_request_info.h"

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_util.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    ServerRequestInfo::ServerRequestInfo (
        TAO_PI::ServerRequestInfo_ptr reqinf)
      : PortableInterceptor_impl::RequestInfo (reqinf),
        reqinf_ (reqinf)
    {
      // proxy set by RequestInfo
    }

    bool
    ServerRequestInfo::_is_a (const std::string& local_type_id)
    {
      return IDL::traits<PortableInterceptor::ServerRequestInfo>::base_type::_is_a (local_type_id);
    }

    CORBA::Any
    ServerRequestInfo::sending_exception ()
    {
      CORBA::Any rc {};
      try
      {
        TAO_CORBA::Any_var tao_any = this->reqinf_->sending_exception ();
        Any_Util::tao_any_to_x11_any (tao_any.in (), rc);
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    PortableInterceptor::ServerId
    ServerRequestInfo::server_id ()
    {
      PortableInterceptor::ServerId rc {};
      try
      {
        TAO_PI::ServerId_var tao_id = this->reqinf_->server_id ();
        if (!TAO_CORBA::is_nil (tao_id.in ()))
          rc = tao_id.in ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    PortableInterceptor::ORBId
    ServerRequestInfo::orb_id ()
    {
      PortableInterceptor::ORBId rc {};
      try
      {
        TAO_PI::ORBId_var tao_id = this->reqinf_->orb_id ();
        if (!TAO_CORBA::is_nil (tao_id.in ()))
          rc = tao_id.in ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    PortableInterceptor::AdapterName
    ServerRequestInfo::adapter_name ()
    {
      PortableInterceptor::AdapterName rc {};
      try
      {
        TAO_PI::AdapterName_var tao_name = this->reqinf_->adapter_name ();
        rc = seq_to_x11<PortableInterceptor::AdapterName> (tao_name.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    PortableInterceptor::ObjectId
    ServerRequestInfo::object_id ()
    {
      PortableInterceptor::ObjectId rc {};
      try
      {
        TAO_PI::ObjectId_var tao_id = this->reqinf_->object_id ();
        rc = seq_to_x11<PortableInterceptor::ObjectId> (tao_id.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    CORBA::OctetSeq
    ServerRequestInfo::adapter_id ()
    {
      CORBA::OctetSeq rc {};
      try
      {
        TAO_CORBA::OctetSeq_var tao_id = this->reqinf_->adapter_id ();
        rc = seq_to_x11<CORBA::OctetSeq> (tao_id.in ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    CORBA::RepositoryId
    ServerRequestInfo::target_most_derived_interface ()
    {
      CORBA::RepositoryId rc {};
      try
      {
        TAO_CORBA::RepositoryId_var tao_id = this->reqinf_->target_most_derived_interface ();
        if (!TAO_CORBA::is_nil (tao_id.in ()))
          rc = tao_id.in ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IDL::traits<CORBA::Policy>::ref_type
    ServerRequestInfo::get_server_policy (
        CORBA::PolicyType type)
    {
      IDL::traits<CORBA::Policy>::ref_type rc {};
      try
      {
        TAO_CORBA::Policy_var tao_pol = this->reqinf_->get_server_policy (type);
        rc = CORBA::make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
              new Object_proxy (TAO_CORBA::Policy::_duplicate(tao_pol.in())));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    void
    ServerRequestInfo::set_slot (
        PortableInterceptor::SlotId id,
        const CORBA::Any& data)
    {
      try
      {
        TAO_CORBA::Any tao_data;
        Any_Util::x11_any_to_tao_any (data, tao_data);
        this->reqinf_->set_slot (id, tao_data);
      }
      catch_tao_system_ex (_sx)
    }

    bool
    ServerRequestInfo::target_is_a (
        const CORBA::RepositoryId& id)
    {
      bool rc {};
      try
      {
        rc = this->reqinf_->target_is_a (id.c_str ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    void
    ServerRequestInfo::add_reply_service_context (
        const IOP::ServiceContext& service_context,
        bool replace)
    {
      try
      {
        TAO_IOP::ServiceContext tao_sc;
        tao_sc.context_id = service_context.context_id ();
        seq_from_x11 (tao_sc.context_data, service_context.context_data ());
        this->reqinf_->add_reply_service_context (tao_sc, replace);
      }
      catch_tao_system_ex (_sx)
    }
  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // ORBInitInfo
    template<>
    object_traits<PortableInterceptor::ServerRequestInfo>::ref_type
    object_traits<PortableInterceptor::ServerRequestInfo>::narrow (
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
          TAO_PI::ServerRequestInfo_ptr const riptr =
              TAO_PI::ServerRequestInfo::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (riptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::ServerRequestInfo> (riptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }

    // ServerRequestInterceptor
    template<>
    object_traits<PortableInterceptor::ServerRequestInterceptor>::ref_type
    object_traits<PortableInterceptor::ServerRequestInterceptor>::narrow (
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
