/**
 * @file    cdr_long_double.h
 * @author  Martin Corino
 *
 * @brief   long double stream ops
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_CDR_LONG_DOUBLE_H_INCLUDED
#define TAOX11_CDR_LONG_DOUBLE_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/CDR.h"

#if (ACE_SIZEOF_LONG_DOUBLE != 16)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// CDR output operator for long double
inline CORBA::Boolean operator<< (TAO_OutputCDR& os,
                                  long double x)
{
  CORBA::LongDouble ld;
  return
    os.fragment_stream (ACE_CDR::LONGDOUBLE_ALIGN,
                        sizeof (CORBA::LongDouble))
    && static_cast<ACE_OutputCDR &> (os) << ld.assign (x);
}

/// CDR input operators for long double
inline CORBA::Boolean operator>> (TAO_InputCDR &is,
                                  long double &x)
{
  CORBA::LongDouble ld;
  if (!(static_cast<ACE_InputCDR &> (is) >> ld))
    return false;
  x = ld;
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // (ACE_SIZEOF_LONG_DOUBLE != 16)

#endif // TAOX11_CDR_LONG_DOUBLE_H_INCLUDED
