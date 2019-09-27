/**
 * @file    pi_current.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PICurrent implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// TAO namespace dependencies
#include "tao/x11/orbproxy.h"

// X11 namespace dependencies
#include "tao/x11/pi/pi_current.h"

#include "tao/x11/orb.h"

#include "tao/x11/anytypecode/any_util.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    PICurrent::PICurrent (
        TAO_PI::Current_ptr pic)
      : pic_ (pic)
    {
      this->set_proxy (new Object_proxy (pic));
    }

    CORBA::Any
    PICurrent::get_slot (PortableInterceptor::SlotId id)
    {
      try
      {
        TAO_CORBA::Any_var tao_any = this->pic_->get_slot (id);
        if (!TAO_CORBA::is_nil (tao_any) && tao_any->type ()->kind () != TAO_CORBA::tk_null)
        {
          CORBA::Any x11_any;
          if (Any_Util::tao_any_to_x11_any (tao_any.in (), x11_any))
          {
            return x11_any;
          }
        }
      }
      catch (const TAO_PI::InvalidSlot&)
      {
        throw PortableInterceptor::InvalidSlot ();
      }
      catch_tao_system_ex (_sx)
      return {};
    }

    void
    PICurrent::set_slot (PortableInterceptor::SlotId identifier,
                         const CORBA::Any & data)
    {
      try
      {
        TAO_CORBA::Any tao_any;
        if (Any_Util::x11_any_to_tao_any (data, tao_any))
        {
          this->pic_->set_slot (identifier, tao_any);
        }
      }
      catch (const TAO_PI::InvalidSlot&)
      {
        throw PortableInterceptor::InvalidSlot ();
      }
      catch_tao_system_ex (_sx)
    }

    IDL::traits<CORBA::ORB>::ref_type
    PICurrent::_get_orb ()
    {
      try
      {
        TAO_CORBA::ORB_var _orb = this->pic_->_get_orb ();
        if (!TAO_CORBA::is_nil (_orb))
        {
          ORB_Proxy_ptr opp = new ORB_Proxy (_orb._retn ());
          return CORBA::make_reference<TAOX11_NAMESPACE::CORBA::ORB> (opp);
        }
        throw CORBA::OBJECT_NOT_EXIST (0, CORBA::COMPLETED_NO);
      }
      catch_tao_system_ex (_sx)
      return {};
    }
  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // ORBInitInfo
    template<>
    object_traits<PortableInterceptor::Current>::ref_type
    object_traits<PortableInterceptor::Current>::narrow (
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
          TAO_PI::Current_var const picptr =
              TAO_PI::Current::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (picptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::PICurrent> (
                TAO_PI::Current::_duplicate (picptr.in ()));
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
