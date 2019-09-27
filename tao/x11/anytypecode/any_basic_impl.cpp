/**
 * @file    any_basic_impl.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any_impl class for basic language
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/CDR.h"

#include "tao/x11/anytypecode/any_basic_impl.h"
#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/typecode_constants.h"
#include "tao/x11/system_exception.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"

namespace TAOX11_NAMESPACE
{
  Any_Basic_Impl::Any_Basic_Impl (CORBA::typecode_reference tc,
                                  void *value)
    : Any_Impl (tc),
      kind_ (static_cast <uint32_t> (CORBA::TCKind::tk_null))
  {
    this->kind_ = static_cast <uint32_t> (TC_helper::unaliased_kind (tc));

    switch (static_cast<CORBA::TCKind> (this->kind_))
    {
      case CORBA::TCKind::tk_short:
        this->u_.s = *static_cast<int16_t *> (value);
        break;
      case CORBA::TCKind::tk_ushort:
        this->u_.us = *static_cast<uint16_t *> (value);
        break;
      case CORBA::TCKind::tk_long:
        this->u_.l = *static_cast<int32_t *> (value);
        break;
      case CORBA::TCKind::tk_ulong:
         this->u_.ul = *static_cast<uint32_t *> (value);
        break;
      case CORBA::TCKind::tk_float:
        this->u_.f = *static_cast<float *> (value);
        break;
      case CORBA::TCKind::tk_double:
        this->u_.d = *static_cast<double *> (value);
        break;
      case CORBA::TCKind::tk_boolean:
        this->u_.b = *static_cast<bool *> (value);
        break;
      case CORBA::TCKind::tk_char:
        this->u_.c = *static_cast<char *> (value);
        break;
      case CORBA::TCKind::tk_octet:
        this->u_.o = *static_cast<uint8_t *> (value);
        break;
      case CORBA::TCKind::tk_longlong:
        this->u_.ll = *static_cast<int64_t *> (value);
        break;
      case CORBA::TCKind::tk_ulonglong:
        this->u_.ull = *static_cast<uint64_t *> (value);
        break;
      case CORBA::TCKind::tk_longdouble:
        this->u_.ld = *static_cast<long double *> (value);
        break;
      case CORBA::TCKind::tk_wchar:
        this->u_.wc = *static_cast<wchar_t *> (value);
        break;
      default:
        break;
    }
  }

  void
  Any_Basic_Impl::insert (CORBA::Any &any,
                          CORBA::typecode_reference tc,
                          const void *value)
  {
    Any_Basic_Impl *new_impl {};
    ACE_NEW (new_impl,
             Any_Basic_Impl (std::move(tc),
                             const_cast<void *> (value)));
    ref_type safe_new_impl (new_impl);
    any.replace (safe_new_impl);
  }

  bool
  Any_Basic_Impl::extract (const CORBA::Any &any,
                           CORBA::typecode_reference tc,
                           void *_tao_elem)
  {
    try
      {
        CORBA::typecode_reference any_tc = any.type ();
        bool const _tao_equiv =
          any_tc->equivalent (tc);

        if (!_tao_equiv)
          {
            return false;
          }

        Any_Impl::ref_type const impl = any.impl ();

        if (impl && !impl->encoded ())
          {
            ref_type const narrow_impl =
              std::dynamic_pointer_cast<Any_Basic_Impl> (impl);

            if (!narrow_impl)
              {
                return false;
              }

            Any_Basic_Impl::assign_value (_tao_elem, narrow_impl);
            return true;
          }

        Any_Basic_Impl *replacement =
          Any_Basic_Impl::create_empty (any_tc);

        ref_type replacement_safety (replacement);

        // We know this will work since the unencoded case is covered above.
        Unknown_IDL_Type::ref_type const unk =
          std::dynamic_pointer_cast<Unknown_IDL_Type> (impl);

        if (!unk)
          return false;

        // Get the kind of the type where we are extracting in ie. the
        // aliased  type if there are any. Passing the aliased kind
        // will not help.
        CORBA::TCKind const tck = tc->kind ();

        // We don't want the rd_ptr of unk to move, in case it is
        // shared by another Any. This copies the state, not the buffer.
        TAO_InputCDR for_reading (unk->_tao_get_cdr ());

        bool const good_decode =
            replacement_safety->demarshal_value (for_reading,
                                        static_cast<uint32_t> (tck));

        if (good_decode)
          {
            Any_Basic_Impl::assign_value (_tao_elem,
                                          replacement_safety,
                                          static_cast<uint32_t> (tck));
            const_cast<CORBA::Any &> (any).replace (replacement_safety);
            return true;
          }
      }
    catch (const CORBA::Exception&)
      {
      }

    return false;
  }

  bool
  Any_Basic_Impl::marshal_value (TAO_OutputCDR &cdr)
  {
    CORBA::TCKind const tckind = static_cast<CORBA::TCKind> (this->kind_);

    switch (tckind)
    {
      case CORBA::TCKind::tk_short:
        return cdr << this->u_.s;
      case CORBA::TCKind::tk_ushort:
        return cdr << this->u_.us;
      case CORBA::TCKind::tk_long:
        return cdr << this->u_.l;
      case CORBA::TCKind::tk_ulong:
        return cdr << this->u_.ul;
      case CORBA::TCKind::tk_float:
        return cdr << this->u_.f;
      case CORBA::TCKind::tk_double:
        return cdr << this->u_.d;
      case CORBA::TCKind::tk_boolean:
        return cdr << TAO_OutputCDR::from_boolean (this->u_.b);
      case CORBA::TCKind::tk_char:
        return cdr << TAO_OutputCDR::from_char (this->u_.c);
      case CORBA::TCKind::tk_octet:
        return cdr << TAO_OutputCDR::from_octet (this->u_.o);
      case CORBA::TCKind::tk_longlong:
        return cdr << this->u_.ll;
      case CORBA::TCKind::tk_ulonglong:
        return cdr << this->u_.ull;
      case CORBA::TCKind::tk_longdouble:
        return cdr << this->u_.ld;
      case CORBA::TCKind::tk_wchar:
        return cdr << TAO_OutputCDR::from_wchar (this->u_.wc);
      default:
        return false;
    }
  }

  bool
  Any_Basic_Impl::demarshal_value (TAO_InputCDR &cdr)
  {
    return this->demarshal_value (cdr,
                                  this->kind_);
  }

  bool
  Any_Basic_Impl::demarshal_value (TAO_InputCDR &cdr,
                                   uint32_t tck)
  {
    CORBA::TCKind const tckind = static_cast<CORBA::TCKind> (tck);
    switch (tckind)
    {
      case CORBA::TCKind::tk_short:
        return cdr >> this->u_.s;
      case CORBA::TCKind::tk_ushort:
        return cdr >> this->u_.us;
      case CORBA::TCKind::tk_long:
        return cdr >> this->u_.l;
      case CORBA::TCKind::tk_ulong:
        return cdr >> this->u_.ul;
      case CORBA::TCKind::tk_float:
        return cdr >> this->u_.f;
      case CORBA::TCKind::tk_double:
        return cdr >> this->u_.d;
      case CORBA::TCKind::tk_boolean:
        return cdr >> TAO_InputCDR::to_boolean (this->u_.b);
      case CORBA::TCKind::tk_char:
        return cdr >> TAO_InputCDR::to_char (this->u_.c);
      case CORBA::TCKind::tk_octet:
        return cdr >> TAO_InputCDR::to_octet (this->u_.o);
      case CORBA::TCKind::tk_longlong:
        return cdr >> this->u_.ll;
      case CORBA::TCKind::tk_ulonglong:
        return cdr >> this->u_.ull;
      case CORBA::TCKind::tk_longdouble:
        return cdr >> this->u_.ld;
      case CORBA::TCKind::tk_wchar:
        return cdr >> TAO_InputCDR::to_wchar (this->u_.wc);
      default:
        return false;
    }
  }

  void
  Any_Basic_Impl::_tao_decode (TAO_InputCDR &cdr)
  {
    if (! this->demarshal_value (cdr))
      {
        throw CORBA::MARSHAL ();
      }
  }

  Any_Basic_Impl *
  Any_Basic_Impl::create_empty (CORBA::typecode_reference tc)
  {
    CORBA::TCKind const kind = tc->kind ();

    Any_Basic_Impl * retval = nullptr;

    switch (kind)
    {
      case CORBA::TCKind::tk_longlong:
        {
          int64_t tmp (0LL);
          ACE_NEW_RETURN (retval,
                          Any_Basic_Impl (tc, &tmp),
                          nullptr);
        }

        break;
      case CORBA::TCKind::tk_longdouble:
        {
          long double tmp (0L);
          ACE_NEW_RETURN (retval,
                          Any_Basic_Impl (tc, &tmp),
                          nullptr);
        }

        break;
      default:
        {
          uint64_t tmp (0ULL);
          ACE_NEW_RETURN (retval,
                          Any_Basic_Impl (tc, &tmp),
                          nullptr);
        }

        break;
    }

    return retval;
  }

  void
  Any_Basic_Impl::assign_value (void *dest, ref_type src)
  {
    Any_Basic_Impl::assign_value (dest,
                                  src,
                                  src->kind_);
  }

  void
  Any_Basic_Impl::assign_value (void *dest,
                                ref_type src,
                                uint32_t tck)
  {
    CORBA::TCKind const kind = static_cast<CORBA::TCKind> (tck);

    switch (kind)
      {
      case CORBA::TCKind::tk_short:
        *static_cast<int16_t *> (dest) = src->u_.s;
        break;
      case CORBA::TCKind::tk_ushort:
        *static_cast<uint16_t *> (dest) = src->u_.us;
        break;
      case CORBA::TCKind::tk_long:
        *static_cast<int32_t *> (dest) = src->u_.l;
        break;
      case CORBA::TCKind::tk_ulong:
        *static_cast<uint32_t *> (dest) = src->u_.ul;
        break;
      case CORBA::TCKind::tk_float:
        *static_cast<float *> (dest) = src->u_.f;
        break;
      case CORBA::TCKind::tk_double:
        *static_cast<double *> (dest) = src->u_.d;
        break;
      case CORBA::TCKind::tk_boolean:
        *static_cast<bool *> (dest) = src->u_.b;
        break;
      case CORBA::TCKind::tk_char:
        *static_cast<char *> (dest) = src->u_.c;
        break;
      case CORBA::TCKind::tk_octet:
        *static_cast<uint8_t *> (dest) = src->u_.o;
        break;
      case CORBA::TCKind::tk_longlong:
        *static_cast<int64_t *> (dest) = src->u_.ll;
        break;
      case CORBA::TCKind::tk_ulonglong:
        *static_cast<uint64_t *> (dest) = src->u_.ull;
        break;
      case CORBA::TCKind::tk_longdouble:
        *static_cast<long double *> (dest) = src->u_.ld;
        break;
      case CORBA::TCKind::tk_wchar:
        *static_cast<wchar_t *> (dest) = src->u_.wc;
        break;
      default:
        break;
      }
  }

} // namespace TAOX11_NAMESPACE
