/**
 * @file    dynstruct_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNSTRUCT_I_H
#define TAOX11_DYNSTRUCT_I_H

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
     * @class DynStruct_i
     *
     * Implementation of Dynamic Any type for Structs
     */
    class TAOX11_DynamicAny_Export DynStruct_i final
      : public virtual IDL::traits<DynStruct>::base_type,
        //public virtual DynamicAny::DynStruct,
        public virtual TAOX11_DynCommon
    {
    public:
      /// Constructor.
      DynStruct_i (bool allow_truncation=true);

      /// Destructor.
      ~DynStruct_i () = default;

      /// Initialize using just a TypeCode.
      IDL::traits<DynamicAny::DynAny>::ref_type
      init (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Initialize using an Any.
      IDL::traits<DynamicAny::DynAny>::ref_type
      init (const CORBA::Any& any);

      // = Functions specific to DynStruct.

      DynamicAny::FieldName current_member_name () override;

      CORBA::TCKind current_member_kind () override;

      DynamicAny::NameValuePairSeq get_members () override;

      void set_members (const DynamicAny::NameValuePairSeq& value) override;

      DynamicAny::NameDynAnyPairSeq get_members_as_dyn_any () override;

      void set_members_as_dyn_any (
          const DynamicAny::NameDynAnyPairSeq & value) override;

      // = DynAny common functions not implemented in class TAO_DynCommon.

      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (IDL::traits<DynamicAny::DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<DynamicAny::DynAny>::ref_type current_component () override;

    private:
      /// Check if the typecode is acceptable.
      void check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Code common to the constructor from an Any arg and the member
      /// function from_any().
      void set_from_any (const CORBA::Any &any);

      /// Called by both versions of init().
      void init_common ();

      // = Use copy() or assign() instead of these.
      DynStruct_i (const DynStruct_i &src) = delete;
      DynStruct_i (DynStruct_i&&) = delete;
      DynStruct_i &operator= (const DynStruct_i &src) = delete;
      DynStruct_i& operator= (DynStruct_i&&) = delete;

    private:
      /// Each component is also a DynAny.
      std::vector< IDL::traits<DynamicAny::DynAny>::ref_type> da_members_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNSTRUCT_I_H */
