/**
 * @file    pi_policy_impl.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/pi/pi_policy_impl.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    /*--------------------------------------------------------------------
     *
     * ProcessingModePolicy
     *
    --------------------------------------------------------------------*/
    ProcessingModePolicy::ProcessingModePolicy (
        TAO_PI::ProcessingModePolicy_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }

    PortableInterceptor::ProcessingMode ProcessingModePolicy::processing_mode ()
    {
      return static_cast< PortableInterceptor::ProcessingMode> (
        this->pol_->processing_mode ());
    }
  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // ProcessingModePolicy
    template<>
    object_traits<PortableInterceptor::ProcessingModePolicy>::ref_type
    object_traits<PortableInterceptor::ProcessingModePolicy>::narrow (
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
          TAO_PI::ProcessingModePolicy_ptr const polptr =
              TAO_PI::ProcessingModePolicy::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::ProcessingModePolicy> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
