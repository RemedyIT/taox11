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

#include "tao/x11/stddef.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/object_traits_t.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class TypeCode; // forward

    typedef object_reference<TypeCode>  typecode_reference;

    typedef weak_object_reference<TypeCode> weak_typecode_reference;

    typedef object_traits<TypeCode> typecode_traits;

  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits <CORBA::TypeCode> :
      public IDL::common_byval_traits <CORBA::object_reference <CORBA::TypeCode>>,
      public CORBA::object_traits <CORBA::TypeCode>
    {
      typedef std::false_type is_abstract;
      typedef std::true_type is_local;

      typedef CORBA::TypeCode base_type;

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
