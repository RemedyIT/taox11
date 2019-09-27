/**
 * @file    my_log_module_export.h
 * @author  Unknown
 *
 * @brief   Export file for the Test Logger library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef MY_LOG_MODULE_EXPORT_H
#define MY_LOG_MODULE_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (MY_LOGGER_HAS_DLL)
#  define MY_LOGGER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && MY_LOGGER_HAS_DLL */

#if !defined (MY_LOGGER_HAS_DLL)
#  define MY_LOGGER_HAS_DLL 1
#endif /* ! MY_LOGGER_HAS_DLL */

#if defined (MY_LOGGER_HAS_DLL) && (MY_LOGGER_HAS_DLL == 1)
#  if defined (MY_LOGGER_BUILD_DLL)
#    define MY_Logger_Export ACE_Proper_Export_Flag
#    define MY_LOGGER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define MY_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* MY_LOGGER_BUILD_DLL */
#    define MY_Logger_Export ACE_Proper_Import_Flag
#    define MY_LOGGER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define MY_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* MY_LOGGER_BUILD_DLL */
#else /* MY_LOGGER_HAS_DLL == 1 */
#  define MY_Logger_Export
#  define MY_LOGGER_SINGLETON_DECLARATION(T)
#  define MY_LOGGER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* MY_LOGGER_HAS_DLL == 1 */

// Set MY_LOGGER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (MY_LOGGER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define MY_LOGGER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define MY_LOGGER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !MY_LOGGER_NTRACE */

#if (MY_LOGGER_NTRACE == 1)
#  define MY_LOGGER_TRACE(X)
#else /* (MY_LOGGER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define MY_LOGGER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (MY_LOGGER_NTRACE == 1) */

#endif /* TEST_LOG_MODULE_EXPORT_H */

// End of auto generated file.
