/**
 * @file    dynany_impl.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynAny implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNANY_I_H
#define TAOX11_DYNANY_I_H

#pragma once

#include /**/ "ace/pre.h"

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
     * @class DynAny
     *
     * @brief Implementation DynAny interface
     *
     */
    class TAOX11_DynamicAny_Export DynAny_i
      : public virtual IDL::traits<DynAny>::base_type,
        public virtual TAOX11_DynCommon
    {
    public:
      DynAny_i (bool allow_truncation);
      ~DynAny_i () = default;

      /// Initialize using just a TypeCode
      IDL::traits<DynamicAny::DynAny>::ref_type
      init (IDL::traits<CORBA::TypeCode>::ref_type tc);

       /// Initialize using an Any.
      IDL::traits<DynamicAny::DynAny>::ref_type
      init (const CORBA::Any& any) ;

      void from_any (const CORBA::Any& value) override;

      TAOX11_NAMESPACE::CORBA::Any to_any () override;

      bool equal (
         TAOX11_IDL::traits<DynAny>::ref_type dyn_any) override;

      void destroy () override;


      IDL::traits<DynAny>::ref_type
      current_component () override;

    private:
      /// Check if the typecode is acceptable.
      void check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Used when we are created from a typecode.
      void set_to_default_value (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Called by both versions of init().
      void init_common ();

      DynAny_i (const DynAny_i&) = delete;
      DynAny_i (DynAny_i&&) = delete;
      DynAny_i& operator= (const DynAny_i&) = delete;
      DynAny_i& operator= (DynAny_i&&) = delete;

    };
  } // namespace DynamicAny

} // namespace TAOX11_NAMESPACE


#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAOX11_DYNANY_I_H */
