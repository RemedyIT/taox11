/**
 * @file    dynvalue_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynValue implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNVALUE_I_H
#define TAOX11_DYNVALUE_I_H

#pragma once

#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"
#include "tao/x11/dynamic_any/dynvaluecommon_i.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    /**
     * @class DynValue_i
     *
     * @brief Implementation of the DynValue class.
     */
    class TAOX11_DynamicAny_Export DynValue_i final
    : public virtual IDL::traits<DynValue>::base_type,
      public virtual DynValueCommon_i
    {
    public:
      /// Constructor.
      DynValue_i (bool allow_truncation=true);

      /// Destructor.
      ~DynValue_i () = default;

      /// Initialize using an Any.
      IDL::traits<DynamicAny::DynAny>::ref_type init (const CORBA::Any& any);

      /// Initialize using a TypeCode and an input stream
      IDL::traits<DynamicAny::DynAny>::ref_type init (IDL::traits<CORBA::TypeCode>::ref_type tc, TAO_InputCDR &in);

      /// Initialize using just a TypeCode.
      IDL::traits<DynamicAny::DynAny>::ref_type init (IDL::traits<CORBA::TypeCode>::ref_type tc);

       // = Functions specific to DynValue.
      DynamicAny::FieldName current_member_name () override;

      CORBA::TCKind current_member_kind () override;

      DynamicAny::NameValuePairSeq get_members () override;

      void set_members (const DynamicAny::NameValuePairSeq & value) override;

      DynamicAny::NameDynAnyPairSeq  get_members_as_dyn_any () override;

      void set_members_as_dyn_any (
          const DynamicAny::NameDynAnyPairSeq & value) override;

      // = DynAny common functions not implemented in class TAO_DynCommon.
      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (IDL::traits<DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<DynAny>::ref_type current_component () override;

      void insert_val (IDL::traits<CORBA::ValueBase>::ref_type value) override;

      IDL::traits<CORBA::ValueBase>::ref_type get_val () override;

      // = DynValueCommon needed to be provided here
      void set_to_value () override;

    private:
      /// List of base types.
      using BaseTypesList_t = std::vector<IDL::traits<CORBA::TypeCode>::ref_type>;

      /// Decompose the given TypeCode into its hierarchical list of
      /// basetypes. The first element of the list is our actual type,
      /// each basetype follows in order backwards down the hierarchy.
      /// All types stored in the list are de-aliased. Optionally
      /// return the total_member_count of the fully derived type.
      static void get_base_types (
        IDL::traits<CORBA::TypeCode>::ref_type,
        BaseTypesList_t &,
        uint32_t &total_member_count);

      /// Return the unaliased valuetype typecode that corresponds to
      /// index (0..total_members-1) from the given hierarchical list of
      /// the derived type and it basetypes.
      static IDL::traits<CORBA::TypeCode>::ref_type get_correct_base_type (
        const BaseTypesList_t &base_types,
        uint32_t &index);

      /// Return the member_type at index (0..total_members-1) from
      /// the given hierarchical list of the derived type and it basetypes.
      static IDL::traits<CORBA::TypeCode>::ref_type get_member_type (
        const BaseTypesList_t &,
        uint32_t index);

      /// Return the member_name at index (0..total_members-1) from
      /// the given hierarchical list of the derived type and it basetypes.
      static std::string get_member_name (
        const BaseTypesList_t &,
        uint32_t index);

      /// Check if the typecode is acceptable.
      void check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc) override;

      /// Common code from the init() functions, initializes the
      /// private bits from the given TypeCode
      void init_helper (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Code common to the init(Any) and the member
      /// function from_any().
      DynValue_i* from_any_helper (const CORBA::Any &any);

      /// Write the value to the output stream.
      void to_outputCDR (TAO_OutputCDR &);

      /// Read the value from the input stream
      DynValue_i* from_inputCDR (TAO_InputCDR &);

      // Use copy() or assign() instead of these.
      DynValue_i (const DynValue_i &src) = delete;
      DynValue_i (DynValue_i&&) = delete;
      DynValue_i &operator= (const DynValue_i &src) = delete;
      DynValue_i& operator= (DynValue_i&&) = delete;

      /// Each component of DynValue and DynValueBox is also a DynAny.
      std::vector< IDL::traits<DynamicAny::DynAny>::ref_type> da_members_;

      /// First element of this is our type, each basetype follows
      /// in order backwards down the hierarchy. All types stored are
      /// de-aliased.
      BaseTypesList_t da_base_types_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNVALUE_I_H */
