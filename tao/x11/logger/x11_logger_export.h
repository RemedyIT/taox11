/**
 * @file    x11_logger_export.h
 * @author  Unknown
 *
 * @brief   Export file for the TAOX11 Logger library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef X11_LOGGER_EXPORT_H
#define X11_LOGGER_EXPORT_H

#include "tao/x11/taox11_export_macros.h"

#if defined (TAOX11_AS_STATIC_LIBS) && !defined (X11_LOGGER_HAS_DLL)
#  define X11_LOGGER_HAS_DLL 0
#endif /* TAOX11_AS_STATIC_LIBS && X11_LOGGER_HAS_DLL */

#if !defined (X11_LOGGER_HAS_DLL)
#  define X11_LOGGER_HAS_DLL 1
#endif /* ! X11_LOGGER_HAS_DLL */

#if defined (X11_LOGGER_HAS_DLL) && (X11_LOGGER_HAS_DLL == 1)
#  if defined (X11_LOGGER_BUILD_DLL)
#    define X11_Logger_Export TAOX11_Proper_Export_Flag
#    define X11_LOGGER_SINGLETON_DECLARATION(T) TAOX11_EXPORT_SINGLETON_DECLARATION (T)
#    define X11_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) TAOX11_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* X11_LOGGER_BUILD_DLL */
#    define X11_Logger_Export TAOX11_Proper_Import_Flag
#    define X11_LOGGER_SINGLETON_DECLARATION(T) TAOX11_IMPORT_SINGLETON_DECLARATION (T)
#    define X11_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) TAOX11_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* X11_LOGGER_BUILD_DLL */
#else /* X11_LOGGER_HAS_DLL == 1 */
#  define X11_Logger_Export
#  define X11_LOGGER_SINGLETON_DECLARATION(T)
#  define X11_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* X11_LOGGER_HAS_DLL == 1 */

#endif /* X11_LOGGER_EXPORT_H */
