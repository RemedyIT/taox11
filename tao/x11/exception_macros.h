/**
 * @file    exception_macros.h
 * @author  Martin Corino
 *
 * @brief   helper macros to catch and propagate exceptions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_EXCEPTION_MACROS_H_INCLUDED
#define TAOX11_EXCEPTION_MACROS_H_INCLUDED

#include "tao/x11/system_exception_x11.h"

#define catch_tao_system_ex(ex) \
  catch (const TAO_VERSIONED_NAMESPACE_NAME::CORBA::SystemException& ex) { \
    TAOX11_NAMESPACE::_raise_x11 (ex); \
  }

#endif // TAOX11_EXCEPTION_MACROS_H_INCLUDED
