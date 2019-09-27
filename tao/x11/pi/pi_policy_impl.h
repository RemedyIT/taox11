/**
 * @file    pi_policy_impl.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_PI_POLICY_IMPL_H
#define TAOX11_PI_POLICY_IMPL_H

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/objproxy.h"

#include "tao/x11/pi/pi.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    /*--------------------------------------------------------------------
     *
     * ProcessingModePolicy
     *
    --------------------------------------------------------------------*/
    class TAOX11_PortableInterceptor_Export ProcessingModePolicy final
      : public IDL::traits<PortableInterceptor::ProcessingModePolicy>::base_type
    {
    public:
      explicit ProcessingModePolicy (TAO_PI::ProcessingModePolicy_ptr polptr);

      ~ProcessingModePolicy () = default;

      virtual PortableInterceptor::ProcessingMode processing_mode () override;

    private:
      TAO_PI::ProcessingModePolicy_ptr pol_;

      ProcessingModePolicy () = delete;
      ProcessingModePolicy (const ProcessingModePolicy&) = delete;
      ProcessingModePolicy (ProcessingModePolicy&&) = delete;
      ProcessingModePolicy& operator= (const ProcessingModePolicy&) = delete;
      ProcessingModePolicy& operator= (ProcessingModePolicy&&) = delete;
    };
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_POLICY_IMPL_H */
