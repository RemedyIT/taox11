/**
 * @file    versioned_x11_namespace.h
 * @author  Martin Corino
 *
 * @brief   Versioned namespace support macros
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VERSIONED_X11_NAMESPACE_H
#define TAOX11_VERSIONED_X11_NAMESPACE_H

#if !defined (TAOX11_HAS_VERSIONED_NAMESPACE)
# define TAOX11_HAS_VERSIONED_NAMESPACE 0
#endif  /* !TAOX11_HAS_VERSIONED_NAMESPACE */

#if defined (TAOX11_HAS_VERSIONED_NAMESPACE) && TAOX11_HAS_VERSIONED_NAMESPACE == 1

# ifndef TAOX11_VERSIONED_NAMESPACE_NAME
#  include "tao/x11/versionx11.h"

// Preprocessor symbols will not be expanded if they are
// concatenated.  Force the preprocessor to expand them during the
// argument prescan by calling a macro that itself calls another that
// performs the actual concatenation.
#  define TAOX11_MAKE_VERSIONED_NAMESPACE_NAME_IMPL(MAJOR,MINOR,MICRO) taox11_ ## MAJOR ## _ ## MINOR ## _ ## MICRO
#  define TAOX11_MAKE_VERSIONED_NAMESPACE_NAME(MAJOR,MINOR,MICRO) TAOX11_MAKE_VERSIONED_NAMESPACE_NAME_IMPL(MAJOR,MINOR,MICRO)
#  define TAOX11_VERSIONED_NAMESPACE_NAME TAOX11_MAKE_VERSIONED_NAMESPACE_NAME(TAOX11_MAJOR_VERSION,TAOX11_MINOR_VERSION,TAOX11_MICRO_VERSION)
# endif  /* !TAO_VERSIONED_NAMESPACE_NAME */

#else

# define TAOX11_VERSIONED_NAMESPACE_NAME taox11

#endif  /* TAOX11_HAS_VERSIONED_NAMESPACE */

#define TAOX11_NAMESPACE TAOX11_VERSIONED_NAMESPACE_NAME

#endif  /* !TAOX11_VERSIONED_X11_NAMESPACE_H */
