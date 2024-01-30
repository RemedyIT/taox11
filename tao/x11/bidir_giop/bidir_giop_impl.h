/**
 * @file    bidir_giop_impl.h
 * @author  Marcel Smit
 *
 * @brief   x11 CORBA BiDir GIOP Policy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_BIDIR_GIOP_IMPL_H
#define TAOX11_BIDIR_GIOP_IMPL_H

#include "tao/x11/bidir_giop/tao_bidir_giop.h"

#include "tao/x11/objproxy.h"

#include "tao/x11/bidir_giop/bidir_giop.h"

namespace TAOX11_NAMESPACE
{
  namespace BiDirPolicy_impl
  {
    /*--------------------------------------------------------------------
     *
     * BidirectionalPolicy
     *
    --------------------------------------------------------------------*/
    class TAOX11_BIDIRGIOP_Export BidirectionalPolicy final
      : public IDL::traits<BiDirPolicy::BidirectionalPolicy>::base_type
    {
    public:
      BidirectionalPolicy (TAO_BIDIR_GIOP::BidirectionalPolicy_ptr polptr);

      ~BidirectionalPolicy () = default;

      ::TAOX11_NAMESPACE::BiDirPolicy::BidirectionalPolicyValue
      value() override;

    private:
      TAO_BIDIR_GIOP::BidirectionalPolicy_ptr pol_;

      BidirectionalPolicy () = delete;
      BidirectionalPolicy (const BidirectionalPolicy&) = delete;
      BidirectionalPolicy (BidirectionalPolicy&&) = delete;
      BidirectionalPolicy& operator= (const BidirectionalPolicy&) = delete;
      BidirectionalPolicy& operator= (BidirectionalPolicy&&) = delete;
    };
  } // namespace BiDirPolicy_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_BIDIR_GIOP_IMPL_H */
