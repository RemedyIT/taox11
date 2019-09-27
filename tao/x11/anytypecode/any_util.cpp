/**
 * @file    any_util.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any utility methods
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/CDR.h"
#include "tao/AnyTypeCode/Any.h"

#include "tao/x11/anytypecode/any_util.h"

namespace TAOX11_NAMESPACE
{
  bool
  Any_Util::x11_any_to_tao_any (const CORBA::Any &any, TAO_CORBA::Any &tao_any)
  {
    TAO_OutputCDR cdr_out;
    if (cdr_out << any)
    {
      TAO_InputCDR cdr_in (cdr_out);
      return (cdr_in >> tao_any);
    }
    return false;
  }

  bool
  Any_Util::tao_any_to_x11_any (const TAO_CORBA::Any &tao_any, CORBA::Any &any)
  {
    TAO_OutputCDR cdr_out;
    if (cdr_out << tao_any)
    {
      TAO_InputCDR cdr_in (cdr_out);
      return (cdr_in >> any);
    }
    return false;
  }
} // namespace TAOX11_NAMESPACE
