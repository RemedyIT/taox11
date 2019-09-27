For testing the logging macro's of TAOX11, CIAOX11, DANCEX11
and selfmade logging categories and testing the use of priority
- and verbose masks, you can use the script log_check.pl.
We check automatically the right output of the macro's with this script.
The script expected blocks of code in a special order and format:

For checking use of priority masks:
 1 - start with " begin priority testcase: " followed by the number of the testcase
 2 - then log the info for the in the output expected category and log macro
 3 - then use the expected log macro:
 4 - repeat 2 and 3 for all expected log macro's in the output
 5 - start with "not expected priority testcase: 1 "
 6 - follow up with  0 or more not in the output expected log macro's.
 7 - end testcase with "end priority testcase: " followed by the number of the testcase

  Example:
  taox11_info << " begin priority testcase: 1 " << std::endl;
  taox11_info << "TAOX11_LOG expected: TAOX11_LOG_DEBUG" << std::endl;
  TAOX11_LOG_DEBUG ("Test of the macro <TAOX11_LOG_DEBUG>") ;
  taox11_info << "TAOX11_LOG expected: TAOX11_LOG_DEBUG_W" << std::endl;
  TAOX11_LOG_DEBUG_W (L"Test of the macro" << "<TAOX11_LOG_DEBUG_W>") ;
  taox11_info << "TAOX11_LOG expected: TAOX11_LOG_INFO" << std::endl;
  TAOX11_LOG_INFO ("Test of the macro <TAOX11_LOG_INFO>");
  taox11_info << "not expected priority testcase: 1 " << std::endl;
  TAOX11_LOG_PANIC ("If you see this message from < TAOX11_LOG_PANIC>, it is an ERROR.") ;
  taox11_info << "end priority testcase: 1 " << std::endl;

For checking use of verbose masks:
 1 - start with " begin verbose testcase: " followed by the number of the testcase
 2 - then log the info for the in the output expected verbose values with
     category + "verbosity expected: " + list of comma separated verbose values
 3 - then use a log macro:
 4 and 5 are optional:
 4 - then log the info for the not in the output expected verbose values, with
     category + "verbosity unexpected: " + list of comma separated verbose values
 5 - follow up with a log macro
 6 - end testcase with "end verbose testcase: " followed by the number of the testcase

  Example:
  taox11_info << " begin verbose testcase: 1 " << std::endl;
  taox11_info << "TAOX11_LOG verbosity expected: V_THREAD_ID,V_PROCESS_ID,V_DATETIME" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  taox11_info << "TAOX11_LOG verbosity unexpected: V_CATEGORY,V_PRIO" << std::endl;
  TAOX11_LOG_INFO ("Test of verbosity with the macro <TAOX11_LOG_INFO>");
  taox11_info << " end verbose testcase: 1 " << std::endl;

For checking if the output of 2 logging macro's are the same:
 1 - start with " begin same message testcase: " followed by the number of the testcase
 2 - then log the expected message with "same message expected: " + message
 3 - then use the first log macro:
 4 - then use the second log macro:
 6 - end testcase with "end same message testcase: " followed by the number of the testcase

 Example:
  taox11_info << " begin same message testcase: 1 " << std::endl;
  taox11_info << "same message expected: This is a message" << std::endl;
  X11_LOG_INFO (TAOX11_LOGGER, "This is a message");
  TAOX11_LOG_INFO ("This is a message");
  taox11_info << " end same message testcase: 1 " << std::endl;
