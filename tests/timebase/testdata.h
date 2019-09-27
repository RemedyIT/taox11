/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "tao/x11/TimeBaseC.h"

// UtcT

TimeBase::TimeT timeTestValue1 (111111111);
TimeBase::UtcT utcTestValue1 (timeTestValue1, 1111, 1111, 11);
TimeBase::TimeT timeTestValue2 (999999999);
TimeBase::UtcT utcTestValue2 (timeTestValue2, 9999, 9999, 99);

TimeBase::IntervalT itvTestValue1 (2222222, 2222222);
TimeBase::IntervalT itvTestValue2 (8888888, 8888888);

bool
equal(const TimeBase::UtcT& utc, const TimeBase::UtcT& utc2)
{
  const TimeBase::UtcT copy = utc;
  bool dummy = copy.time () == utc2.time ()
      && copy.inacchi () == utc2.inacchi ()
      && copy.inacclo () == utc2.inacclo () && copy.tdf () == utc2.tdf ();
  return dummy && utc.time () == utc2.time ()
      && utc.inacchi () == utc2.inacchi () && utc.inacclo () == utc2.inacclo ()
      && utc.tdf () == utc2.tdf ();
}

bool
equal(const TimeBase::IntervalT & itv, const TimeBase::IntervalT & itv2)
{
  return itv.lower_bound () == itv2.lower_bound ()
      && itv.upper_bound () == itv2.upper_bound ();
}

