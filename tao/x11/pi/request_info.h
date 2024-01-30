/**
 * @file    request_info.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI RequestInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_REQUESTINFO_H_INCLUDED
#define TAOX11_PI_REQUESTINFO_H_INCLUDED

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/pi/PI_includeC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PortableInterceptor_Export RequestInfo
      : public virtual IDL::traits<PortableInterceptor::RequestInfo>::base_type
    {
    public:
      explicit RequestInfo (TAO_PI::RequestInfo_ptr reqinf);

      virtual ~RequestInfo () = default;

      uint32_t request_id () override;

      std::string operation () override;

      ::TAOX11_NAMESPACE::Dynamic::ParameterList arguments () override;

      ::TAOX11_NAMESPACE::Dynamic::ExceptionList exceptions () override;

      ::TAOX11_NAMESPACE::Dynamic::ContextList contexts () override;

      ::TAOX11_NAMESPACE::Dynamic::RequestContext operation_context () override;

      TAOX11_NAMESPACE::CORBA::Any result () override;

      bool response_expected () override;

      ::TAOX11_NAMESPACE::Messaging::SyncScope sync_scope () override;

      ::TAOX11_NAMESPACE::PortableInterceptor::ReplyStatus reply_status () override;

      IDL::traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type forward_reference () override;

      TAOX11_NAMESPACE::CORBA::Any get_slot (
          ::TAOX11_NAMESPACE::PortableInterceptor::SlotId id) override;

      ::TAOX11_NAMESPACE::IOP::ServiceContext get_request_service_context (
          ::TAOX11_NAMESPACE::IOP::ServiceId id) override;

      ::TAOX11_NAMESPACE::IOP::ServiceContext get_reply_service_context (
          ::TAOX11_NAMESPACE::IOP::ServiceId id) override;

    private:
      TAO_PI::RequestInfo_ptr reqinf_;

      RequestInfo () = delete;
      RequestInfo (const RequestInfo&) = delete;
      RequestInfo (RequestInfo&&) = delete;
      RequestInfo& operator= (const RequestInfo&) = delete;
      RequestInfo& operator= (RequestInfo&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_REQUESTINFO_H_INCLUDED */
