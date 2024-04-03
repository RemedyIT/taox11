/**
 * @file    x11_logger_export.h
 * @author  Unknown
 *
 * @brief   Export file for the TAOX11 Logger library
 *
 * This file is generated automatically by generate_export_file.pl TAOX11_Logger
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef X11_LOGGER_EXPORT_H
#define X11_LOGGER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (X11_LOGGER_HAS_DLL)
#  define X11_LOGGER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && X11_LOGGER_HAS_DLL */

#if !defined (X11_LOGGER_HAS_DLL)
#  define X11_LOGGER_HAS_DLL 1
#endif /* ! X11_LOGGER_HAS_DLL */

#if defined (X11_LOGGER_HAS_DLL) && (X11_LOGGER_HAS_DLL == 1)
#  if defined (X11_LOGGER_BUILD_DLL)
#    define X11_Logger_Export ACE_Proper_Export_Flag
#    define X11_LOGGER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define X11_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* X11_LOGGER_BUILD_DLL */
#    define X11_Logger_Export ACE_Proper_Import_Flag
#    define X11_LOGGER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define X11_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* X11_LOGGER_BUILD_DLL */
#else /* X11_LOGGER_HAS_DLL == 1 */
#  define X11_Logger_Export
#  define X11_LOGGER_SINGLETON_DECLARATION(T)
#  define X11_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* X11_LOGGER_HAS_DLL == 1 */

#endif /* X11_LOGGER_EXPORT_H */

// End of auto generated file.
