/**
 * @file    ior_info.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI IORInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_PI_IORINFO_H_INCLUDED
#define TAOX11_PI_IORINFO_H_INCLUDED

#include "tao/x11/ior_interceptor/taox11_ior_interceptor_export.h"
#include "tao/x11/ior_interceptor/tao_ior_interceptor.h"

#include "tao/x11/ior_interceptor/ior_interceptor.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_IOR_Interceptor_Export IORInfo final
      : public IDL::traits<PortableInterceptor::IORInfo>::base_type
    {
    public:
      explicit IORInfo (TAO_PI::IORInfo_ptr iorinf);

      virtual ~IORInfo () = default;

      IDL::traits<CORBA::Policy>::ref_type
      get_effective_policy (CORBA::PolicyType type) override;

      void
      add_ior_component (const IOP::TaggedComponent& a_component) override;

      void
      add_ior_component_to_profile (
          const IOP::TaggedComponent& a_component,
          IOP::ProfileId profile_id) override;

      PortableInterceptor::AdapterManagerId
      manager_id () override;

      PortableInterceptor::AdapterState
      state () override;

      IDL::traits<PortableInterceptor::ObjectReferenceTemplate>::ref_type
      adapter_template () override;

      IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type
      current_factory () override;

      void
      current_factory (
          IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type _v) override;

    private:
      TAO_PI::IORInfo_ptr iorinf_;

      IORInfo () = delete;
      IORInfo (const IORInfo&) = delete;
      IORInfo (IORInfo&&) = delete;
      IORInfo& operator= (const IORInfo&) = delete;
      IORInfo& operator= (IORInfo&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_IORINFO_H_INCLUDED */
