/**
 * @file    log_base.h
 * @author  Martin Corino
 *
 * @brief   General logger stream implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef LOGGER_LOG_BASE_H
#define LOGGER_LOG_BASE_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/logger/log_bits.h"
#include "tao/x11/logger/log_type.h"
#include "tao/x11/logger/logstream_t.h"
#include "tao/x11/logger/x11_logger_export.h"
#include "tao/x11/ext/stdext.h"
#include "ace/OS_NS_unistd.h"
#include <thread>
#if defined(X11_NLOGGING)
# include <iostream>
#endif

namespace x11_logger
{
  struct __Timestamp { bool with_date_; };
  struct __HexPrint { const char* buf_; uint32_t len_; };

#if defined(X11_NLOGGING)

  // X11_FUZZ: disable check_cout_cerr
  inline std::basic_ostream<char>& trace () { return std::cerr; }
  inline std::basic_ostream<char>& debug () { return std::cerr; }
  inline std::basic_ostream<char>& info () { return std::cerr; }
  inline std::basic_ostream<char>& warning () { return std::cerr; }
  inline std::basic_ostream<char>& error () { return std::cerr; }
  inline std::basic_ostream<char>& critical () { return std::cerr; }
  inline std::basic_ostream<char>& panic () { return std::cerr; }

  inline std::basic_ostream<wchar_t>& trace_w () { return std::wcerr; }
  inline std::basic_ostream<wchar_t>& debug_w () { return std::wcerr; }
  inline std::basic_ostream<wchar_t>& info_w () { return std::wcerr; }
  inline std::basic_ostream<wchar_t>& warning_w () { return std::wcerr; }
  inline std::basic_ostream<wchar_t>& error_w () { return std::wcerr; }
  inline std::basic_ostream<wchar_t>& critical_w () { return std::wcerr; }
  inline std::basic_ostream<wchar_t>& panic_w () { return std::wcerr; }
  // X11_FUZZ: enable check_cout_cerr

  inline std::ostream& operator<< (std::ostream& os, __Timestamp ts)
  {
    time_t t = time(nullptr);
    const size_t buflen = 64;
    char buf[buflen] = {0};
    strftime(buf, buflen-1, ts.with_date_ ? "%c" : "%T", localtime(std::addressof(t)));
    return os << buf;
  }

  inline std::wostream& operator<< (std::wostream& os, __Timestamp ts)
  {
    time_t t = time(nullptr);
    const size_t buflen = 64;
    char buf[buflen] = {0};
    strftime(buf, buflen-1, ts.with_date_ ? "%c" : "%T", localtime(std::addressof(t)));
    return os << buf;
  }

#else

  /*
   * Forward declarations
   */
  template <typename CH, typename TR>
  class log_buffer_t;

  /**
   * Base Log_Module
   */
  class X11_Logger_Export Log_Module
  {
  public:
    virtual ~Log_Module();

    // = Get/set the output mask.
    /// Get the current output mask.
    uint32_t output_mask () const;

    /// Set the output mask, returns original mask.
    uint32_t output_mask (uint32_t);

    /// Set logfile path specifications
    void set_logfile_path (std::string base_path);

    /// Set logfile creation specifications
    void set_logfile_specs (uint32_t maxsize,
                            uint32_t count = 1,
                            uint32_t flags = FF_TRUNCATE);

    /// Get logfile base path
    bool get_logfile_path (std::string& path) const;

    /// Get logfile creation specifications
    bool get_logfile_specs (uint32_t &maxsize,
                            uint32_t &count,
                            uint32_t &flags);

    // = Get/set the priority mask.
    /// Get the current priority mask.
    uint32_t priority_mask () const;

    /// Set the priority mask, returns original mask.
    uint32_t priority_mask (uint32_t);

    // = Get/set the verbosity mask.
    /// Get the current mask.
    uint32_t verbosity_mask () const;

    /// Set the verbosity mask, returns original mask.
    uint32_t verbosity_mask (uint32_t);

    /// Get the category name
    const std::string& category () const;

    static Log_Module* getInstance();

  protected:
    Log_Module (std::string category,
                uint32_t priority_mask = LP_PANIC,
                uint32_t verbosity_mask = V_CATEGORY |
                                          V_PRIO |
                                          V_PROCESS_ID |
                                          V_THREAD_ID |
                                          V_TIME);

    void get_env_var_prio_mask (bool global=false);
    void get_env_var_verbose_mask (bool global=false);
    void get_env_var_output_mask ();

  private:
    template <typename CH, typename TR>
    friend class log_buffer_t;

    Log_Module();

    void set_logfile_path_i (std::string base_path);
    void set_logfile_specs_i (uint32_t maxsize,
                              uint32_t count,
                              uint32_t flags);

    // Get current logfile stream
    // (handles rolling versions)
    FILE* get_logfile_stream (const ACE_Log_Record& lr);
    // build full log filename according
    // to logfile base path and creation specs
    void build_file_path (long);

    std::string const category_ { "X11" };
    uint32_t priority_mask_ { LP_PANIC };
    uint32_t verbosity_mask_ {};
    uint32_t output_mask_ { OS_STDERR };
    std::string file_base_;
    uint32_t file_max_size_ {};
    uint32_t file_count_ {};
    uint32_t file_flags_ {};
    FILE* file_stream_ {};
    std::string file_path_;
  };

  /**
   * Log_Module, specific for the test framework.
   */
  class X11_Logger_Export TAOX11_Test_Log_Module
    : public x11_logger::Log_Module
  {
  public:
    virtual ~TAOX11_Test_Log_Module ();

    static TAOX11_Test_Log_Module* getInstance();

    using log_type = Log_Type_T<TAOX11_Test_Log_Module>;

  private:
    TAOX11_Test_Log_Module ();
  };

  /**
   * Helpers
   */
  template <>
  X11_Logger_Export void
  log_buffer_t<wchar_t>::log_record ();

  template <>
  X11_Logger_Export void
  log_buffer_t<char>::log_record ();

