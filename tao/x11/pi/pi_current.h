/**
 * @file    pi_current.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PICurrent implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_PI_CURRENT_H_INCLUDED
#define TAOX11_PI_CURRENT_H_INCLUDED

#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/pi/tao_pi.h"

#include "tao/x11/objproxy.h"
#include "tao/x11/pi/PI_includeC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_PortableInterceptor_Export PICurrent final
      : public IDL::traits<PortableInterceptor::Current>::base_type
    {
    public:
      explicit PICurrent (TAO_PI::Current_ptr pic);

      virtual ~PICurrent () = default;

      virtual
      CORBA::Any
      get_slot (PortableInterceptor::SlotId id) override;

      virtual
      void
      set_slot (PortableInterceptor::SlotId identifier,
                             const CORBA::Any & data) override;

      virtual
      IDL::traits<CORBA::ORB>::ref_type
      _get_orb () override;

    private:
      TAO_PI::Current_ptr pic_;

      PICurrent () = delete;
      PICurrent (const PICurrent&) = delete;
      PICurrent (PICurrent&&) = delete;
      PICurrent& operator= (const PICurrent&) = delete;
      PICurrent& operator= (PICurrent&&) = delete;
    };
  } // namespace PortableInterceptor_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_PI_CURRENT_H_INCLUDED */
