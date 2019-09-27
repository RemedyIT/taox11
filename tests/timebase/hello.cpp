/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

bool
Hello::equal_const(const TimeBase::IntervalT & itv,
    const TimeBase::IntervalT & itv2) const
{
  return itv.lower_bound () == itv2.lower_bound ()
      && itv.upper_bound () == itv2.upper_bound ();
}

bool
Hello::equal_const(const TimeBase::UtcT& utc, TimeBase::UtcT& utc2) const
{
  return utc.time () == utc2.time () && utc.inacchi () == utc2.inacchi ()
      && utc.inacclo () == utc2.inacclo () && utc.tdf () == utc2.tdf ();
}

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
    orb_ (orb), result_ (result)
{
}

TimeBase::UtcT
Hello::get_UtcT()
{
  TimeBase::UtcT utc (utcTestValue2);
  return utc;
}

void
Hello::set_UtcT(const TimeBase::UtcT& utc)
{
  if (!equal (utc, utcTestValue1))
  {
    TAOX11_TEST_INFO << "ERROR: Hello::set_UtcT parameter value wrong "
        << utc << std::endl;
    ++result_;
  }
}

void
Hello::out_UtcT(TimeBase::UtcT& utc)
{
  utc = get_UtcT ();
}

void
Hello::inout_UtcT(TimeBase::UtcT& utc)
{
  equal_const(utc, utcTestValue1);
  if (!equal (utc, utcTestValue1))
  {
    TAOX11_TEST_INFO << "ERROR: Hello::inout_UtcT parameter value wrong "
        << utc << std::endl;
    ++result_;
  }
  TAOX11_TEST_INFO << "Trace utc1: " << utc << std::endl;
  utc = get_UtcT ();
  TAOX11_TEST_INFO << "Trace utc2: " << utc << std::endl;
}

TimeBase::UtcT
Hello::utc()
{
  return get_UtcT ();
}

void
Hello::utc(const TimeBase::UtcT& _v)
{
  equal_const(_v, utcTestValue1);
  if (!equal (_v, utcTestValue1))
  {
    TAOX11_TEST_INFO << "ERROR: Hello::utc parameter value wrong " << _v << std::endl;
    ++result_;
  }
}

// IntervalT

TimeBase::IntervalT
Hello::get_IntervalT()
{
  TimeBase::IntervalT itv (itvTestValue2);
  return itv;
}

void
Hello::set_IntervalT(const TimeBase::IntervalT& itv)
{
  if (!equal (itv, itvTestValue1) || !this->equal_const(itv, itvTestValue1))
  {
    TAOX11_TEST_INFO << "ERROR: Hello::set_IntervalT parameter value wrong "
        << itv << std::endl;
    ++result_;
  }
}

void
Hello::out_IntervalT(TimeBase::IntervalT& itv)
{
  itv = get_IntervalT ();
}

void
Hello::inout_IntervalT(TimeBase::IntervalT& itv)
{
  if (!equal (itv, itvTestValue1))
  {
    TAOX11_TEST_INFO << "ERROR: Hello::inout_IntervalT parameter value wrong "
        << itv << std::endl;
    ++result_;
  }
  TAOX11_TEST_INFO << "Trace itv1: " << itv << std::endl;
  itv = get_IntervalT ();
  TAOX11_TEST_INFO << "Trace itv2: " << itv << std::endl;
}

TimeBase::IntervalT
Hello::itv()
{
  return get_IntervalT ();
}

void
Hello::itv(const TimeBase::IntervalT& _v)
{
  if (!equal (_v, itvTestValue1))
  {
    TAOX11_TEST_INFO << "ERROR: Hello::itv parameter value wrong " << _v << std::endl;
    ++result_;
  }
}

void
Hello::shutdown()
{
  this->orb_->shutdown (false);
}

