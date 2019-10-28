/**
 * @file    any_basic_impl.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any_impl class for basic language
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_BASIC_IMPL_H
#define TAOX11_ANY_BASIC_IMPL_H

#pragma once

#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class Any_Basic_Impl
   *
   * @brief Non-template class for all the basic types.
   */
  class TAOX11_AnyTypeCode_Export Any_Basic_Impl : public Any_Impl
  {
  public:
    typedef std::shared_ptr<Any_Basic_Impl> ref_type;

    Any_Basic_Impl (CORBA::typecode_reference,
                    void *value);

    virtual ~Any_Basic_Impl () = default;

    static void insert (CORBA::Any &,
                        CORBA::typecode_reference,
                        const void *);
    static bool extract (const CORBA::Any &,
                         CORBA::typecode_reference,
                         void *);

    virtual bool marshal_value (TAO_OutputCDR &);

    bool demarshal_value (TAO_InputCDR &);
    bool demarshal_value (TAO_InputCDR &,
                          uint32_t);

    virtual void _tao_decode (TAO_InputCDR &);

    static Any_Basic_Impl *create_empty (CORBA::typecode_reference);

  private:
    static void assign_value (void *, ref_type);

    static void assign_value (void *,
                              ref_type,
                              uint32_t tck);
  private:
    uint32_t kind_;
    union
    {
      int16_t s;
      uint16_t us;
      int32_t l;
      uint32_t ul;
      float f;
      double d;
      bool b;
      char c;
      uint8_t o;
      int64_t ll;
      uint64_t ull;
      long double ld;
      wchar_t wc;
    }u_;
  };

} // namespace TAOX11_NAMESPACE

#endif /* TAO_ANY_BASIC_IMPL_H */
