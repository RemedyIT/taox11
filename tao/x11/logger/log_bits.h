/**
 * @file    log_bits.h
 * @author  Martin Corino
 *
 * @brief   X11 logger flags
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef LOGGER_LOG_BITS_H
#define LOGGER_LOG_BITS_H

#pragma once

#include /**/ "ace/config-all.h"

#include "ace/Log_Priority.h"
#include <cstdint>

namespace x11_logger
{

  enum X11_LogMask : uint32_t
  {
    /// Messages indicating function-calling sequence (decimal 2).
    LP_TRACE = LM_TRACE,

    /// Messages that contain information normally of use only when
    /// debugging a program (decimal 4).
    LP_DEBUG = LM_DEBUG,

    /// Informational messages (decimal 8).
    LP_INFO = LM_INFO,

    /// Warning messages (decimal 32).
    LP_WARNING = LM_WARNING,

    /// Error messages (decimal 128).
    LP_ERROR = LM_ERROR,

    /// Critical conditions, such as hard device errors (decimal 256).
    LP_CRITICAL = LM_CRITICAL,

    /// A panic condition.  This is normally broadcast to all users
    /// (decimal 1024).
    LP_PANIC = LM_EMERGENCY,

    /// Default all kind of messages except LP_TRACE
    //LP_DEFAULT = LP_DEBUG |LP_INFO | LP_WARNING | LP_ERROR | LP_CRITICAL | LP_PANIC,

    /// All kind of messages
    LP_ALL = LP_TRACE | LP_DEBUG | LP_INFO | LP_WARNING | LP_ERROR | LP_CRITICAL | LP_PANIC,

    /// All kind of error messages
    LP_ALL_ERROR = LP_ERROR | LP_CRITICAL | LP_PANIC
  };

  enum X11_Verbose : uint32_t
  {
    V_PRIO = 01,
    V_THREAD_ID = 02,
    V_PROCESS_ID = 04,
    V_TIME = 010,
    V_DATETIME = 020,
    V_CATEGORY = 040,
    V_ALL = V_PRIO | V_THREAD_ID | V_PROCESS_ID | V_DATETIME | V_CATEGORY
  };

  enum X11_OutputSink : uint32_t
  {
    /// Write log messages to STDERR
    OS_STDERR = 0001,
    /// Write log messages to system logger
    OS_SYSLOG = 0002,
    /// Write log messages to file
    /// allows optional args as FILE=&ltbase&gt,&ltsize&gt,&ltcount&gt,&ltflags&gt
    /// base  := either a file basename (no extension)
    ///          or a path to an existing folder
    ///          or the combination of both
    /// size  := the maximum size of a single logfile
    /// count := the number of rolling logfiles
    /// flags := 'P' (files per process) and or
    ///          'T' (truncate existing files)
    OS_FILE   = 0004,
    /// Send log messages to an ACE logger daemon
    OS_IPC    = 0010,
    /// Send log messages to a custom ACE logger backend
    OS_CUSTOM = 0020
  };

  enum X11_FileFlag : uint32_t
  {
    FF_PROCESS  = 0001,
    FF_TRUNCATE = 0002
  };
}

#endif /* LOGGER_LOG_BITS_H */
