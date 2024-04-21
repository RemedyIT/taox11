/**
 * @file    my_log_module_export.h
 * @author  Unknown
 *
 * @brief   Export file for the Test Logger library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef MY_LOG_MODULE_EXPORT_H
#define MY_LOG_MODULE_EXPORT_H

#include "tao/x11/base/taox11_export_macros.h"

#if defined (TAOX11_AS_STATIC_LIBS) && !defined (MY_LOGGER_HAS_DLL)
#  define MY_LOGGER_HAS_DLL 0
#endif /* TAOX11_AS_STATIC_LIBS && MY_LOGGER_HAS_DLL */

#if !defined (MY_LOGGER_HAS_DLL)
#  define MY_LOGGER_HAS_DLL 1
#endif /* ! MY_LOGGER_HAS_DLL */

#if defined (MY_LOGGER_HAS_DLL) && (MY_LOGGER_HAS_DLL == 1)
#  if defined (MY_LOGGER_BUILD_DLL)
#    define MY_Logger_Export TAOX11_Proper_Export_Flag
#    define MY_LOGGER_SINGLETON_DECLARATION(T) TAOX11_EXPORT_SINGLETON_DECLARATION (T)
#    define MY_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) TAOX11_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MY_LOGGER_BUILD_DLL */
#    define MY_Logger_Export TAOX11_Proper_Import_Flag
#    define MY_LOGGER_SINGLETON_DECLARATION(T) TAOX11_IMPORT_SINGLETON_DECLARATION (T)
#    define MY_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) TAOX11_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MY_LOGGER_BUILD_DLL */
#else /* MY_LOGGER_HAS_DLL == 1 */
#  define MY_Logger_Export
#  define MY_LOGGER_SINGLETON_DECLARATION(T)
#  define MY_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MY_LOGGER_HAS_DLL == 1 */

#endif /* TEST_LOG_MODULE_EXPORT_H */
