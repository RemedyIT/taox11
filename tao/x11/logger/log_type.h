/**
 * @file    log_type.h
 * @author  Martin Corino
 *
 * @brief   General logger type handling
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef LOGGER_LOG_TYPE_H
#define LOGGER_LOG_TYPE_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/logger/x11_logger_export.h"
#include "tao/x11/logger/log_bits.h"
#include <cstdint>
#include <string>

namespace x11_logger
{

#if defined(X11_NLOGGING)

  class NULL_LogType
  {
  public:
    NULL_LogType () = default;
    ~NULL_LogType () = default;

    /// Get the current output mask.
    static inline uint32_t output_mask ()    { return 0; }

    /// Set the output mask, returns original mask.
    static inline uint32_t output_mask (uint32_t) { return 0; }

    /// Set logfile path specifications
    static inline void set_logfile_path (std::string) {}

    /// Set logfile creation specifications
    static void set_logfile_specs (uint32_t, uint32_t = 0, uint32_t = 0) {}

    /// Get logfile base path
    static bool get_logfile_path (std::string&) { return true; }

    /// Get logfile creation specifications
    static bool get_logfile_specs (uint32_t &, uint32_t &, uint32_t &) { return true; }

    // = Get/set the priority mask.
    /// Get the current priority mask.
    static uint32_t priority_mask () { return 0; }

    /// Set the priority mask, returns original mask.
    static uint32_t priority_mask (uint32_t) { return 0; }

    // = Get/set the verbosity mask.
    /// Get the current mask.
    static uint32_t verbosity_mask () { return 0; }

    /// Set the verbosity mask, returns original mask.
    static uint32_t verbosity_mask (uint32_t) { return 0; }

    /// Get the category name
    static std::string category () { return std::string(); }
  };

#else
  /*
   * Forward declarations
   */

  class Log_Module;

  template <typename CH, typename TR>
  class log_buffer_t;

  /**
   * Base Log_Type class
   */
  class X11_Logger_Export Log_Type
  {
  public:
    virtual ~Log_Type() {}

    inline Log_Module* get_module() const
    { return this->log_module_; }

  protected:
    Log_Module *log_module_ {};

    template <typename CH, typename TR>
    friend class log_buffer_t;

    virtual Log_Type * const* get_reference () const = 0;
  };

  /**
   * Log_Type_T template
   */
  template <typename LOG_MODULE>
  class Log_Type_T
    : public Log_Type
  {
  public:
    inline Log_Type_T ()
    {
      this->log_module_ = LOG_MODULE::getInstance ();
    }
    virtual ~Log_Type_T ()
    {
      if (this == Log_Type_T<LOG_MODULE>::reference_)
      {
        // destruction of the global log type instance will only
        // take place at program shutdown when all is single
        // threaded so no need for any locking
        Log_Type_T<LOG_MODULE>::reference_ = nullptr;
      }
    }

    /// Get the current output mask.
    static uint32_t output_mask ()
    {
      return LOG_MODULE::getInstance ()->output_mask ();
    }

    /// Set the output mask, returns original mask.
    static uint32_t output_mask (uint32_t v)
    {
      return LOG_MODULE::getInstance ()->output_mask (v);
    }

    /// Set logfile path specifications
    static void set_logfile_path (std::string base_path)
    {
      LOG_MODULE::getInstance ()->set_logfile_path (base_path);
    }

    /// Set logfile creation specifications
    static void set_logfile_specs (uint32_t maxsize,
                                   uint32_t count = 1,
                                   uint32_t flags = FF_TRUNCATE)
    {
      LOG_MODULE::getInstance ()->set_logfile_specs (maxsize, count, flags);
    }

    /// Get logfile base path
    static bool get_logfile_path (std::string& path)
    {
      return LOG_MODULE::getInstance ()->get_logfile_path (path);
    }

    /// Get logfile creation specifications
    static bool get_logfile_specs (uint32_t &maxsize,
                                   uint32_t &count,
                                   uint32_t &flags)
    {
      return LOG_MODULE::getInstance ()->get_logfile_specs (maxsize, count, flags);
    }

    // = Get/set the priority mask.
    /// Get the current priority mask.
    static uint32_t priority_mask ()
    {
      return LOG_MODULE::getInstance ()->priority_mask ();
    }

    /// Set the priority mask, returns original mask.
    static uint32_t priority_mask (uint32_t v)
    {
      return LOG_MODULE::getInstance ()->priority_mask (v);
    }

    // = Get/set the verbosity mask.
    /// Get the current mask.
    static uint32_t verbosity_mask ()
    {
      return LOG_MODULE::getInstance ()->verbosity_mask ();
    }

    /// Set the verbosity mask, returns original mask.
    static uint32_t verbosity_mask (uint32_t v)
    {
      return LOG_MODULE::getInstance ()->verbosity_mask (v);
    }

    /// Get the category name
    static std::string category ()
    {
      return LOG_MODULE::getInstance ()->category ();
    }

  protected:
    template <typename CH, typename TR>
    friend class log_buffer_t;

    virtual Log_Type * const* get_reference () const override
    {
      return &Log_Type_T<LOG_MODULE>::reference_;
    }

  private:
    static Log_Type_T<LOG_MODULE>  instance_;
    static Log_Type * reference_;
  };

  // declare static members
  template <typename LOG_MODULE> Log_Type_T<LOG_MODULE>
  Log_Type_T<LOG_MODULE>::instance_;

  template <typename LOG_MODULE> Log_Type *
  Log_Type_T<LOG_MODULE>::reference_ = &Log_Type_T<LOG_MODULE>::instance_;

#endif /* !X11_NLOGGING */
}

#endif // LOGGER_LOG_TYPE_H
