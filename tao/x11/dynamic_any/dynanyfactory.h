/**
 * @file    dynanyfactory.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 dynanyfactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNANYFACTORY_H
#define TAOX11_DYNANYFACTORY_H

#pragma once

#include "tao/x11/dynamic_any/dynamicany.h"
#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

   /**
   * @class DynAnyFactory
   *
   * @brief TAOX11 DynAnyFactory implementation
   *
   */
    class TAOX11_DynamicAny_Export DynAnyFactory_i final
      : public IDL::traits<DynAnyFactory>::base_type
    {
    public:
      /// Constructor.
      DynAnyFactory_i ();

      // = The DynamicAnyFactory methods.
      IDL::traits<DynAny>::ref_type
        create_dyn_any (const CORBA::Any& value) override;

      IDL::traits<DynAny>::ref_type
        create_dyn_any_from_type_code (
          CORBA::object_reference<CORBA::TypeCode> type) override;

      IDL::traits<DynAny>::ref_type
        create_dyn_any_without_truncation (
            const CORBA::Any& value) override;

      DynAnySeq create_multiple_dyn_anys (
          const AnySeq& values,
          bool allow_truncate) override;

      AnySeq create_multiple_anys (
          const DynAnySeq& values) override;

      /// Obtain the kind of object, after all aliasing has been removed.
      static CORBA::TCKind unalias (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Same as above, but returns type code instead of TCKind. Caller
      /// must release the return value.
      static CORBA::object_reference<CORBA::TypeCode> strip_alias (CORBA::object_reference<CORBA::TypeCode> tc);

     private:
      DynAnyFactory_i (const DynAnyFactory_i&) = delete;
      DynAnyFactory_i (DynAnyFactory_i&&) = delete;
      DynAnyFactory_i& operator= (const DynAnyFactory_i&) = delete;
      DynAnyFactory_i& operator= (DynAnyFactory_i&&) = delete;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNANYFACTORY_H */
