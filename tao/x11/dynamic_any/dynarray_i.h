/**
 * @file    dynarray_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNARRAY_I_H
#define TAOX11_DYNARRAY_I_H

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
     * @class DynArray_i
     *
     * Implementation of Dynamic Any type for arrays
     */
    class TAOX11_DynamicAny_Export DynArray_i final
      : public virtual IDL::traits<DynArray>::base_type,
        //public virtual DynamicAny::DynArray,
        public virtual TAOX11_DynCommon
    {
    public:
      /// Constructor.
      DynArray_i (bool allow_truncation=true);

      /// Destructor.
      ~DynArray_i () = default;

      /// Initialize using just a TypeCode.
      IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type
      init (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Initialize using an Any.
      IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type
      init (const CORBA::Any& any);

       // = Functions specific to DynArray.

      DynamicAny::AnySeq get_elements () override;

      void set_elements (const DynamicAny::AnySeq & value) override;

      DynamicAny::DynAnySeq get_elements_as_dyn_any () override;

      void set_elements_as_dyn_any (const DynamicAny::DynAnySeq & value) override;

      // = DynAny common functions not implemented in class TAOX11_DynCommon.

      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type current_component () override;

    private:
      /// Returns the type of elements contained in the array.
      IDL::traits<CORBA::TypeCode>::ref_type get_element_type ();

      /// Gets the length of the array from the typecode.
      uint32_t get_tc_length (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Called by both versions of init().
      void init_common (uint32_t component_count);

      // Use copy() or assign() instead of these.
      DynArray_i (const DynArray_i &src) = delete;
      DynArray_i (DynArray_i&&) = delete;
      DynArray_i &operator= (const DynArray_i &src) = delete;
      DynArray_i& operator= (DynArray_i&&) = delete;

    private:
      /// Each component is also a DynAny.
      std::vector<IDL::traits<::TAOX11_NAMESPACE::DynamicAny::DynAny>::ref_type> da_members_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNARRAY_I_H */