# ifndef X11_EXTERN_
#   define X11_EXTERN_ extern
# endif
# ifndef X11_EXPORT_
#   define X11_EXPORT_
# endif

  X11_EXTERN_ template class X11_EXPORT_ log_ostream_t<char>;
  X11_EXTERN_ template class X11_EXPORT_ log_ostream_t<wchar_t>;

  X11_Logger_Export log_ostream_t<char>& trace ();
  X11_Logger_Export log_ostream_t<char>& debug ();
  X11_Logger_Export log_ostream_t<char>& info ();
  X11_Logger_Export log_ostream_t<char>& warning ();
  X11_Logger_Export log_ostream_t<char>& error ();
  X11_Logger_Export log_ostream_t<char>& critical ();
  X11_Logger_Export log_ostream_t<char>& panic ();

  X11_Logger_Export log_ostream_t<wchar_t>& trace_w ();
  X11_Logger_Export log_ostream_t<wchar_t>& debug_w ();
  X11_Logger_Export log_ostream_t<wchar_t>& info_w ();
  X11_Logger_Export log_ostream_t<wchar_t>& warning_w ();
  X11_Logger_Export log_ostream_t<wchar_t>& error_w ();
  X11_Logger_Export log_ostream_t<wchar_t>& critical_w ();
  X11_Logger_Export log_ostream_t<wchar_t>& panic_w ();

  X11_Logger_Export uint32_t log_indent_inc ();
  X11_Logger_Export uint32_t log_indent_dec ();

  template <typename CH, typename TR, typename LOG_MODULE>
  log_ostream_t<CH, TR>&
  operator<< (log_ostream_t<CH,TR>& os, Log_Type_T<LOG_MODULE> lm)
  {
    os.rdbuf ()->log_module (lm);
    return os;
  }

  X11_Logger_Export std::ostream&
  operator<< (std::ostream&, __Timestamp);

  X11_Logger_Export std::wostream&
  operator<< (std::wostream&, __Timestamp);

  X11_Logger_Export std::ostream&
  operator<< (std::ostream& os, __HexPrint hp);

  /**
   * Call tracer
   */
  template <typename LOGTYPE, typename CH>
  class Call_Tracer_T
  {
  public:
    Call_Tracer_T (
        log_ostream_t<CH>& logstream,
        const CH* name,
        const CH* file,
        int line)
      : logstream_ (logstream),
        name_ (name)
    {
      if (this->name_)
      {
        this->logstream_
            << LOGTYPE ()
            << std::basic_string<CH> (x11_logger::log_indent_inc (), '=')
            << "> "
            << this->name_
            << " @ " << file << ':' << line
            << std::endl;
      }
    }
    ~Call_Tracer_T ()
    {
      if (this->name_)
      {
        this->logstream_
            << LOGTYPE ()
            << '<'
            << std::basic_string<CH> (x11_logger::log_indent_dec (), '=')
            << ' '
            << this->name_
            << std::endl;
      }
    }

  private:
    log_ostream_t<CH> &logstream_;
    const CH *name_;
  };


