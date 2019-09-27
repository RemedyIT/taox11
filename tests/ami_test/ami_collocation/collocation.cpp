/**
 * @file    collocation.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 AMI collocation test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "collocation_tester.h"

#include "testlib/taox11_testlog.h"

int main (int argc, char *argv[])
{
  int result {};
  try
  {
    Collocation_Test coll_test;


    TAOX11_TEST_DEBUG << "Init" << std::endl;
    result = coll_test.init (argc, argv);

    if (!result)
    {
      TAOX11_TEST_DEBUG << "Run" << std::endl;
      result = coll_test.run ();
    }

    TAOX11_TEST_DEBUG << "Shutdown" << std::endl;
    result += coll_test.shutdown ();

    TAOX11_TEST_DEBUG << "Finished" << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Caught exception <collocation::main> :" << ex << std::endl;
    return 1;
  }

  return result;
}
