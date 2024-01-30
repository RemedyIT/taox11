/**
 * @file    abstract_base.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 Abstract interface base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/tao_corba.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/CDR.h"

#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_basic_impl_t.h"
#include "tao/x11/valuetype/valuetype_proxies.h"
#include "tao/x11/valuetype/abstractbase_proxy.h"
#include "tao/x11/valuetype/abstract_base.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/anytypecode/any.h"

namespace TAOX11_NAMESPACE
{

  namespace IDL
  {
    std::string traits<CORBA::AbstractBase>::_dump (
        traits<CORBA::AbstractBase>::ref_type obj,
        const char* objtype)
    {
      std::string objstr = "IDL::traits<"; objstr += objtype; objstr += ">::ref_type {abstract:";
      traits<CORBA::Object>::ref_type obj_ref = obj->_to_object();

      if (obj_ref)
      {
        objstr += "object:";
#if (TAO_HAS_MINIMUM_CORBA == 0)
        objstr += obj_ref->_repository_id();
#endif
      }
      else //valuetype
      {
        traits<CORBA::ValueBase>::ref_type val_ref = obj->_to_value();
        if (val_ref)
        {
          objstr += "valuetype:"; objstr += val_ref->_obv_typecode ()->id ();
        }
        else
        {
          objstr += "nil";
        }
      }
      objstr += '}';
      return objstr;
    }
  } // namespace IDL

  namespace CORBA
  {
    AbstractBase::AbstractBase (Abstractbase_proxy_ptr prx)
      : proxy_ (prx)
    {
      this->proxy_->_add_ref ();
    }

    AbstractBase::~AbstractBase ()
    {
      if (this->proxy_ != nullptr)
        this->proxy_->_remove_ref ();
      this->proxy_ = nullptr;
    }

    // Required for derived value types
    // these do not actually copy/move anything
    // as 'proxy_' will always be nil for value types
    // and '_selfref' belongs ONLY to 'this'
    AbstractBase::AbstractBase (const AbstractBase&)
    {}
    AbstractBase::AbstractBase (AbstractBase&&)
    {}

    AbstractBase::_shared_ptr_type AbstractBase::_lock_shared ()
    {
      AbstractBase::_shared_ptr_type absref = this->self_ref_.lock ();
      if (!absref)
      {
        absref = AbstractBase::_shared_ptr_type (this, AbstractBase::dtor ());
        this->self_ref_ = absref;
      }
      return absref;
    }

    IDL::traits<CORBA::Object>::ref_type AbstractBase::_to_object ()
    {
      if (this->proxy_ != nullptr)
      {
          return Object::_narrow (Object_proxy (this->proxy_->_to_object ()));
      }
      return nullptr;
    }

    CORBA::valuetype_reference<CORBA::ValueBase> AbstractBase::_to_value ()
    {
      // should never be called; abstract base references to valuetype instances
      // should always be concrete and than _to_value() will be overloaded.
      return nullptr;
    }

    bool AbstractBase::_is_a (const std::string& local_type_id)
    {
      return local_type_id == this->AbstractBase::_obv_repository_id ();
    }

    std::string AbstractBase::_interface_repository_id () const
    {
      // make sure to always call the base version here
      return this->AbstractBase::_obv_repository_id ();
    }

    bool AbstractBase::_is_object ()
    {
      return (this->proxy_ != nullptr);
    }

    const std::string& AbstractBase::_obv_repository_id () const
    {
      static const std::string _repo_id = "IDL:omg.org/CORBA/AbstractBase:1.0";
      return _repo_id;
    }

    bool AbstractBase::_obv_marshal_v (TAO_OutputCDR &) const
    {
      return false;
    }

    bool AbstractBase::_abs_marshal (TAO_OutputCDR& strm, _ref_type abs)
    {
      TAO_CORBA::Boolean discriminator = true;

      // We marshal a null abstract interface ptr as a discriminator
      // plus null object reference (see CORBA::Object::marshal()
      // and operator << for CORBA::Object).
      if (!abs)
      {
        // Marshal discriminator, then empty type hint.
        strm << ACE_OutputCDR::from_boolean (discriminator);
        return strm << TAO_CORBA::Object::_nil ();
      }

      if (abs->_is_object ())
      {
        // Object

        if (abs->proxy_ == nullptr)
        {
          // derived Object class

          // marshal indicator value
          if (strm << ACE_OutputCDR::from_boolean (discriminator))
          {
            // get concrete object ref and marshal proxy
            return (strm << abs->_to_object ()->get_proxy ()->get ());
          }
        }
        else
        {
          // abstract base (not concrete)

          // just marshal proxy; leave it to TAO
          return (strm << abs->proxy_);
        }
      }
      else
      {
        // Valuetype
        discriminator = false;

        // marshal indicator value
        if (strm << ACE_OutputCDR::from_boolean (discriminator))
          {
            TAO_CORBA::ULong value_tag = TAO_OBV_GIOP_Flags::Value_tag_base
                                         | TAO_OBV_GIOP_Flags::Type_info_single;

            if ((strm.write_ulong (value_tag)) == 0)
              {
                return false;
              }

            if ((strm << abs->_obv_repository_id ()) == 0)
              {
                return false;
              }

            return abs->_obv_marshal_v (strm);
          }
      }

      return false;
    }

    bool AbstractBase::_abs_unmarshal (TAO_InputCDR& strm, _ref_type& new_abs)
    {
      new_abs = nullptr;
      TAO_CORBA::Boolean discriminator = false;
      ACE_InputCDR::to_boolean tb (discriminator);
      TAO_ORB_Core *orb_core = nullptr;

      if (strm >> tb)
      {
        if (!discriminator)
          {
            TAO_CORBA::ULong value_tag;

            if (!strm.read_ulong (value_tag))
              {
                return false;
              }

            if (TAO_OBV_GIOP_Flags::is_null_ref (value_tag))
              {
                // Ok, null reference unmarshaled.
                return true;
              }

            if (!TAO_OBV_GIOP_Flags::is_value_tag (value_tag))
              {
                TAOX11_LOG_DEBUG (ACE_TEXT ("operator>> CORBA::AbstractBase ")
                                  ACE_TEXT ("not value_tag\n"));
                return false;
              }

            TAO_CORBA::String_var repo_id_stream;

            // It would be more efficient not to copy the string)
            if (strm.read_string (repo_id_stream.inout ()) == 0)
              {
                return false;
              }

            orb_core = strm.orb_core ();

            if (orb_core == 0)
              {
                orb_core = TAO_ORB_Core_instance ();

                TAOX11_LOG_DEBUG (ACE_TEXT ("TAO (%P|%t) WARNING: extracting ")
                                  ACE_TEXT ("valuetype using default ORB_Core\n"));
              }

            TAO_CORBA::ValueFactory_var factory =
              orb_core->orb ()->lookup_value_factory (repo_id_stream.in ());
            // should be an X11 factory proxy
            ValueFactory_proxy* factory_impl =
                dynamic_cast<ValueFactory_proxy*> (factory.in ());

            // We should throw an exception, if there were an appropriate one.
            if (factory.in() == nullptr || factory_impl == nullptr)
              {
                TAOX11_LOG_DEBUG (ACE_TEXT("(")
                                  << x11_logger::processid
                                  << ACE_TEXT ("|")
                                  << x11_logger::threadid
                                  << ACE_TEXT (") - The following unknown type was received: ")
                                  << repo_id_stream.in ());
                return false;
              }

            CORBA::ValueBase::_ref_type new_vt = factory_impl->get_factory ()->create_for_unmarshal ();
            if (new_vt->_obv_unmarshal_v (strm))
            {
              new_abs = new_vt->_obv_abstract_base ();
              return true;
            }
            return false;
          }
          else
          {
            TAO_CORBA::Object_var generic_objref;

            if (strm >> generic_objref.inout ())
              {
                if (TAO_CORBA::is_nil (generic_objref.in ()))
                  {
                    return true;
                  }

                TAO_Stub *concrete_stubobj = generic_objref->_stubobj ();

                bool const collocated =
                  !TAO_CORBA::is_nil (concrete_stubobj->servant_orb_var ().in ())
                  && concrete_stubobj->optimize_collocation_objects ()
                  && generic_objref->_is_collocated ();

                Abstractbase_proxy_var new_absprx; // keep refcount safe
                ACE_NEW_RETURN (new_absprx,
                                Abstractbase_proxy (
                                  concrete_stubobj,
                                  collocated,
                                  generic_objref->_servant ()),
                                false);
                // AbstractBase will duplicate proxy; so use in() to have var remove extra refcount
                new_abs =
                    AbstractBase::_ref_type (new AbstractBase (new_absprx.in ()));
                return true;
              }
          }
        }

      return false;
    }

    CORBA::typecode_reference const _tc_AbstractBase = CORBA::make_reference<CORBA::TypeCode_impl> (TAO_CORBA::_tc_AbstractBase, true);

  } // namespace CORBA

  void
  operator<<= (CORBA::Any& any, TAOX11_CORBA::AbstractBase::_ref_type abs)
  {
    Any_AbstractBase_Impl_T<IDL::traits<CORBA::AbstractBase>,
                            CDR_Marshal_Policy>::insert (any, CORBA::_tc_AbstractBase, abs);
  }

  bool
  operator>>= (const CORBA::Any& any, TAOX11_CORBA::AbstractBase::_ref_type &abs)
  {
    Any_Impl::ref_type impl = any.impl ();
    return impl->to_abstract_base (abs);
  }

} // namespace TAOX11_NAMESPACE

