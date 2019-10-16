/**
 * @file    test_my_log_module.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Test Logging module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "my_log_module.h"

int main(int /*argc*/, char** /*argv[]*/)
{
  try
  {
    MY_LOG_CATEGORY_TRACE << "ERROR: This trace message should not be visible"
      << std::endl;
    MY_LOG_CATEGORY_DEBUG << "This debug message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_INFO << "ERROR: This info message should not be visible"
      << std::endl;
    MY_LOG_CATEGORY_WARNING << "This warning message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_ERROR << "ERROR: This error message should not be visible"
      << std::endl;
    MY_LOG_CATEGORY_CRITICAL << "ERROR: This critical message should not be visible"
      << std::endl;
    MY_LOG_CATEGORY_PANIC << "ERROR: This panic message should not be visible"
      << std::endl;

    MY_LOG_CATEGORY_LOGGER::priority_mask (x11_logger::X11_LogMask::LP_ALL);
    MY_LOG_CATEGORY_TRACE << "This trace message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_DEBUG << "This debug message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_INFO << "This info message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_WARNING << "This warning message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_ERROR << "This error message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_CRITICAL << "This critical message should be visible"
      << std::endl;
    MY_LOG_CATEGORY_PANIC << "This panic message should be visible"
      << std::endl;
  }
  catch (const std::exception& e)
  {
    // Debug otherwise it won't show in the first test case!!
    MY_LOG_CATEGORY_DEBUG << "ERROR: unexpected exception caught: " << e.what ()
      << std::endl;
    return 1;
  }
  return 0;
}
