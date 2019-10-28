/**
 * @file    log_test.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 logging test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"

void
test_log ()
{
  // Testcase priority 1
  // First use defaults:
  TAOX11_TEST_INFO << "**************** Use the default no priority mask"
              << " and default verbose options :"
              << "V_CATEGORY, V_PRIO, V_THREAD_ID, V_PROCESSID and V_TIME ."
              << std::endl;

  TAOX11_TEST_INFO << " begin priority testcase: 1 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_PANIC" << std::endl;
  // by default PANIC messages will be logged
  TAOX11_LOG_PANIC ("Test of the macro <TAOX11_LOG_PANIC>.") ;

  TAOX11_TEST_INFO << "not expected priority testcase: 1 " << std::endl;
  TAOX11_LOG_DEBUG ("If you see this message from < TAOX11_LOG_DEBUG>, it is an ERROR.") ;
  TAOX11_LOG_WARNING ("If you see this message from < TAOX11_LOG_WARNING>, it is an ERROR.") ;
  TAOX11_LOG_DEBUG_W ("If you see this message from < TAOX11_LOG_DEBUG_W>, it is an ERROR.") ;
  TAOX11_LOG_INFO ("If you see this message from < TAOX11_LOG_INFO>, it is an ERROR.");
  TAOX11_LOG_CRITICAL ("If you see this message from < TAOX11_LOG_CRITICAL>, it is an ERROR.");
  TAOX11_LOG_ERROR ("If you see this message from < TAOX11_LOG_ERROR>, it is an ERROR.");
  TAOX11_TEST_INFO << "end priority testcase: 1 " << std::endl;

  // Testcase priority 2
  TAOX11_TEST_INFO << "**************** Use priority mask"
              << " LP_WARNING | LP_PANIC "
              << " and verbose options V_PRIO."
              << std::endl;

  TAOX11_LOGGER::priority_mask (x11_logger::LP_WARNING |
                                x11_logger::LP_PANIC);
  TAOX11_LOGGER::verbosity_mask (x11_logger::V_PRIO );

  TAOX11_TEST_INFO << " begin priority testcase: 2 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_PANIC" << std::endl;
  TAOX11_LOG_PANIC ("Test of the macro <TAOX11_LOG_PANIC>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_WARNING" << std::endl;
  TAOX11_LOG_WARNING ("Test of the macro <TAOX11_LOG_WARNING>");

  TAOX11_TEST_INFO << " not expected priority testcase: 2 " << std::endl;
  TAOX11_LOG_DEBUG ("If you see this message from < TAOX11_LOG_DEBUG>, it is an ERROR.") ;
  TAOX11_LOG_INFO ("If you see this message from < TAOX11_LOG_INFO>, it is an ERROR.");
  TAOX11_LOG_CRITICAL ("If you see this message from < TAOX11_LOG_CRITICAL>, it is an ERROR.");
  TAOX11_LOG_ERROR ("If you see this message from < TAOX11_LOG_ERROR>, it is an ERROR.");

  TAOX11_TEST_INFO << " end priority testcase: 2 " << std::endl;

  // Testcase priority 3
  TAOX11_TEST_INFO << "**************** Use priority mask"
              << " LP_DEBUG | LP_INFO | LP_CRITICAL"
              << " and verbose options V_PRIO ."
              << std::endl;

  TAOX11_LOGGER::priority_mask (x11_logger::LP_DEBUG |
                                x11_logger::LP_INFO |
                                x11_logger::LP_CRITICAL);

  TAOX11_TEST_INFO << " begin priority testcase: 3 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_DEBUG" << std::endl;
  TAOX11_LOG_DEBUG ("Test of the macro <TAOX11_LOG_DEBUG>") ;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_INFO" << std::endl;
  TAOX11_LOG_INFO ("Test of the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_CRITICAL" << std::endl;
  TAOX11_LOG_CRITICAL ("Test of the macro <TAOX11_LOG_CRITICAL>");

  TAOX11_TEST_INFO << " not expected priority testcase: 3 " << std::endl;
  TAOX11_LOG_PANIC ("If you see this message from < TAOX11_LOG_PANIC>, it is an ERROR.");
  TAOX11_LOG_ERROR ("If you see this message from < TAOX11_LOG_ERROR>, it is an ERROR.");
  TAOX11_LOG_WARNING ("If you see this message from < TAOX11_LOG_WARNING>, it is an ERROR.");

  TAOX11_TEST_INFO << " end priority testcase: 3 " << std::endl;

  // Testcase priority 4
  TAOX11_TEST_INFO << "*************** uses priority mask"
              << " LP_ALL_ER-ROR"
              << " and verbose options V_CATEGORY."
              << std::endl;
  TAOX11_LOGGER::priority_mask (x11_logger::LP_ALL_ERROR );
  TAOX11_LOGGER::verbosity_mask (x11_logger::V_CATEGORY );

  TAOX11_TEST_INFO << " begin priority testcase: 4 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_PANIC" << std::endl;
  TAOX11_LOG_PANIC ("Test of the macro <TAOX11_LOG_PANIC>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_CRITICAL" << std::endl;
  TAOX11_LOG_CRITICAL ("Test of the macro <TAOX11_LOG_CRITICAL>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_ER-ROR" << std::endl;
  TAOX11_LOG_ERROR ("Test of the macro <TAOX11_LOG_ER-ROR>");

  TAOX11_TEST_INFO << " not expected priority testcase: 4 " << std::endl;
  TAOX11_LOG_DEBUG ("If you see this message from < TAOX11_LOG_DEBUG>, it is an ERROR.") ;
  TAOX11_LOG_INFO ("If you see this message from < TAOX11_LOG_INFO>, it is an ERROR.");
  TAOX11_LOG_WARNING ("If you see this message from < TAOX11_LOG_WARNING>, it is an ERROR.");

  TAOX11_TEST_INFO << " end priority testcase: 4 " << std::endl;

  // Testcase priority 5
  TAOX11_TEST_INFO << "**************** Use priority mask"
              << " LP_ALL  and verbose options "
              << "V_CATEGORY."
              << std::endl;
  TAOX11_LOGGER::priority_mask (x11_logger::LP_ALL );

  TAOX11_TEST_INFO << " begin priority testcase: 5 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_DEBUG" << std::endl;
  TAOX11_LOG_DEBUG ("Test of the macro <TAOX11_LOG_DEBUG>") ;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_DEBUG_W" << std::endl;
  TAOX11_LOG_DEBUG_W (L"Test of the macro" << "<TAOX11_LOG_DEBUG_W>") ;
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_INFO" << std::endl;
  TAOX11_LOG_INFO ("Test of the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_PANIC" << std::endl;
  TAOX11_LOG_PANIC ("Test of the macro <TAOX11_LOG_PANIC>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_CRITICAL" << std::endl;
  TAOX11_LOG_CRITICAL ("Test of the macro <TAOX11_LOG_CRITICAL>");
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_WARNING" << std::endl;
  TAOX11_LOG_WARNING ("Test of the macro <TAOX11_LOG_WARNING>");

  TAOX11_TEST_INFO << "not expected priority testcase: 5 " << std::endl;
  TAOX11_TEST_INFO << "end priority testcase: 5 " << std::endl;

  // Testcase priority 6
  TAOX11_TEST_INFO << "**************** Use priority mask"
              << " LP_ALL  and verbose options "
              << "V_CATEGORY."
              << std::endl;
  TAOX11_LOGGER::priority_mask (x11_logger::LP_ALL );

  TAOX11_TEST_INFO << " begin priority testcase: 6 " << std::endl;

  // Test if/else-construction without brackets, because this
  // has caused problems in the past.
  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_DEBUG" << std::endl;
  if (1)
    TAOX11_LOG_DEBUG ("Test of the macro <TAOX11_LOG_DEBUG>") ;
  else TAOX11_TEST_INFO << "If-else test <TAOX11_LOG_DEBUG>, if you see this it is an ERROR" << std::endl;

  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_DEBUG" << std::endl;
  if (!1)
    TAOX11_LOG_DEBUG ("If-else test<TAOX11_LOG_DEBUG>, if you see this it is an ERROR") ;
  else TAOX11_LOG_DEBUG  ("Test of the macro <TAOX11_LOG_DEBUG>");

  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_DEBUG_W" << std::endl;
  if (1)
    TAOX11_LOG_DEBUG_W (L"Test of the macro" << "<TAOX11_LOG_DEBUG_W>") ;
  else TAOX11_TEST_INFO << "If-else test <TAOX11_LOG_DEBUG_W>, if you see this it is an ERROR" << std::endl;

  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_INFO" << std::endl;
  if (1)
    TAOX11_LOG_INFO ("Test of the macro <TAOX11_LOG_INFO>");
  else TAOX11_TEST_INFO << "If-else test <TAOX11_LOG_INFO>, if you see this it is an ERROR" << std::endl;

  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_PANIC" << std::endl;
  if (1)
    TAOX11_LOG_PANIC ("Test of the macro <TAOX11_LOG_PANIC>");
  else TAOX11_TEST_INFO << "If-else test <TAOX11_LOG_PANIC>, if you see this it is an ERROR" << std::endl;

  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_CRITICAL" << std::endl;
  if (1)
    TAOX11_LOG_CRITICAL ("Test of the macro <TAOX11_LOG_CRITICAL>");
  else TAOX11_TEST_INFO << "If-else test <TAOX11_LOG_CRITICAL>, if you see this it is an ERROR" << std::endl;

  TAOX11_TEST_INFO << "TAOX11_LOG expected: TAOX11_LOG_WARNING" << std::endl;
  if (1)
    TAOX11_LOG_WARNING ("Test of the macro <TAOX11_LOG_WARNING>");
  else TAOX11_TEST_INFO << "If-else test <TAOX11_LOG_WARNING>, if you see this it is an ERROR" << std::endl;

  TAOX11_TEST_INFO << "not expected priority testcase: 6 " << std::endl;
  TAOX11_TEST_INFO << "end priority testcase: 6 " << std::endl;

  // Testcase verbose 1
  TAOX11_TEST_INFO << "************* uses priority mask"
              << " LP_INFO"
              << " and verbose options :"
              << "V_THREAD_ID, V_PROCESS_ID and V_DATETIME ."
              << std::endl;
  TAOX11_LOGGER::priority_mask (x11_logger::LP_INFO );

  TAOX11_LOGGER::verbosity_mask (x11_logger::V_THREAD_ID |
                                 x11_logger::V_PROCESS_ID |
                                 x11_logger::V_DATETIME );

  TAOX11_TEST_INFO << " begin verbose testcase: 1 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity expected: V_THREAD_ID,V_PROCESS_ID,V_DATETIME" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity unexpected: V_CATEGORY,V_PRIO" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << " end verbose testcase: 1 " << std::endl;

  // Testcase verbose 2
  TAOX11_TEST_INFO << "************* uses priority mask"
              << " LP_INFO"
              << " and verbose options :"
              << "V_CATEGORY, V_PRIO, V_PROCESS_ID and V_TIME ."
              << std::endl;

  TAOX11_LOGGER::verbosity_mask (x11_logger::V_PRIO |
                                 x11_logger::V_CATEGORY |
                                 x11_logger::V_PROCESS_ID |
                                 x11_logger::V_TIME );

  TAOX11_TEST_INFO << " begin verbose testcase: 2 " << std::endl;
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity expected: V_CATEGORY,V_PRIO,V_PROCESS_ID,V_TIME" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity  with the macro <TAOX11_LOG_INFO>");
  TAOX11_TEST_INFO << "TAOX11_LOG verbosity unexpected: V_DATETIME" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");

  TAOX11_TEST_INFO << " end verbose testcase: 2 " << std::endl;

  // Testcase same messages with TAOX11_LOG and X11_LOG .
  TAOX11_TEST_INFO << "************* uses priority mask"
              << " LP_INFO, and verbose options :"
              << "V_CATEGORY, V_PRIO."
              << " Expecting 2 equal messages:" << std::endl;
  TAOX11_LOGGER::verbosity_mask (x11_logger::V_PRIO |
                                 x11_logger::V_CATEGORY);

  TAOX11_TEST_INFO << " begin same message testcase: 1 " << std::endl;
  TAOX11_TEST_INFO << "same message expected: Result from ret_str is hello" << std::endl;
  std::string ret_str = "hello";
  std::string mess_str = "Result from ret_str is ";
  X11_LOG_INFO (TAOX11_LOGGER, mess_str << ret_str);
  TAOX11_LOG_INFO (mess_str << ret_str);
  TAOX11_TEST_INFO << " end same message testcase: 1 " << std::endl;

  TAOX11_TEST_INFO << " begin 2 in 1 message testcase: 1 " << std::endl;
  TAOX11_TEST_INFO << "2 lines expected: first line & second line" << std::endl;
  TAOX11_LOG_INFO ("first line" << std::endl << "second line");
  TAOX11_TEST_INFO << " end 2 in 1 message testcase: 1 " << std::endl;
}

int main(int /*argc*/, char** /*argv[]*/)
{
  try
  {
    // Test logging macro's
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
