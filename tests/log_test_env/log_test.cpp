/**
 * @file    log_test.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 logging test with env. var.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"

void
test_log ()
{
  // Testcase priority 1
  // First use priority mask LP_INFO|LP_WARNING  set via env.var. TAOX11_LOG_MASK:
  TAOX11_TEST_INFO << "**************** Use priority mask LP_INFO|LP_WARNING"
              << std::endl;

  TAOX11_TEST_INFO << " begin priority testcase: 1 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_INFO" << std::endl;
  TAOX11_LOG_INFO ("Test of the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_WARNING" << std::endl;
  TAOX11_LOG_WARNING ("Test of the macro <TAOX11_LOG_WARNING>");

  TAOX11_TEST_INFO << "not expected priority testcase: 1 " << std::endl;
  TAOX11_LOG_DEBUG ("If you see this message from < TAOX11_LOG_DEBUG>, it is an ERROR.") ;
  TAOX11_TEST_INFO << "end priority testcase: 1 " << std::endl;


  // Testcase priority 2
  // Overwrite priority mask LP_INFO|LP_WARNING  set via env.var.  with LP_PANIC:
  TAOX11_TEST_INFO << "**************** Use priority mask LP_PANIC"
               << std::endl;
  TAOX11_LOGGER::priority_mask (x11_logger::LP_PANIC );
  TAOX11_TEST_INFO << " begin priority testcase: 2 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_PANIC" << std::endl;
  TAOX11_LOG_PANIC ("Test of the macro <TAOX11_LOG_PANIC>");
  TAOX11_TEST_INFO << "not expected priority testcase: 2 " << std::endl;
  TAOX11_LOG_DEBUG ("If you see this message from < TAOX11_LOG_DEBUG>, it is an ERROR.") ;
  TAOX11_LOG_INFO ("If you see this message from < TAOX11_LOG_INFO>, it is an ERROR.") ;
  TAOX11_TEST_INFO << "end priority testcase: 2 " << std::endl;

  // Testcase verbose 1
  // Use verbose mask V_CATEGORY|V_PRIO set via env. var. TAOX11_VERBOSE
  TAOX11_TEST_INFO << "************* uses priority mask LP_INFO"
              << " and verbose options :"
              << "V_CATEGORY, V_PRIO."
              << std::endl;

  TAOX11_LOGGER::priority_mask (x11_logger::LP_INFO );
  TAOX11_TEST_INFO << " begin verbose testcase: 1 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity expected: V_CATEGORY,V_PRIO" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity unexpected: V_TIME,V_PROCESS_ID" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << " end verbose testcase: 1 " << std::endl;

  // Testcase verbose 2
  // Overwrite verbose mask V_CATEGORY|V_PRIO set via env. var. with V_TIME
  TAOX11_TEST_INFO << "************* uses priority mask LP_INFO"
              << " and verbose options :"
              << "V_TIME."
              << std::endl;

  TAOX11_LOGGER::priority_mask (x11_logger::LP_INFO );
  TAOX11_LOGGER::verbosity_mask (x11_logger::V_TIME );
  TAOX11_TEST_INFO << " begin verbose testcase: 2 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity expected: V_TIME" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity unexpected: CATEGORY,V_PRIO,V_PROCESS_ID" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << " end verbose testcase: 2 " << std::endl;
}

int main(int /*argc*/, char** /*argv[]*/)
{
  try
  {
    //now test logging macro's
    test_log();

    TAOX11_TEST_INFO << "shutting down...";

    TAOX11_TEST_INFO << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    return 1;
  }
  return 0;
}