#endif /* !X11_NLOGGING */

  template <typename CH, typename TR>
  inline std::basic_ostream<CH, TR>&
  threadid (std::basic_ostream<CH, TR>& _os) { return (_os << std::this_thread::get_id ()); }

  template <typename CH, typename TR>
  inline std::basic_ostream<CH, TR>&
  processid (std::basic_ostream<CH, TR>& _os) { return (_os << ACE_OS::getpid ()); }

  inline __Timestamp
  timestamp (bool date) { return { date }; }

  inline __HexPrint
  hexdump (const char* buf, uint32_t buflen) { return { buf, buflen }; }

  template <typename CH, typename TR>
  inline std::basic_ostream<CH, TR>&
  now (std::basic_ostream<CH, TR>& _os) { return (_os << x11_logger::timestamp (false)); }

  template <typename CH, typename TR>
  inline std::basic_ostream<CH, TR>&
  date (std::basic_ostream<CH, TR>& _os) { return (_os << x11_logger::timestamp (true)); }

} /* x11_logger */

// CORE logging
#if defined(X11_NLOGGING)

// X11_FUZZ: disable check_cout_cerr
#define X11_TRACE_LOG(_logtype_)    std::cerr
#define X11_DEBUG_LOG(_logtype_)    std::cerr
#define X11_INFO_LOG(_logtype_)     std::cerr
#define X11_WARNING_LOG(_logtype_)  std::cerr
#define X11_ERROR_LOG(_logtype_)    std::cerr
#define X11_CRITICAL_LOG(_logtype_) std::cerr
#define X11_PANIC_LOG(_logtype_)    std::cerr

#define X11_TRACE_W_LOG(_logtype_)    std::cerr
#define X11_DEBUG_W_LOG(_logtype_)    std::cerr
#define X11_INFO_W_LOG(_logtype_)     std::cerr
#define X11_WARNING_W_LOG(_logtype_)  std::cerr
#define X11_ERROR_W_LOG(_logtype_)    std::cerr
#define X11_CRITICAL_W_LOG(_logtype_) std::cerr
#define X11_PANIC_W_LOG(_logtype_)    std::cerr
// X11_FUZZ: enable check_cout_cerr

#define X11_LOGGER  x11_logger::NULL_LogType

#define X11_LOG_TRACE(_logtype_, __stmt__)
#define X11_LOG_DEBUG(_logtype_, __stmt__)
#define X11_LOG_INFO(_logtype_, __stmt__)
#define X11_LOG_WARNING(_logtype_, __stmt__)
#define X11_LOG_ERROR(_logtype_, __stmt__)
#define X11_LOG_CRITICAL(_logtype_, __stmt__)
#define X11_LOG_PANIC(_logtype_, __stmt__)
#define X11_LOG_TRACE_W( _logtype_, __stmt__)
#define X11_LOG_DEBUG_W( _logtype_, __stmt__)
#define X11_LOG_INFO_W(_logtype_, __stmt__)
#define X11_LOG_WARNING_W( _logtype_, __stmt__)
#define X11_LOG_ERROR_W(_logtype_, __stmt__)
#define X11_LOG_CRITICAL_W(_logtype_, __stmt__)
#define X11_LOG_PANIC_W(_logtype_, __stmt__)

#define X11_CALL_TRACE(_logtype_, __call__)
#define X11_CALL_TRACE_W(_logtype_, __call__)

