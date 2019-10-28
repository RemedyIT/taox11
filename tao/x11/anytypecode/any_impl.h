/**
 * @file    any_impl.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any implementation base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_IMPL_H
#define TAOX11_ANY_IMPL_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/object_fwd.h"
#include "tao/x11/taox11_export.h"

#include "tao/x11/anytypecode/typecode_ref.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"

#include "tao/x11/valuetype/valuetype_traits_t.h"
#include "tao/x11/valuetype/abstractbase_traits_t.h"

namespace TAOX11_NAMESPACE
{

  /**
   * @class Any_impl
   *
   * @brief Base class for the Any template subclasses.
   *
   * Contains common functionality and some pure virtual methods.
   */
  class TAOX11_AnyTypeCode_Export Any_Impl
  {
  public:
    typedef std::shared_ptr<Any_Impl> ref_type;

    bool marshal (TAO_OutputCDR &);
    virtual bool marshal_type (TAO_OutputCDR &);
    virtual bool marshal_value (TAO_OutputCDR &) = 0;

    CORBA::typecode_reference type () const
    {
      return this->type_;
    }
    void type (CORBA::typecode_reference tc)
    {
      this->type_ = tc;
    }

    virtual int _tao_byte_order () const;

    virtual void _tao_decode (TAO_InputCDR &);

    virtual bool to_object (CORBA::object_reference<CORBA::Object>&) const;
    virtual bool to_value (CORBA::valuetype_reference<CORBA::ValueBase>&) const;
    virtual bool to_abstract_base (CORBA::abstractbase_reference<CORBA::AbstractBase>&) const;

    bool encoded () const
    {
      return this->encoded_;
    }

  protected:
    Any_Impl (CORBA::typecode_reference tc,
        bool encoded = false);
    virtual ~Any_Impl () = default;

    CORBA::typecode_reference type_;
    bool encoded_;
  };
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_ANY_H */
