/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"
#include "tao/x11/anytypecode/any.h"

void testUtcT(IDL::traits<Test::Hello>::ref_type & hello, int & result_)
{
  TAOX11_TEST_INFO << "Test UtcT get/set." << std::endl;
  TimeBase::UtcT getUtc = hello->get_UtcT();
  if (!equal(getUtc, utcTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->get_UtcT() return value wrong "
      << getUtc << std::endl;
    ++result_;
  }
  TimeBase::UtcT utc(utcTestValue1);
  hello->set_UtcT(utc);

  TAOX11_TEST_INFO << "Test UtcT type out parameter." << std::endl;
  TimeBase::UtcT outUtc;
  hello->out_UtcT(outUtc);
  if (!equal(outUtc, utcTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->out_UtcT() return value wrong "
      << outUtc << std::endl;
    ++result_;
  }

  TAOX11_TEST_INFO << "Test UtcT type inout parameter." << std::endl;
  TimeBase::UtcT inoutUtc(utcTestValue1);
  hello->inout_UtcT(inoutUtc);
  if (!equal(inoutUtc, utcTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->out_UtcT() return value wrong "
      << inoutUtc << std::endl;
    ++result_;
  }

  TAOX11_TEST_INFO << "Test UtcT type attribute." << std::endl;
  if (!equal(hello->utc(), utcTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->utc() return value wrong "
      << hello->utc() << std::endl;
    ++result_;
  }
  hello->utc(utcTestValue1);

  TAOX11_TEST_INFO << "Test UtcT type swap method." << std::endl;
  utc.swap(inoutUtc);

  TAOX11_TEST_INFO << "Test get const UTC parts." << std::endl;
  const TimeBase::TimeT time1 = utc.time ();
  const uint32_t inacclo = utc.inacclo ();
  const uint16_t inacchi = utc.inacchi ();
  const TimeBase::TdfT tdf = utc.tdf ();
  TAOX11_TEST_INFO << " const UTC parts: time= " << time1 << ", inacclo= " << inacclo
    << ", inacchi= " << inacchi << ", tdf= " << tdf << std::endl;

  TAOX11_TEST_INFO << "Test moving UtcT to Any." << std::endl;
  // Cover non copying version of move UtcT
  TimeBase::UtcT getUtcMove = hello->get_UtcT();
  CORBA::Any anyUtc;
  anyUtc <<= std::move (getUtcMove);
  // Cover copying version of move UtcT
  const TimeBase::UtcT getUtcConst = hello->get_UtcT();
  anyUtc <<= getUtcConst;
  // Cover extraction of UtcT
  bool resultUtc = (anyUtc >>= getUtc);
  if (!resultUtc)
  {
    TAOX11_TEST_INFO << "Extraction of UtcT failed." << std::endl;
    ++result_;
  }
  // Cover streaming of UtcT
  TAOX11_TEST_INFO << "Test stream UtcT to std::ostream: " << getUtc << std::endl;
}

void testIntervalT(IDL::traits<Test::Hello>::ref_type & hello, int & result_)
{
  TAOX11_TEST_INFO << "Test IntervalT get/set." << std::endl;
  TimeBase::IntervalT getItv = hello->get_IntervalT();
  if (!equal(getItv, itvTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->get_IntervalT() return value wrong "
      << getItv << std::endl;
    ++result_;
  }
  TimeBase::IntervalT itv(itvTestValue1);
  hello->set_IntervalT(itv);

  TAOX11_TEST_INFO << "Test IntervalT type out parameter." << std::endl;
  TimeBase::IntervalT outItv;
  hello->out_IntervalT(outItv);
  if (!equal(outItv, itvTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->out_IntervalT() return value wrong "
      << outItv << std::endl;
    ++result_;
  }

  TAOX11_TEST_INFO << "Test IntervalT type inout parameter." << std::endl;
  TimeBase::IntervalT inoutItv(itvTestValue1);

  hello->inout_IntervalT(inoutItv);
  if (!equal(inoutItv, itvTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->out_IntervalT() return value wrong "
      << inoutItv << std::endl;
    ++result_;
  }

  TAOX11_TEST_INFO << "Test IntervalT type attribute." << std::endl;
  if (!equal(hello->itv(), itvTestValue2))
  {
    TAOX11_TEST_INFO << "ERROR: hello->itv() return value wrong "
      << hello->itv() << std::endl;
    ++result_;
  }
  hello->itv(itvTestValue1);

  TAOX11_TEST_INFO << "Test IntervalT type swap method." << std::endl;
  itv.swap(inoutItv);

  TAOX11_TEST_INFO << "Test get const IntervalT lower and upper bound." << std::endl;
  const TimeBase::TimeT timeLower = itv.lower_bound ();
  const TimeBase::TimeT timeUpper = itv.upper_bound ();
  TAOX11_TEST_INFO << " const IntervalT lower bound = " << timeLower << ", upper bound = " << timeUpper << std::endl;

  TAOX11_TEST_INFO << "Test moving IntervalT to Any." << std::endl;
  // Cover non copying version of move IntervalT
  TimeBase::IntervalT getItvMove = hello->get_IntervalT();
  CORBA::Any anyItv;
  anyItv <<= std::move(getItvMove);
  // Cover copying version of move IntervalT
  const TimeBase::IntervalT getItvConst = hello->get_IntervalT();
  anyItv <<= getItvConst;
  // Cover extraction of IntervalT
  bool resultItv = ( anyItv >>= getItv);
  if (!resultItv)
  {
    TAOX11_TEST_INFO << "Extraction of IntervalT failed." << std::endl;
    ++result_;
  }
  // Cover streaming of IntervalT
  TAOX11_TEST_INFO << "Test stream IntervalT to std::ostream: " << getItv << std::endl;
}

int main(int argc, char* argv[])
{
  int result_ = 0;
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);
    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR
        << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
        << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object(
        "file://test.ior");
    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: UtcT_to_object(<ior>) returned null reference."
        << std::endl;
      return 1;
    }
    TAOX11_TEST_INFO << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);
    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
        << std::endl;
      return 1;
    }
    TAOX11_TEST_INFO << "narrowed Hello interface" << std::endl;

    // Cover methods from Test::Hello
    TAOX11_TEST_INFO << "Test stream object to std::ostream: " << hello << std::endl;

    testUtcT(hello, result_);
    testIntervalT(hello, result_);

    TAOX11_TEST_INFO << "shutting down...";
    hello->shutdown();
    _orb->destroy();
    TAOX11_TEST_INFO << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result_;
  }
  return result_;
}
