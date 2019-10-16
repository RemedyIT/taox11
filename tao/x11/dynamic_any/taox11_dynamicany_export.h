/**
 * @file    taox11_dynamicany_export.h
 * @author  Unknown
 *
 * @brief   Export file for the TAOX11 Dynamic any library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_DYNAMICANY_EXPORT_H
#define TAOX11_DYNAMICANY_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS)
#  if !defined (TAOX11_DYNAMICANY_HAS_DLL)
#    define TAOX11_DYNAMICANY_HAS_DLL 0
#  endif /* ! TAOX11_DYNAMICANY_HAS_DLL */
#else
#  if !defined (TAOX11_DYNAMICANY_HAS_DLL)
#    define TAOX11_DYNAMICANY_HAS_DLL 1
#  endif /* ! TAOX11_DYNAMICANY_HAS_DLL */
#endif

#if defined (TAOX11_DYNAMICANY_HAS_DLL) && (TAOX11_DYNAMICANY_HAS_DLL == 1)
#  if defined (TAOX11_DYNAMICANY_BUILD_DLL)
#    define TAOX11_DynamicAny_Export ACE_Proper_Export_Flag
#    define TAOX11_DYNAMICANY_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAOX11_DYNAMICANY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TAOX11_DYNAMICANY_BUILD_DLL */
#    define TAOX11_DynamicAny_Export ACE_Proper_Import_Flag
#    define TAOX11_DYNAMICANY_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAOX11_DYNAMICANY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TAOX11_DYNAMICANY_BUILD_DLL */
#else /* TAOX11_DYNAMICANY_HAS_DLL == 1 */
#  define TAOX11_DynamicAny_Export
#  define TAOX11_DYNAMICANY_SINGLETON_DECLARATION(T)
#  define TAOX11_DYNAMICANY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TAOX11_DYNAMICANY_HAS_DLL == 1 */

#endif /* TAOX11_DYNAMICANY_EXPORT_H */

// End of auto generated file.
