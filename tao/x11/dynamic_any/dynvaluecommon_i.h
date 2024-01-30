/**
 * @file    dynvaluecommon_i.h
 * @author  Marijke Henstmengel
 *
 * @brief   CORBA C++11 TAOX11 DynamicAny::DynValueCommon implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNVALUECOMMON_I_H
#define TAOX11_DYNVALUECOMMON_I_H

#pragma once

#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"
#include "tao/x11/dynamic_any/dynany_impl.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    /**
     * @class DynValueCommon_i
     *
     * @brief Implementation of the DynValueCommon class.
     */
    class TAOX11_DynamicAny_Export DynValueCommon_i
    : public virtual IDL::traits<DynValueCommon>::base_type,
      public virtual DynAny_i
    {
    public:
      /// Constructor.
      DynValueCommon_i (bool allow_truncation=true);

      /// Destructor.
      ~DynValueCommon_i () = default;

         /// Called by DynValue and DynValueBox init() functions.
      void init_common ();

      // = Functions specific to DynValueCommon.
      bool is_null () override;

      void set_to_null () override;

    private:
      // = Use copy() or assign() instead of these.
      DynValueCommon_i (const DynValueCommon_i &src) = delete;
      DynValueCommon_i (DynValueCommon_i&&) = delete;
      DynValueCommon_i &operator= (const DynValueCommon_i &src) = delete;
      DynValueCommon_i& operator= (DynValueCommon_i&&) = delete;

      /// Check if the typecode is acceptable. Needs to be provided by DynValue or DynValueBox
      virtual void check_typecode (IDL::traits<CORBA::TypeCode>::ref_type tc) = 0;

    protected:
      bool is_null_ {true};
    };
  }
}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAOX11_DYNVALUECOMMON_I_H */
