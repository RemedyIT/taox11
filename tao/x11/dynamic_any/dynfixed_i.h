/**
 * @file    dynfixed_i.h
 * @brief   CORBA C++11 DynamicAny implementation for fixed-point values
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_DYNFIXED_I_H
#define TAOX11_DYNFIXED_I_H

#pragma once

#include "tao/x11/dynamic_any/taox11_dynamicany_export.h"
#include "tao/x11/dynamic_any/dyn_common.h"
#include "tao/x11/fixed_t.h"

namespace TAOX11_NAMESPACE
{
  namespace DynamicAny
  {
    class TAOX11_DynamicAny_Export DynFixed_i final
      : public virtual IDL::traits<DynFixed>::base_type,
        public virtual TAOX11_DynCommon
    {
    public:
      explicit DynFixed_i (bool allow_truncation = true);
      ~DynFixed_i () = default;

      IDL::traits<DynAny>::ref_type init (IDL::traits<CORBA::TypeCode>::ref_type tc);
      IDL::traits<DynAny>::ref_type init (CORBA::Any const& any);

      std::string get_value () override;
      bool set_value (std::string const& value) override;

      void from_any (CORBA::Any const& value) override;
      CORBA::Any to_any () override;
      bool equal (IDL::traits<DynAny>::ref_type dyn_any) override;
      void destroy () override;
      IDL::traits<DynAny>::ref_type current_component () override;

    private:
      void init_common ();
      bool read_value (TAO_InputCDR& cdr, ACE_CDR::Fixed& value) const;

      DynFixed_i (DynFixed_i const&) = delete;
      DynFixed_i (DynFixed_i&&) = delete;
      DynFixed_i& operator= (DynFixed_i const&) = delete;
      DynFixed_i& operator= (DynFixed_i&&) = delete;

      ACE_CDR::Fixed value_;
    };
  }
}

#endif /* TAOX11_DYNFIXED_I_H */
