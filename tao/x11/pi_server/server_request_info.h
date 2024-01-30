/**
 * @file    server_request_info.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI ServerRequestInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_CLIENTREQUESTINFO_H_INCLUDED
#define TAOX11_PI_CLIENTREQUESTINFO_H_INCLUDED

#include "tao/x11/pi_server/taox11_pi_server_export.h"
#include "tao/x11/pi_server/tao_pi_server.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/pi_server/PI_Server_includeC.h"
#include "tao/x11/pi/request_info.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PI_Server_Export ServerRequestInfo final
      : public virtual IDL::traits<PortableInterceptor::ServerRequestInfo>::base_type,
        public virtual PortableInterceptor_impl::RequestInfo
    {
    public:
      explicit ServerRequestInfo (TAO_PI::ServerRequestInfo_ptr reqinf);

      virtual ~ServerRequestInfo () = default;

      bool _is_a (const std::string& local_type_id) override;

      TAOX11_NAMESPACE::CORBA::Any sending_exception () override;

      ::TAOX11_NAMESPACE::PortableInterceptor::ServerId server_id () override;

      ::TAOX11_NAMESPACE::PortableInterceptor::ORBId orb_id () override;

      ::TAOX11_NAMESPACE::PortableInterceptor::AdapterName adapter_name () override;

      ::TAOX11_NAMESPACE::PortableInterceptor::ObjectId object_id () override;

      ::TAOX11_NAMESPACE::CORBA::OctetSeq adapter_id () override;

      ::TAOX11_NAMESPACE::CORBA::RepositoryId target_most_derived_interface () override;

      IDL::traits<::TAOX11_NAMESPACE::CORBA::Policy>::ref_type get_server_policy (
          ::TAOX11_NAMESPACE::CORBA::PolicyType type) override;

      void
      set_slot (
          ::TAOX11_NAMESPACE::PortableInterceptor::SlotId id,
          const TAOX11_NAMESPACE::CORBA::Any& data) override;

      bool
      target_is_a (const ::TAOX11_NAMESPACE::CORBA::RepositoryId& id) override;

      void
      add_reply_service_context (
          const ::TAOX11_NAMESPACE::IOP::ServiceContext& service_context,
          bool replace) override;

    private:
      TAO_PI::ServerRequestInfo_ptr reqinf_;

      ServerRequestInfo () = delete;
      ServerRequestInfo (const ServerRequestInfo&) = delete;
      ServerRequestInfo (ServerRequestInfo&&) = delete;
      ServerRequestInfo& operator= (const ServerRequestInfo&) = delete;
      ServerRequestInfo& operator= (ServerRequestInfo&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_CLIENTREQUESTINFO_H_INCLUDED */
