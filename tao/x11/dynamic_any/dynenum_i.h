/**
 * @file    dynenum_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNENUM_I_H
#define TAOX11_DYNENUM_I_H

#pragma once

#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"
#include "tao/x11/dynamic_any/dyn_common.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {

    /**
    * @class DynEnum_i
    *
    * Implementation of Dynamic Any type for enums
    */
    class TAOX11_DynamicAny_Export DynEnum_i final
      : public virtual IDL::traits<DynEnum>::base_type,
        public virtual TAOX11_DynCommon
     {
    public:
      /// Constructor.
      DynEnum_i (bool allow_truncation=true);

      /// Destructor.
      ~DynEnum_i () = default;

      /// Initialize using just a TypeCode.
      IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type
      init (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Initialize using an Any.
      IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type
      init (const CORBA::Any& any);

      // = Functions specific to DynEnum.

      std::string get_as_string () override;

      void set_as_string (const std::string &value) override;

      uint32_t get_as_ulong () override;

      void set_as_ulong (uint32_t value) override;

      // = DynAny common functions not implemented in class TAOX11_DynCommon.

      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (
          IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type
      current_component () override;

    private:
      // Called by both versions of init().
      void init_common ();

      // Use copy() or assign() instead of these.
      DynEnum_i (const DynEnum_i &src) = delete;
      DynEnum_i (DynEnum_i&&) = delete;
      DynEnum_i &operator= (const DynEnum_i &src) = delete;
      DynEnum_i& operator= (DynEnum_i&&) = delete;

    private:
      /// Current numeric value of the enum.
      uint32_t value_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNENUM_I_H */
