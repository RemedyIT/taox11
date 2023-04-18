/**
 * @file    dynany_impl.cpp
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 wrapper for the TAO DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/CDR.h"

#include "tao/x11/dynamic_any/dynany_impl.h"
#include "tao/x11/dynamic_any/dynanyfactory.h"
#include "tao/x11/log.h"
#include "tao/x11/dynamic_any/dynanyutils_t.h"
#include "tao/x11/anytypecode/any_unknown_type.h"

#include "ace/OS_NS_string.h"
#include "ace/Malloc_Allocator.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    namespace
    {
      /// Special allocator to force allocating zero
      /// initialized memory.
      class ZeroMem_Allocator : public ACE_New_Allocator
      {
      public:
        void *malloc (size_t nbytes) override
        {
          // reroute to calloc for zero init
          return this->calloc (nbytes);
        }
      };
    }

    DynAny_i::DynAny_i (bool allow_truncation)
      : TAOX11_DynCommon (allow_truncation)
    {
      TAOX11_LOG_TRACE ("DynAny_i::DynAny_i");
    }

    void
    DynAny_i::init_common ()
    {
      TAOX11_LOG_TRACE ("DynAny_i::init_common");
      this->ref_to_component_ = false;
      this->container_is_destroying_ = false;
      this->has_components_ = false;
      this->destroyed_ = false;
      this->current_position_ = -1;
      this->component_count_ = 0;
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynAny_i::init (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_LOG_TRACE ("DynAny_i::init with tc");
      this->check_typecode (tc);

      this->set_to_default_value (tc);

      this->init_common ();

      this->type_ = tc;

      return this->_this();
    }

    IDL::traits<DynamicAny::DynAny>::ref_type
    DynAny_i::init (const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE ("DynAny_i::init with any");

      this->type_ = any.type ();

      this->check_typecode (this->type_);

      this->init_common ();

      this->any_ = any;

      return this->_this();
    }

    void
    DynAny_i::from_any ( const CORBA::Any& any)
    {
      TAOX11_LOG_TRACE ("DynAny_i::from_any");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }

      IDL::traits<CORBA::TypeCode>::ref_type any_tc = any.type ();

      if (!this->type_->equivalent (any_tc))
      {
       throw DynamicAny::DynAny::TypeMismatch ();
      }
      this->any_ = any;
    }

    CORBA::Any
    DynAny_i:: to_any ()
    {
      TAOX11_LOG_TRACE ("DynAny_i::to_any");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      return this->any_;
    }

    bool
    DynAny_i::equal (IDL::traits<DynAny>::ref_type dyn_any)
    {
      TAOX11_LOG_TRACE ("DynAny_i::equal");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      IDL::traits<DynAny_i>::ref_type rhs_n = IDL::traits<DynAny_i>::narrow (dyn_any);
      if (rhs_n == nullptr)
      {
        return false;
      }
      if (!this->type_->equivalent (rhs_n->type_))
      {
        TAOX11_LOG_TRACE ("DynAny_i::equal- a");
        return false;
      }
      CORBA::TCKind tk = DynamicAny::DynAnyFactory_i::unalias (this->type_);
      switch (tk)
      {
        case CORBA::TCKind::tk_null:
        case CORBA::TCKind::tk_void:
          return true;
        case CORBA::TCKind::tk_short:
          {
            int16_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            int16_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_long:
          {
            int32_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            int32_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_ushort:
          {
            uint16_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            uint16_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_ulong:
          {
            uint32_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            uint32_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_float:
          {
            float rhs_v;
            rhs_n->any_ >>= rhs_v;
            float lhs_v;
            this->any_ >>= lhs_v;
            return ACE::is_equal (lhs_v, rhs_v);
          }
        case CORBA::TCKind::tk_double:
          {
            double rhs_v;
            rhs_n->any_ >>= rhs_v;
            double lhs_v;
            this->any_ >>= lhs_v;
            return ACE::is_equal (lhs_v, rhs_v);
          }
        case CORBA::TCKind::tk_longdouble:
          {
            long double rhs_v;
            rhs_n->any_ >>= rhs_v;
            long double lhs_v;
            this->any_ >>= lhs_v;
            return ACE::is_equal (lhs_v, rhs_v);
          }
        case CORBA::TCKind::tk_longlong:
          {
            int64_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            int64_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_ulonglong:
          {
            uint64_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            uint64_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_boolean:
          {
            bool rhs_v;
            rhs_n->any_ >>= rhs_v;
            bool lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_char:
          {
            char rhs_v;
            rhs_n->any_ >>= rhs_v;
            char lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_wchar:
          {
            wchar_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            wchar_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_octet:
          {
            uint8_t rhs_v;
            rhs_n->any_ >>= rhs_v;
            uint8_t lhs_v;
            this->any_ >>= lhs_v;
            return (lhs_v == rhs_v);
          }
        case CORBA::TCKind::tk_any:
          {
            CORBA::Any rhs_v;
            rhs_n->any_ >>= rhs_v;
            CORBA::Any lhs_v;
            this->any_ >>= lhs_v;

            IDL::traits<DynamicAny::DynAny>::ref_type rhs_dyn =
                MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
                rhs_v.type (),
                rhs_v,
                this->allow_truncation_ );

            IDL::traits<DynamicAny::DynAny>::ref_type lhs_dyn =
              MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
                lhs_v.type(),
                lhs_v,
                this->allow_truncation_ );

            bool const b = rhs_dyn->equal (lhs_dyn);

            rhs_dyn->destroy ();

            lhs_dyn->destroy ();

            return b;
          }
        case CORBA::TCKind::tk_TypeCode:
          {
            IDL::traits<CORBA::TypeCode>::ref_type rhs_v;
            rhs_n->any_ >>= rhs_v;
            IDL::traits<CORBA::TypeCode>::ref_type lhs_v;
            this->any_ >>= lhs_v;
            // See CORBA 3.2 - must use equal() here.
            return lhs_v->equal (lhs_v);
          }
        case CORBA::TCKind::tk_objref:
          {
            IDL::traits<CORBA::Object>::ref_type rhs_v;
            rhs_n->any_ >>= rhs_v;
            IDL::traits<CORBA::Object>::ref_type lhs_v;
            this->any_ >>= lhs_v;
            return lhs_v->_is_equivalent (lhs_v);
          }
        case CORBA::TCKind::tk_abstract_interface:
          {
            IDL::traits<CORBA::AbstractBase>::ref_type rhs_v;
            rhs_n->any_ >>= rhs_v;
            IDL::traits<CORBA::AbstractBase>::ref_type lhs_v;
            this->any_ >>= lhs_v;
            if (bool(rhs_v) == bool(lhs_v))
            {
              if (rhs_v)
              {
                // same type (derived interface or value type)
                if (rhs_v->_interface_repository_id () == lhs_v->_interface_repository_id ())
                {
                  if (rhs_v->_interface_repository_id () == "IDL:omg.org/CORBA/AbstractBase:1.0")
                  {
                    // both value type derivatives
                    IDL::traits<CORBA::ValueBase>::ref_type rhs_vt =
                        rhs_v->_to_value ();
                    IDL::traits<CORBA::ValueBase>::ref_type lhs_vt =
                        lhs_v->_to_value ();
                    if (rhs_vt->_obv_typecode ()->equal (lhs_vt->_obv_typecode ()))
                    {
                      ZeroMem_Allocator zm_alloc;
                      // somewhat crude but I cannot think of another way
                      // by using ZeroMem_Allocator we force zero initialized
                      // buffer memory so we can compare CDR streams
                      TAO_OutputCDR rhs_out (size_t(0), ACE_CDR_BYTE_ORDER, &zm_alloc);
                      CORBA::AbstractBase::_abs_marshal (rhs_out, rhs_v);
                      rhs_out.consolidate();
                      TAO_OutputCDR lhs_out (size_t(0), ACE_CDR_BYTE_ORDER, &zm_alloc);
                      CORBA::AbstractBase::_abs_marshal (lhs_out, lhs_v);
                      lhs_out.consolidate();

                      return (rhs_out.length () == lhs_out.length ()) &&
                          (ACE_OS::memcmp (rhs_out.buffer (), lhs_out.buffer (), rhs_out.length ()) == 0);
                    }
                  }
                  else
                  {
                    // both derived interfaces
                    IDL::traits<CORBA::Object>::ref_type rhs_vo =
                        rhs_v->_to_object ();
                    IDL::traits<CORBA::Object>::ref_type lhs_vo =
                        lhs_v->_to_object ();
                    return lhs_vo->_is_equivalent (lhs_vo);
                  }
                }
              }
              else
              {
                return true; // both nil
              }
            }
            return false;
          }

        case CORBA::TCKind::tk_string:
          {
            IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
              DynamicAny::DynAnyFactory_i::strip_alias (this->type_);

            uint32_t const bound = unaliased_tc->length ();

            bool rstatus, lstatus;

            std::string rhs_v, lhs_v;
            if (bound == 0)
            {
              rstatus = rhs_n->any_ >>= rhs_v;
              lstatus = this->any_ >>= lhs_v;

              if ((rstatus && lstatus) == 0)
              {
                return false;
              }
            }
            else
            {
              rstatus = (rhs_n->any_ >>= rhs_v) &&
                        (rhs_v.size () <= bound);
              lstatus = (this->any_ >>= lhs_v) &&
                        (lhs_v.size () <= bound);

              if ((rstatus && lstatus) == 0)
              {
                return false;
              }
            }
            return (rhs_v == lhs_v);
          }
        case CORBA::TCKind::tk_wstring:
          {
            IDL::traits<CORBA::TypeCode>::ref_type unaliased_tc =
              DynamicAny::DynAnyFactory_i::strip_alias (this->type_);

             uint32_t bound = unaliased_tc->length ();

             bool rstatus, lstatus;
             std::wstring rhs_v, lhs_v;
             if (bound == 0)
             {
               rstatus = rhs_n->any_ >>= rhs_v;
               lstatus = this->any_ >>= lhs_v;
               if ((rstatus && lstatus) == 0)
               {
                 return false;
               }
             }
             else
             {
               rstatus = (rhs_n->any_ >>= rhs_v) &&
                          (rhs_v.size () <= bound);
               lstatus = (this->any_ >>= lhs_v) &&
                         (lhs_v.size () <= bound);
               if ((rstatus && lstatus) == 0)
               {
                 return false;
               }
             }
             return (rhs_v == lhs_v);
          }
        default:
          break; // Cannot happen...
        }
      return false;
   }

    void
    DynAny_i::destroy ()
    {
      TAOX11_LOG_TRACE ("DynAny_i::destroy");
      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      // no destroy on component DynAny objects
      if (!this->ref_to_component_ || this->container_is_destroying_)
      {
        this->destroyed_ = true;
      }
    }

    TAOX11_IDL::traits<DynamicAny::DynAny>::ref_type
    DynAny_i::current_component ()
    {
      TAOX11_LOG_TRACE ("DynAny_i::current_component");

      if (this->destroyed_)
      {
        throw CORBA::OBJECT_NOT_EXIST ();
      }
      throw DynamicAny::DynAny::TypeMismatch ();
    }

    void
    DynAny_i::check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      // Check to see if it's a simple type.
      CORBA::TCKind tk = DynAnyFactory_i::unalias (tc);

      switch (tk)
      {
        case CORBA::TCKind::tk_null:
        case CORBA::TCKind::tk_void:
        case CORBA::TCKind::tk_short:
        case CORBA::TCKind::tk_long:
        case CORBA::TCKind::tk_ushort:
        case CORBA::TCKind::tk_ulong:
        case CORBA::TCKind::tk_float:
        case CORBA::TCKind::tk_double:
        case CORBA::TCKind::tk_longlong:
        case CORBA::TCKind::tk_ulonglong:
        case CORBA::TCKind::tk_boolean:
        case CORBA::TCKind::tk_char:
        case CORBA::TCKind::tk_wchar:
        case CORBA::TCKind::tk_octet:
        case CORBA::TCKind::tk_any:
        case CORBA::TCKind::tk_TypeCode:
        case CORBA::TCKind::tk_objref:
        case CORBA::TCKind::tk_abstract_interface:
        case CORBA::TCKind::tk_string:
        case CORBA::TCKind::tk_wstring:
        case CORBA::TCKind::tk_longdouble:
          break;
        case CORBA::TCKind::tk_sequence:
          if (tc->equivalent (CORBA::_tc_BooleanSeq)
            || tc->equivalent (CORBA::_tc_OctetSeq)
            || tc->equivalent (CORBA::_tc_CharSeq)
            || tc->equivalent (CORBA::_tc_WCharSeq)
            || tc->equivalent (CORBA::_tc_ShortSeq)
            || tc->equivalent (CORBA::_tc_UShortSeq)
            || tc->equivalent (CORBA::_tc_LongSeq)
            || tc->equivalent (CORBA::_tc_ULongSeq)
            || tc->equivalent (CORBA::_tc_LongLongSeq)
            || tc->equivalent (CORBA::_tc_ULongLongSeq)
            || tc->equivalent (CORBA::_tc_FloatSeq)
            || tc->equivalent (CORBA::_tc_DoubleSeq)
            || tc->equivalent (CORBA::_tc_LongDoubleSeq))
            {
              // Otherwise fall through.
              break;
            }
          // fallthrough
        default:
          throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();
      }
    }

    void
    DynAny_i::set_to_default_value (IDL::traits<CORBA::TypeCode>::ref_type tc)
    {
      CORBA::TCKind tk = DynAnyFactory_i::unalias (tc);
      switch (tk)
      {
        case CORBA::TCKind::tk_null:
          break;
        case CORBA::TCKind::tk_void:
          this->any_.type (CORBA::_tc_void);
          break;
        case CORBA::TCKind::tk_short:
        {
          int16_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_long:
        {
          int32_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_ushort:
        {
          uint16_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_ulong:
        {
          uint32_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_longlong:
        {
          int64_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_ulonglong:
        {
          uint64_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_boolean:
        {
          bool temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_octet:
        {
          uint8_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_char:
        {
          char temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_wchar:
        {
          wchar_t temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_float:
        {
          float temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_double:
        {
          double temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_longdouble:
        {
          long double temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_any:
        {
          CORBA::Any any {};
          this->any_<<= any;
          break;
        }
        case CORBA::TCKind::tk_TypeCode:
          this->any_ <<= CORBA::_tc_TypeCode;
          break;
        case CORBA::TCKind::tk_objref:
        {
          this->any_<<= CORBA::_tc_Object;
          break;
        }
        case CORBA::TCKind::tk_abstract_interface:
        {
          this->any_<<= CORBA::_tc_AbstractBase;
          break;
        }
        case CORBA::TCKind::tk_string:
        {
          std::string temp {};
          this->any_ <<= temp;
          break;
        }
        case CORBA::TCKind::tk_wstring:
        {
          std::wstring wstr {};
          this->any_ <<= wstr;
          break;
        }
        default:
          // Should never get here - check_typecode() has already been called.
          break;
      }
    }
  }



  namespace CORBA
  {
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynAny>::ref_type
    object_traits<DynamicAny::DynAny>::narrow (
         object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
      }
      return nullptr;
    }

    //DynFixed unimplemented, we only need to implement the narrow
    //otherwise the compiler complains.
    template<>
    TAOX11_DynamicAny_Export object_traits<DynamicAny::DynFixed>::ref_type
    object_traits<DynamicAny::DynFixed>::narrow (
         object_reference<CORBA::Object> obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
      }
      return nullptr;
   }
  }
}

