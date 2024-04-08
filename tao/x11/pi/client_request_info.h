/**
 * @file    client_request_info.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI ClientRequestInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_CLIENTREQUESTINFO_H_INCLUDED
#define TAOX11_PI_CLIENTREQUESTINFO_H_INCLUDED

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/pi/PI_includeC.h"
#include "tao/x11/pi/request_info.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PortableInterceptor_Export ClientRequestInfo final
      : public virtual IDL::traits<PortableInterceptor::ClientRequestInfo>::base_type,
        public virtual PortableInterceptor_impl::RequestInfo
    {
    public:
      explicit ClientRequestInfo (TAO_PI::ClientRequestInfo_ptr reqinf);

      virtual ~ClientRequestInfo () = default;

      IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type
      target () override;

      bool
      _is_a (const std::string& local_type_id) override;

      IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type
      effective_target () override;

      ::TAOX11_NAMESPACE::IOP::TaggedProfile
      effective_profile () override;

      TAOX11_NAMESPACE::CORBA::Any
      received_exception () override;

      ::TAOX11_NAMESPACE::CORBA::RepositoryId
      received_exception_id () override;

      ::TAOX11_NAMESPACE::IOP::TaggedComponent
      get_effective_component (
          ::TAOX11_NAMESPACE::IOP::ComponentId id) override;

      ::TAOX11_NAMESPACE::IOP::TaggedComponentSeq
      get_effective_components (
          ::TAOX11_NAMESPACE::IOP::ComponentId id) override;

      IDL::traits<::TAOX11_NAMESPACE::CORBA::Policy>::ref_type
      get_request_policy (
          ::TAOX11_NAMESPACE::CORBA::PolicyType type) override;

      void
      add_request_service_context (
          const ::TAOX11_NAMESPACE::IOP::ServiceContext& service_context,
          bool replace) override;

    private:
      TAO_PI::ClientRequestInfo_ptr reqinf_;

      ClientRequestInfo () = delete;
      ClientRequestInfo (const ClientRequestInfo&) = delete;
      ClientRequestInfo (ClientRequestInfo&&) = delete;
      ClientRequestInfo& operator= (const ClientRequestInfo&) = delete;
      ClientRequestInfo& operator= (ClientRequestInfo&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_CLIENTREQUESTINFO_H_INCLUDED */
