/**
 * @file    my_log_module.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Test Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "my_log_module.h"

namespace my_logger
{
  My_Log_Module* My_Log_Module::getInstance()
  {
    static My_Log_Module instance_;

    return &instance_;
  }

  My_Log_Module::My_Log_Module ()
    : x11_logger::Log_Module ("MY_LOG_CATEGORY")
  {
    /// Only show the debug and warning messages in the log module. This'll override
    /// the LOG_MASK environment variable.
    this->priority_mask (
      x11_logger::X11_LogMask::LP_DEBUG |
      x11_logger::X11_LogMask::LP_WARNING);
    /// Show the category, the priority and the date/time
    this->verbosity_mask (
      x11_logger::X11_Verbose::V_CATEGORY |
      x11_logger::X11_Verbose::V_PRIO |
      x11_logger::X11_Verbose::V_DATETIME);
  }

  My_Log_Module::~My_Log_Module ()
  {
  }
};
