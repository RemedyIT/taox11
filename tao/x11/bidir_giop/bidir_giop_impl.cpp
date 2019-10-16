/**
 * @file    bidir_giop_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   x11 CORBA BiDir GIOP Policy
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/bidir_giop/bidir_giop_impl.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace BiDirPolicy_impl
  {

    BidirectionalPolicy::BidirectionalPolicy (
        TAO_BIDIR_GIOP::BidirectionalPolicy_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }

    ::TAOX11_NAMESPACE::BiDirPolicy::BidirectionalPolicyValue
    BidirectionalPolicy::value()
    {
      return static_cast< ::TAOX11_NAMESPACE::BiDirPolicy::BidirectionalPolicyValue> (
        this->pol_->value ());
    }
  }

  namespace CORBA
  {
    // BidirectionalPolicy
    template<>
    TAOX11_BIDIRGIOP_Export object_traits< ::TAOX11_NAMESPACE::BiDirPolicy::BidirectionalPolicy>::ref_type
    object_traits< ::TAOX11_NAMESPACE::BiDirPolicy::BidirectionalPolicy>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_BIDIR_GIOP::BidirectionalPolicy_ptr const polptr =
            TAO_BIDIR_GIOP::BidirectionalPolicy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<BiDirPolicy_impl::BidirectionalPolicy> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }
  }
}

