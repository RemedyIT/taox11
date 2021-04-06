/**
 * @file    collocation.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "collocation_tester.h"

#include "testlib/taox11_testlog.h"

int main (int argc, char *argv[])
{
  int retval {};

  try
  {
    Collocation_Test coll_test;

    TAOX11_TEST_DEBUG << "Init" << std::endl;
    retval += coll_test.init (argc, argv);

    TAOX11_TEST_DEBUG << "Run" << std::endl;
    retval += coll_test.run ();

    TAOX11_TEST_DEBUG << "Shutdown" << std::endl;
    coll_test.shutdown ();

    TAOX11_TEST_DEBUG << "Finished" << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Caught exception <collocation::main> :" << ex << std::endl;
    return 1;
  }

  return 0;
}
