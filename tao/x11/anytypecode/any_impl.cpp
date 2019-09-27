/**
 * @file    any_impl.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any implementation base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/any_impl.h"
#include "tao/x11/anytypecode/typecode.h"

#include "tao/x11/log.h"

namespace TAOX11_NAMESPACE
{
  Any_Impl::Any_Impl (CORBA::typecode_reference tc,
                      bool encoded)
    : type_ (std::move(tc))
    , encoded_ (encoded)
  {
  }

  bool
  Any_Impl::marshal (TAO_OutputCDR &cdr)
  {
    if (!this->marshal_type (cdr))
      {
        return false;
      }

    // Once the typecode has been marshaled send the actual
    // value (this is polymorphic for valuetypes)
    return this->marshal_value (cdr);
  }

  bool
  Any_Impl::marshal_type (TAO_OutputCDR &cdr)
  {
    // Otherwise send the typecode of the inserted type.
    return ((cdr << this->type_) != 0);
  }

  int
  Any_Impl::_tao_byte_order () const
  {
    return TAO_ENCAP_BYTE_ORDER;
  }

  void
  Any_Impl::_tao_decode (TAO_InputCDR &)
  {
    throw TAOX11_CORBA::NO_IMPLEMENT ();
  }

  bool
  Any_Impl::to_object (IDL::traits<CORBA::Object>::ref_type&) const
  {
    return false;
  }

  bool
  Any_Impl::to_value (CORBA::valuetype_reference<CORBA::ValueBase>&) const
  {
    return false;
  }

  bool
  Any_Impl::to_abstract_base (CORBA::abstractbase_reference<CORBA::AbstractBase>&) const
  {
    return false;
  }

} // namespace TAOX11_NAMESPACE
