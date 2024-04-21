/**
 * @file    typecode_ref.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 Typecode reference
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_TYPECODE_REF_H_INCLUDED
#define TAOX11_TYPECODE_REF_H_INCLUDED

#include /**/ "ace/config-all.h"

#include "tao/x11/base/stddef.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/object_traits_t.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class TypeCode; // forward

    using typecode_reference = object_reference<TypeCode>;

    using weak_typecode_reference = weak_object_reference<TypeCode>;

    using typecode_traits = object_traits<TypeCode>;
  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits <CORBA::TypeCode> :
      public IDL::common_byval_traits <CORBA::object_reference <CORBA::TypeCode>>,
      public CORBA::object_traits <CORBA::TypeCode>
    {
      using is_abstract = std::false_type;
      using is_local = std::true_type;

      using base_type = CORBA::TypeCode;

      template <typename OStrm_, typename Formatter = formatter<CORBA::TypeCode, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_TYPECODE_REF_H_INCLUDED
