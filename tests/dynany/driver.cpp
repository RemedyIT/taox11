/**
 * @file    driver.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   Implementation file for test driver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "driver.h"
#include "test_dynany.h"
#include "test_dynarray.h"
#include "test_dynenum.h"
#include "test_dynsequence.h"
#include "test_dynstruct.h"
#include "test_dynunion.h"
#include "test_dynvalue.h"
#include "test_wrapper.h"

#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

int
main(int argc, ACE_TCHAR *argv[])
{
  int16_t error_count = 0;
  try
  {
    Driver driver;

    // initialize the driver
    if (driver.init (argc, argv) == -1)
    {
      TAOX11_TEST_ERROR << "ERROR: driver initialization failed." << std::endl;
      return 1;
    }
    // run the tests
    error_count = driver.run ();
    if (error_count != 0)
    {
      TAOX11_TEST_ERROR << "ERROR: " << error_count << " tests failed"
                        << std::endl;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR: Caught unexpected CORBA exception:" << ex << std::endl;
     ++error_count;
  }
  return error_count;
}

// destructor
Driver::~Driver ()
{
  if (this->orb_)
  {
    this->orb_->shutdown ();
    this->orb_->destroy ();
  }
}

// initialize the driver
int
Driver::init (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Retrieve the underlying ORB
    this->orb_ = CORBA::ORB_init (argc, argv, "local");

    // Parse command line and verify parameters.
    if (this->parse_args (argc, argv) == -1)
      TAOX11_TEST_ERROR << "ERROR driver.cpp - parse_args failed" << std::endl;
    else
      TAOX11_TEST_DEBUG << "driver.cpp - parse_args OK" << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR <<"ERROR: Driver::init exception" << ex << std::endl;
    return -1;
  }

  return 0;
}

int
Driver::parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1)
    TAOX11_TEST_ERROR <<"usage: " << argv [0]
                      <<" -t [dynany|dynarray|dynenum|dynsequence|dynstruct|dynunion|dynvalue]"
                      << std::endl;

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:"));
  int c;
  const ACE_TCHAR *test_str = 0;
  while ((c = get_opts ()) != -1)
    switch (c)
    {
      case 't':
      test_str = get_opts.opt_arg ();
      if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynany")))
        this->test_type_ = TEST_DYNANY;
      else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynarray")))
        this->test_type_ = TEST_DYNARRAY;
      else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynenum")))
        this->test_type_ = TEST_DYNENUM;
      else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynsequence")))
        this->test_type_ = TEST_DYNSEQUENCE;
      else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynstruct")))
        this->test_type_ = TEST_DYNSTRUCT;
      else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynunion")))
        this->test_type_ = TEST_DYNUNION;
      else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynvalue")))
        this->test_type_ = TEST_DYNVALUE;
      else
        TAOX11_TEST_ERROR << "I don't recognize test type"
                          << test_str << std::endl;
      break;

    case '?':
    default:
      TAOX11_TEST_ERROR << "usage: " << argv [0]
                        << " -t [dynany|dynarray|dynenum|dynsequence|dynstruct|dynunion|dynvalue]"
                        << std::endl;
      return -1;
    }

  // Indicates successful parsing of command line.
  return 0;
}

int
Driver::run ()
{
  TAOX11_TEST_DEBUG << "Driver::run starting test " << this->test_type_ << std::endl;

  int16_t error_count {};

  switch (this->test_type_)
  {
    case TEST_DYNANY:
      {
        std::unique_ptr< Test_Wrapper<Test_DynAny> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynAny > >(this->orb_);
        error_count += wrapper->run_test ();
      }
    break;
    case TEST_DYNARRAY:
      {
        std::unique_ptr< Test_Wrapper<Test_DynArray> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynArray> > (this->orb_);
        error_count += wrapper->run_test ();
      }
      break;
    case TEST_DYNENUM:
      {
        std::unique_ptr< Test_Wrapper<Test_DynEnum> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynEnum> > (this->orb_);
        error_count += wrapper->run_test ();
      }
      break;
    case TEST_DYNSEQUENCE:
      {
        std::unique_ptr< Test_Wrapper<Test_DynSequence> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynSequence> > (this->orb_);
        error_count += wrapper->run_test ();
      }
      break;
    case TEST_DYNSTRUCT:
      {
        std::unique_ptr< Test_Wrapper<Test_DynStruct> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynStruct> > (this->orb_);
        error_count += wrapper->run_test ();
      }
      break;
    case TEST_DYNUNION:
      {
        std::unique_ptr< Test_Wrapper<Test_DynUnion> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynUnion> > (this->orb_);
        error_count += wrapper->run_test ();
      }
      break;
    case TEST_DYNVALUE:
      {
        std::unique_ptr< Test_Wrapper<Test_DynValue> > wrapper =
          std::make_unique< Test_Wrapper<Test_DynValue> > (this->orb_);
        error_count += wrapper->run_test ();
      }
      break;

    default:
      break;
  }
  return error_count;
}
