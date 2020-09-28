/**
 * @file    log.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_LOG_H
#define TAOX11_LOG_H

#include "tao/x11/logger/log_base.h"
#include "tao/x11/versioned_x11_namespace.h"
#include "tao/x11/taox11_export.h"

namespace TAOX11_NAMESPACE
{

#define taox11_trace      ::x11_logger::trace ()
#define taox11_debug      ::x11_logger::debug ()
#define taox11_info       ::x11_logger::info ()
#define taox11_warning    ::x11_logger::warning ()
#define taox11_error      ::x11_logger::error ()
#define taox11_critical   ::x11_logger::critical ()
#define taox11_panic      ::x11_logger::panic ()

#define taox11_trace_w    ::x11_logger::trace_w ()
#define taox11_debug_w    ::x11_logger::debug_w ()
#define taox11_info_w     ::x11_logger::info_w ()
#define taox11_warning_w  ::x11_logger::warning_w ()
#define taox11_error_w    ::x11_logger::error_w ()
#define taox11_critical_w ::x11_logger::critical_w ()
#define taox11_panic_w    ::x11_logger::panic_w ()

#if !defined(X11_NLOGGING)

  namespace taox11_logger
  {
    /**
     * @class TAOX11_Log_Module
     *
     * @brief The default TAOX11 Log Module
     *
     * Controls the default log output of all TAOX11
     * applications. User can manipulate the output
     * via environment variables or in code.
     * See the docs for more info.
     *
     */
    class TAOX11_Export TAOX11_Log_Module
    : public x11_logger::Log_Module
    {
    public:
      virtual ~TAOX11_Log_Module();

      static TAOX11_Log_Module* getInstance();

      using log_type = x11_logger::Log_Type_T<TAOX11_Log_Module>;

    private:
      TAOX11_Log_Module();
    };
  }

#endif /* !X11_NLOGGING */

} // namespace TAOX11_NAMESPACE

#if defined(X11_NLOGGING)

#define TAOX11_LOGGER   x11_logger::NULL_LogType

#else

#define TAOX11_LOGGER \
  TAOX11_NAMESPACE::taox11_logger::TAOX11_Log_Module::log_type

#endif /* !X11_NLOGGING */

// CORE logging
#define TAOX11_LOG_TRACE( __stmt__) \
  X11_LOG_TRACE( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_DEBUG( __stmt__) \
  X11_LOG_DEBUG(TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_INFO( __stmt__) \
  X11_LOG_INFO( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_WARNING( __stmt__) \
  X11_LOG_WARNING ( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_ERROR( __stmt__) \
  X11_LOG_ERROR( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_CRITICAL( __stmt__) \
  X11_LOG_CRITICAL( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_PANIC( __stmt__) \
  X11_LOG_PANIC( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_TRACE_W( __stmt__) \
   X11_LOG_TRACE_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_DEBUG_W( __stmt__) \
  X11_LOG_DEBUG_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_INFO_W( __stmt__) \
  X11_LOG_INFO_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_WARNING_W( __stmt__) \
  X11_LOG_WARNING_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_ERROR_W( __stmt__) \
  X11_LOG_ERROR_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_CRITICAL_W( __stmt__) \
  X11_LOG_CRITICAL_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_LOG_PANIC_W( __stmt__) \
  X11_LOG_PANIC_W( TAOX11_LOGGER, __stmt__)

#define TAOX11_CALL_TRACE(__call__) \
  X11_CALL_TRACE(TAOX11_LOGGER, __call__)

#define TAOX11_CALL_TRACE_W(__call__) \
  X11_CALL_TRACE_W(TAOX11_LOGGER, __call__)

#endif /* TAOX11_LOG_H */
