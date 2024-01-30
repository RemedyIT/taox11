/**
 * @file    dynvaluebox_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynValueBox implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNVALUEBOX_I_H
#define TAOX11_DYNVALUEBOX_I_H

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
     * @class DynValueBox_i
     *
     * @brief Implementation of the DynValueBox class.
     */
    class TAOX11_DynamicAny_Export DynValueBox_i final
    : public virtual IDL::traits<DynValueBox>::base_type,
      public virtual DynValueCommon_i
    {
    public:
      /// Constructor.
      DynValueBox_i (bool allow_truncation=true);

      /// Destructor.
      ~DynValueBox_i () = default;

      /// Initialize using just a TypeCode.
      IDL::traits<DynamicAny::DynAny>::ref_type init (IDL::traits<CORBA::TypeCode>::ref_type tc);

      /// Initialize using an Any.
      IDL::traits<DynamicAny::DynAny>::ref_type init (const CORBA::Any& any);

      // = Functions specific to DynValueBox.
      CORBA::Any get_boxed_value () override;

      void set_boxed_value (const CORBA::Any & boxed) override;

      IDL::traits<DynAny>::ref_type get_boxed_value_as_dyn_any () override;

      void set_boxed_value_as_dyn_any (IDL::traits<DynAny>::ref_type boxed) override;

      // = DynAny common functions not implemented in class TAO_DynCommon.
      void from_any (const CORBA::Any & value) override;

      CORBA::Any to_any () override;

      bool equal (IDL::traits<DynAny>::ref_type dyn_any) override;

      void destroy () override;

      IDL::traits<DynAny>::ref_type current_component () override;

      // = DynValueCommon needed to be provided here
      void set_to_value () override;

    private:
      /// Check if the typecode is acceptable.
      void check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc) override;

      /// Code common to the constructor from an Any arg and the member
      /// function from_any().
      void set_from_any (const CORBA::Any &any);

      DynValueBox_i (const DynValueBox_i &src) = delete;
      DynValueBox_i (DynValueBox_i&&) = delete;
      DynValueBox_i &operator= (const DynValueBox_i &src) = delete;
      DynValueBox_i &operator= (DynValueBox_i&&) = delete;

      /// The boxed component of DynValueBox is another DynAny.
      IDL::traits<DynAny>::ref_type boxed_;
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNVALUEBOX_I_H */
