/**
 * @file    any.h
 * @author  Marijke Hengstmengel
 *
 * @brief   TAOX11 CORBA Any class
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_H
#define TAOX11_ANY_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/object_fwd.h"
#include "tao/x11/basic_traits.h"
#include "tao/x11/taox11_export.h"

#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anytypecode/typecode_ref.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class Any; // forward
  }

  namespace IDL
  {
    template<>
    struct traits <CORBA::Any> :
      public common_traits<CORBA::Any>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline std::ostream& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
  }

  namespace CORBA
  {
    /**
     * @class Any
     *
     * This is the top-level class of the Any implementation. It is mostly a wrapper
     * for the TAO Any implementation.
     */
    class TAOX11_AnyTypeCode_Export Any
    {
    public:
      using impl_ref_type = std::shared_ptr<Any_Impl>;

      Any () = default;
      Any (const Any &);
      Any (Any&&);

      ~Any () = default;

      Any &operator= (const Any &rhs)
      {
        if (std::addressof(rhs) != this)
        {
          this->impl_ = rhs.impl_;
        }
        return *this;
      }
      Any &operator= (Any && rhs)
      {
        if (std::addressof(rhs) != this)
        {
          this->impl_ = std::move (rhs.impl_);
        }
        return *this;
      }

      impl_ref_type impl () const
      {
        return this->impl_;
      }

      /// TAOX11-specific signature.
      void replace (impl_ref_type new_impl)
      {
        this->impl_ = new_impl;
      }

      /// Return TypeCode of the element stored in the Any.
      CORBA::typecode_reference type () const;

      /// For use along with <<= of a value of aliased type when the alias must
      /// be preserved.
      void type (CORBA::typecode_reference);

    private:
      impl_ref_type impl_ {};
    };

  } // namespace CORBA

  /// Typesafe insertion.
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, uint8_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, bool);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, char);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, int16_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, uint16_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, int32_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, uint32_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, int64_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, uint64_t);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, float);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, double);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, long double);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::Any &);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Any&&);

  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &any, const std::string &s);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &any, std::string &&s); //move, non copying

  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, wchar_t);

  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::typecode_reference);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, IDL::traits<CORBA::Object>::ref_type);

#if !defined(ACE_LACKS_STD_WSTRING)
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &any, const std::wstring &s);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &any, std::wstring &&s); //move, non copying
#endif

  /// Typesafe extraction.
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, uint8_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, bool &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, char &);

  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, int16_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, uint16_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, int32_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, uint32_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, int64_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, uint64_t &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, float &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, double &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, long double &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, CORBA::Any &);
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, CORBA::typecode_reference &);

  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, IDL::traits<CORBA::Object>::ref_type&);

  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, wchar_t &);

  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &any, std::string &s);
#if !defined(ACE_LACKS_STD_WSTRING)
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &any, std::wstring &s);
#endif

  namespace IDL
  {
    template <typename OStrm_>
    struct formatter< CORBA::Any, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          const CORBA::Any&)
      {
        os_ << "CORBA::Any";
        return os_;
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::Any>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::Any>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::Any, OStrm_>,
                          typename writer_t::formatter_t>::type;
      IDL::traits<CORBA::Any>::write_on (os, w.val_, formatter_t ());
      return os;
    }
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

// Used in optionally generated ostream operators (for debugging).
// Don't put this in TAOX11_NAMESPACE !
inline std::ostream &
operator<< (std::ostream& os, const TAOX11_NAMESPACE::CORBA::Any& any)
{
  return TAOX11_NAMESPACE::IDL::traits<TAOX11_NAMESPACE::CORBA::Any>::write_on(os, any);
}

#endif /* TAOX11_ANY_H */
