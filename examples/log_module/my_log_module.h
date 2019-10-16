/**
 * @file    my_log_module.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Test Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef MY_LOG_MODULE_H
#define MY_LOG_MODULE_H

#include "tao/x11/log.h"
#include "my_log_module_export.h"

//FUZZ: disable check_for_ACE_SYNCH_MUTEX
namespace my_logger
{
  /**
   * @class My_Log_Module
   *
   * @brief A user defined log module, derived from the X11 log module.
   *
   * This specific log module (the user is able to declare more log modules in
   * a shared object) is specific for "MY_LOG_CATEGORY". It'll log only the
   * debug and warning messages. Besides the log message itself, it'll also log
   * the category ("MY_LOG_CATEGORY" in this case)m the priority, and the
   * date/time.
   *
   * getInstance is there to optimize the usage of the singleton
   */
  class MY_Logger_Export My_Log_Module
  : public x11_logger::Log_Module
  {
  public:
    virtual ~My_Log_Module ();

    typedef x11_logger::Log_Type_T<My_Log_Module> log_type;

    static My_Log_Module* getInstance();

  private:
    My_Log_Module ();
  };
};

/// The singleton declaration
#define MY_LOG_CATEGORY_LOGGER my_logger::My_Log_Module::log_type

/**
 * All log streaming operators. The singleton is streamed in order to determine
 * the correct priority and verbosity masks in the framework logging module.
 */
#define MY_LOG_CATEGORY_TRACE       X11_TRACE_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_DEBUG       X11_DEBUG_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_INFO        X11_INFO_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_WARNING     X11_WARNING_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_ERROR       X11_ERROR_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_CRITICAL    X11_CRITICAL_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_PANIC       X11_PANIC_LOG(MY_LOG_CATEGORY_LOGGER)

#define MY_LOG_CATEGORY_TRACE_W     X11_TRACE_W_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_DEBUG_W     X11_DEBUG_W_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_INFO_W      X11_INFO_W_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_WARNING_W   X11_WARNING_W_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_ERROR_W     X11_ERROR_W_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_CRITICAL_W  X11_CRITICAL_W_LOG(MY_LOG_CATEGORY_LOGGER)
#define MY_LOG_CATEGORY_PANIC_W     X11_PANIC_W_LOG(MY_LOG_CATEGORY_LOGGER)

#endif /* TEST_LOG_MODULE_H */