#else

#define X11_TRACE_LOG(_logtype_)    x11_logger::trace () << _logtype_ ()
#define X11_DEBUG_LOG(_logtype_)    x11_logger::debug () << _logtype_ ()
#define X11_INFO_LOG(_logtype_)     x11_logger::info () << _logtype_ ()
#define X11_WARNING_LOG(_logtype_)  x11_logger::warning () << _logtype_ ()
#define X11_ERROR_LOG(_logtype_)    x11_logger::error () << _logtype_ ()
#define X11_CRITICAL_LOG(_logtype_) x11_logger::critical () << _logtype_ ()
#define X11_PANIC_LOG(_logtype_)    x11_logger::panic () << _logtype_ ()

#define X11_TRACE_W_LOG(_logtype_)    x11_logger::trace_w () << _logtype_ ()
#define X11_DEBUG_W_LOG(_logtype_)    x11_logger::debug_w () << _logtype_ ()
#define X11_INFO_W_LOG(_logtype_)     x11_logger::info_w () << _logtype_ ()
#define X11_WARNING_W_LOG(_logtype_)  x11_logger::warning_w () << _logtype_ ()
#define X11_ERROR_W_LOG(_logtype_)    x11_logger::error_w () << _logtype_ ()
#define X11_CRITICAL_W_LOG(_logtype_) x11_logger::critical_w () << _logtype_ ()
#define X11_PANIC_W_LOG(_logtype_)    x11_logger::panic_w () << _logtype_ ()

#define X11_LOGGER \
    x11_logger::Log_Type_T<x11_logger::Log_Module>

#define X11_LOG_TRACE(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_TRACE) == LM_TRACE) { \
      X11_TRACE_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_DEBUG(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_DEBUG) == LM_DEBUG) { \
      X11_DEBUG_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_INFO(_logtype_, __stmt__) \
 do { \
    if ((_logtype_::priority_mask () & LM_INFO) == LM_INFO) { \
      X11_INFO_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_WARNING(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_WARNING) == LM_WARNING) { \
      X11_WARNING_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_ERROR(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_ERROR) == LM_ERROR) { \
      X11_ERROR_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_CRITICAL(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_CRITICAL) == LM_CRITICAL) { \
      X11_CRITICAL_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_PANIC(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_EMERGENCY) == LM_EMERGENCY) { \
      X11_PANIC_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_TRACE_W( _logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_TRACE) == LM_TRACE) { \
      X11_TRACE_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_DEBUG_W( _logtype_, __stmt__) \
  do { \
     if ((_logtype_::priority_mask () & LM_DEBUG) == LM_DEBUG) { \
       X11_DEBUG_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_INFO_W(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_INFO) == LM_INFO) { \
      X11_INFO_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_WARNING_W( _logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_WARNING) == LM_WARNING) { \
      X11_WARNING_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_ERROR_W(_logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_ERROR) == LM_ERROR) { \
      X11_ERROR_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_CRITICAL_W( _logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_CRITICAL) == LM_CRITICAL) { \
      X11_CRITICAL_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_LOG_PANIC_W( _logtype_, __stmt__) \
  do { \
    if ((_logtype_::priority_mask () & LM_EMERGENCY) == LM_EMERGENCY) { \
      X11_PANIC_W_LOG(_logtype_) << __stmt__ << std::endl; \
    } \
  } while (0)

#define X11_CALL_TRACE(_logtype_, __call__) \
  x11_logger::Call_Tracer_T< _logtype_, char > ____ ( \
          x11_logger::trace (), \
          ((_logtype_::priority_mask () & LM_TRACE) == LM_TRACE) ? (__call__) : nullptr, \
          __FILE__, __LINE__)

#define X11_CALL_TRACE_W(_logtype_, __call__) \
  x11_logger::Call_Tracer_T< _logtype_, wchar_t > ____ ( \
          x11_logger::trace_w (), \
          ((_logtype_::priority_mask () & LM_TRACE) == LM_TRACE) ? (__call__) : nullptr, \
          __FILE__, __LINE__)

#endif /* !X11_NLOGGING */

#endif /* LOGGER_LOG_BASE_H */
