/**
 * @file    any.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   TAOX11 CORBA core ANY class
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/object.h"
#include "tao/x11/stub_arg_traits.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/any_arg_traits.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/exception_macros.h"

#include "tao/x11/anytypecode/any_basic_impl.h"
#include "tao/x11/anytypecode/any_basic_impl_t.h"
#include "tao/x11/anytypecode/any_dual_impl_t.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/typecode_constants.h"

#ifdef ACE_ANY_OPS_USE_NAMESPACE
# define TAO_ANY_NAMESPACE  TAO_VERSIONED_NAMESPACE_NAME::CORBA
#else
# define TAO_ANY_NAMESPACE  TAO_VERSIONED_NAMESPACE_NAME
#endif

#include "tao/x11/log.h"

namespace TAOX11_NAMESPACE
{

  namespace CORBA
  {
    Any::Any (const Any& any)
      : impl_ (any.impl_)
    {
    }

    Any::Any (Any&& any)
      : impl_ (std::move (any.impl_))
    {
    }

    CORBA::typecode_reference
    Any::type () const
    {
      if (this->impl_)
      {
        return this->impl_->type ();
      }
      return CORBA::_tc_null;
    }

    void
    Any::type (typecode_reference tc)
    {
      bool equiv = false;

      if (this->impl_)
        {
          equiv = this->impl_->type ()->equivalent (tc);
        }

      if (equiv)
        {
          this->impl_->type (tc);
        }
      else
        {
          throw CORBA::BAD_TYPECODE ();
        }
     }
  } // namespace CORBA

  void
  operator<<= (CORBA::Any &any, uint8_t o)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_octet, &o);
  }

  void
  operator<<= (CORBA::Any &any, bool b)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_boolean, &b);
  }

  void
  operator<<= (CORBA::Any &any, char c)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_char, &c);
  }

  void
  operator<<= (CORBA::Any &any, int16_t s)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_short, &s);
  }

  void
  operator<<= (CORBA::Any &any, uint16_t us)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_ushort, &us);
  }

  void
  operator<<= (CORBA::Any &any, int32_t l)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_long, &l);
  }

  void
  operator<<= (CORBA::Any &any, uint32_t ul)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_ulong, &ul);
  }

  void
  operator<<= (CORBA::Any &any, int64_t ll)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_longlong, &ll);
  }

  void
  operator<<= (CORBA::Any &any, uint64_t ull)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_ulonglong, &ull);
  }

  void
  operator<<= (CORBA::Any &any, float f)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_float, &f);
  }

  void
  operator<<= (CORBA::Any &any, double d)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_double, &d);
  }

  void
  operator<<= (CORBA::Any &any, long double ld)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_longdouble, &ld);
  }

  void
  operator<<= (CORBA::Any &any, wchar_t ws)
  {
    Any_Basic_Impl::insert (any, CORBA::_tc_wchar, &ws);
  }

  void
  operator<<= (CORBA::Any &any, const CORBA::Any &a)
  {
    Any_Dual_Impl_T<IDL::traits<CORBA::Any>,
                    CDR_Marshal_Policy>::insert_copy (
        any,
        CORBA::_tc_any,
        a
      );
  }

  void
  operator<<= (CORBA::Any &any, CORBA::Any&& a)
  {
    Any_Dual_Impl_T<IDL::traits<CORBA::Any>,
                    CDR_Marshal_Policy>::insert (
        any,
        CORBA::_tc_any,
        std::move (a)
      );
  }

  void
  operator<<= (CORBA::Any &any, const std::string &s)
  {
    Any_Dual_Impl_T<IDL::traits<std::string>,
                    CDR_Marshal_Policy>::insert_copy (
        any,
        CORBA::_tc_string,
        s
      );
  }

  void
  operator<<= (CORBA::Any &any, std::string &&s)
  {
    Any_Dual_Impl_T<IDL::traits<std::string>,
                    CDR_Marshal_Policy>::insert (
        any,
        CORBA::_tc_string,
        std::move (s)
      );
  }

#if !defined(ACE_LACKS_STD_WSTRING)
  void
  operator <<= (CORBA::Any &any, const std::wstring &str)
  {
    Any_Dual_Impl_T<IDL::traits<std::wstring>,
                    CDR_Marshal_Policy>::insert_copy (
        any,
        CORBA::_tc_wstring,
        str
      );
  }

  void
  operator<<= (CORBA::Any &any, std::wstring &&str)
  {
    Any_Dual_Impl_T<IDL::traits<std::wstring>,
                    CDR_Marshal_Policy>::insert (
        any,
        CORBA::_tc_wstring,
        std::move (str)
      );
  }
#endif

  // Insertion of TypeCode
  void
  operator<<= (CORBA::Any &any, CORBA::typecode_reference tc)
  {
     if (!tc)
     {
       throw CORBA::MARSHAL (4, CORBA::COMPLETED_NO);
     }

     Any_Basic_Impl_T<IDL::traits<CORBA::TypeCode>,
                      CDR_Marshal_Policy>::insert (any,
                                                   CORBA::_tc_TypeCode,
                                                   std::move(tc));
  }

  // Insertion of CORBA object
  void
  operator<<= (CORBA::Any &any, IDL::traits<CORBA::Object>::ref_type objref)
  {
    if (objref && objref->_is_local ())
    {
      Any_Object_Impl_T<IDL::traits<CORBA::Object>,
                        NULL_Marshal_Policy>::insert (any,
                                                      CORBA::_tc_Object,
                                                      std::move(objref));
    }
    else
    {
      Any_Object_Impl_T<IDL::traits<CORBA::Object>,
                       CDR_Marshal_Policy>::insert (any,
                                                    CORBA::_tc_Object,
                                                    std::move(objref));
    }
  }

  // Extraction: these are safe and hence we have to check that the
  // typecode of the Any is equal to the one we are trying to extract
  // into.

  // Extraction into the other basic types.
  bool
  operator>>= (const CORBA::Any &any, uint8_t &o)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_octet, &o);
  }

  bool
  operator>>= (const CORBA::Any &any, bool &b)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_boolean, &b);
  }

  bool
  operator>>= (const CORBA::Any &any, char &c)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_char, &c);
  }
  bool
  operator>>= (const CORBA::Any &any, int16_t &s)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_short, &s);
  }

  bool
  operator>>= (const CORBA::Any &any, uint16_t &us)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_ushort, &us);
  }

  bool
  operator>>= (const CORBA::Any &any, int32_t &l)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_long, &l);
  }

  bool
  operator>>= (const CORBA::Any &any, uint32_t &ul)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_ulong,&ul);
  }

  bool
  operator>>= (const CORBA::Any &any, int64_t &ll)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_longlong,&ll);
  }

  bool
  operator>>= (const CORBA::Any &any, uint64_t &ull)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_ulonglong,&ull);
  }

  bool
  operator>>= (const CORBA::Any &any, float &f)
  {
   return Any_Basic_Impl::extract (any, CORBA::_tc_float, &f);
  }

  bool
  operator>>= (const CORBA::Any &any, double &d)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_double,&d);
  }

  bool
  operator>>= (const CORBA::Any &any, long double &ld)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_longdouble,&ld);
  }
  bool
  operator>>= (const CORBA::Any &any, wchar_t &ws)
  {
    return Any_Basic_Impl::extract (any, CORBA::_tc_wchar,&ws);
  }

  bool
  operator>>= (const CORBA::Any &any, CORBA::Any &a)
  {
    return Any_Dual_Impl_T<IDL::traits<CORBA::Any>,
                           CDR_Marshal_Policy>::extract (
                              any,
                              CORBA::_tc_any,
                              a);
  }

  bool
  operator>>= (const CORBA::Any &any, CORBA::typecode_reference &tc)
  {
    return Any_Basic_Impl_T<IDL::traits<CORBA::TypeCode>,
                            CDR_Marshal_Policy>::extract (any,
                                                  CORBA::_tc_TypeCode,
                                                  tc);
  }

  bool
  operator>>= (const CORBA::Any &any, IDL::traits<CORBA::Object>::ref_type& obj)
  {
    Any_Impl::ref_type const impl = any.impl ();
    if (impl)
    {
      return impl->to_object (obj);
    }
    return false;
  }

  bool
  operator >>= (const CORBA::Any &any, std::string &str)
  {
    return Any_Dual_Impl_T<IDL::traits<std::string>,
                           CDR_Marshal_Policy>::extract (
                              any,
                              CORBA::_tc_string,
                              str);
  }

#if !defined(ACE_LACKS_STD_WSTRING)
  bool
  operator >>= (const CORBA::Any &any, std::wstring &str)
  {
    return Any_Dual_Impl_T<IDL::traits<std::wstring>,
                           CDR_Marshal_Policy>::extract (
                              any,
                              CORBA::_tc_wstring,
                              str);
  }
#endif

} // namespace TAOX11_NAMESPACE

