/**
 * @file    any_unknown_type.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any implementation class for
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_UNKNOWN_TYPE_H
#define TAOX11_ANY_UNKNOWN_TYPE_H

#pragma once

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Lock_Adapter_T.h"

#include "tao/x11/anytypecode/any_impl.h"
#include "tao/CDR.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class Unknown_IDL_Type
   *
   * @brief CDR-based Any impl class.
   *
   * Used when demarshaling an Any, and with DII/DSI, and Dynamic Anys.
   */
  class TAOX11_AnyTypeCode_Export Unknown_IDL_Type : public Any_Impl
  {
  public:
    typedef std::shared_ptr<Unknown_IDL_Type> ref_type;

    Unknown_IDL_Type (CORBA::typecode_reference,
                      TAO_InputCDR &cdr);

    Unknown_IDL_Type (CORBA::typecode_reference);

    virtual ~Unknown_IDL_Type ();

    bool marshal_value (TAO_OutputCDR &) override;
//    virtual const void *value () const;

    virtual TAO_InputCDR &_tao_get_cdr ();
    int _tao_byte_order () const override;

    void _tao_decode (TAO_InputCDR &) override;

    bool to_object (IDL::traits<CORBA::Object>::ref_type &) const override;
    bool to_value (CORBA::valuetype_reference<CORBA::ValueBase> &) const override;
    bool to_abstract_base (CORBA::abstractbase_reference<CORBA::AbstractBase> &) const override;

  private:
    // We make the lock global, so that it won't be deleted when shared.
    // For instance, see Any_Basic_Impl::extract() which copies the insides
    // from an Unknown_IDL_Type to an Any_Basic_Impl.
    // See also tao/AnyTypeCode/Any_Unknown_IDL_Type.h
    typedef ACE_Refcounted_Auto_Ptr<ACE_Lock,
                                    ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >
      LOCK;
    static LOCK const lock_i ();
    LOCK const lock_;
    mutable TAO_InputCDR cdr_;
  };
}

#endif /* TAOX11_ANY_UNKNOWN_TYPE_H */
