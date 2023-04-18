/**
 * @file    dynunion_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */


#ifndef TAOX11_DYNUNION_I_H
#define TAOX11_DYNUNION_I_H

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
     * @class DynUnion_i
     *
     * Implementation of Dynamic Any type for Unions.
     */
    class TAOX11_DynamicAny_Export DynUnion_i final
      : public virtual IDL::traits<DynUnion>::base_type,
        //public virtual DynamicAny::DynUnion,
        public virtual TAOX11_DynCommon{
    public:
      /// Constructor.
      DynUnion_i (bool allow_truncation=true);

      /// Destructor.
      ~DynUnion_i () = default;

      /// Constructor taking an Any argument.
      IDL::traits<DynamicAny::DynAny>::ref_type
      init (const CORBA::Any& any);

      /// Constructor taking a typecode argument.
      IDL::traits<DynamicAny::DynAny>::ref_type
      init (IDL::traits<CORBA::TypeCode>::ref_type tc);

      // = LocalObject methods.
      //static TAO_DynUnion_i *_narrow (CORBA::Object_ptr obj);

      // = Functions specific to DynUnion.

      IDL::traits<DynamicAny::DynAny>::ref_type
      get_discriminator () override;

      void
      set_discriminator (IDL::traits<DynamicAny::DynAny>::ref_type d) override;

      void set_to_default_member () override;

      void set_to_no_active_member () override;

      bool has_no_active_member () override;

      CORBA::TCKind discriminator_kind () override;

      IDL::traits<DynamicAny::DynAny>::ref_type member () override;

      std::string member_name () override;

      CORBA::TCKind member_kind () override;

      // = DynAny common functions not implemented in class TAOX11_DynCommon.

      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (IDL::traits<DynamicAny::DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<DynamicAny::DynAny>::ref_type current_component () override;

    private:
      /// Code common to the constructor from an Any arg and the member
      /// function from_any().
      void set_from_any (const CORBA::Any &any);

      /// Called by both versions of init().
      void init_common ();

      /// Iterative check for label value match.
      bool label_match (const CORBA::Any &my_any,
                        const CORBA::Any &other_any);

      /// Use copy() or assign() instead of these.
      DynUnion_i (const DynUnion_i &src) = delete;
      DynUnion_i (DynUnion_i&&) = delete;
      DynUnion_i &operator= (const DynUnion_i &src) = delete;
      DynUnion_i& operator= (DynUnion_i&&) = delete;

    private:
      /// Just two components.
      IDL::traits<DynamicAny::DynAny>::ref_type member_;
      IDL::traits<DynamicAny::DynAny>::ref_type discriminator_;

      /// The active member's slot in the union type code.
      uint32_t member_slot_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNUNION_I_H */
