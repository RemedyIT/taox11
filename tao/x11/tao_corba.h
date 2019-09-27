/**
 * @file    tao_corba.h
 * @author  Martin Corino
 *
 * @brief   include basic TAO CORBA definitions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_TAO_CORBA_H_INCLUDED
#define TAOX11_TAO_CORBA_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/corba.h"
#include /**/ "tao/Versioned_Namespace.h"

#define TAO_CORBA \
  TAO_VERSIONED_NAMESPACE_NAME::CORBA

#define TAO_TAO \
  TAO_VERSIONED_NAMESPACE_NAME::TAO

#define TAO_IOP \
  TAO_VERSIONED_NAMESPACE_NAME::IOP

#include "tao/x11/cdr_long_double.h"

#endif // TAOX11_TAO_CORBA_H_INCLUDED
