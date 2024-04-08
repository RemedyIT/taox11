/**
 * @file    dynsequence_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNSEQUENCE_I_H
#define TAOX11_DYNSEQUENCE_I_H

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
     * @class DynSequence_i
     *
     * Implementation of Dynamic Any type for Sequences.
     */
    class TAOX11_DynamicAny_Export DynSequence_i final
      : public virtual IDL::traits<DynSequence>::base_type,
        public virtual TAOX11_DynCommon
    {
    public:
      /// Constructor.
      DynSequence_i (bool allow_truncation=true);

      /// Destructor.
      ~DynSequence_i () = default;

      /// Initialize using just a TypeCode
       IDL::traits<DynamicAny::DynAny>::ref_type
       init (IDL::traits<CORBA::TypeCode>::ref_type tc);

        /// Initialize using an Any.
       IDL::traits<DynamicAny::DynAny>::ref_type
       init (const CORBA::Any& any);

      // = Functions specific to DynSequence.

      uint32_t get_length () override;

      void set_length (uint32_t len) override;

      DynamicAny::AnySeq get_elements () override;

      void set_elements (const DynamicAny::AnySeq & value) override;

      DynamicAny::DynAnySeq get_elements_as_dyn_any () override;

      void set_elements_as_dyn_any (const DynamicAny::DynAnySeq & value) override;


      // = DynAny common functions not implemented in class TAOX11_DynCommon.

      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (IDL::traits<DynamicAny::DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<DynamicAny::DynAny>::ref_type current_component () override;

    private:
      // Utility, turns the type of elements contained in the sequence.
      IDL::traits<CORBA::TypeCode>::ref_type
      get_element_type ();

      // Called by both versions of init().
      void init_common (uint32_t component_count);

      // Use copy() or assign() instead of these.
      DynSequence_i (const DynSequence_i &src) = delete;
      DynSequence_i (DynSequence_i&&) = delete;
      DynSequence_i &operator= (const DynSequence_i &src) = delete;
      DynSequence_i& operator= (DynSequence_i&&) = delete;

    private:
      /// Each component is also a DynAny.
      std::vector< IDL::traits<DynamicAny::DynAny>::ref_type> da_members_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_DYNSEQUENCE_I_H */
