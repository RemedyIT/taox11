/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ server application defines
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"
#include "tao/x11/TimeBaseS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, int& result);

  // = The skeleton methods
  virtual TimeBase::UtcT get_UtcT() override;
  virtual void set_UtcT(const TimeBase::UtcT& utc) override;
  virtual void out_UtcT(TimeBase::UtcT& utc) override;
  virtual void inout_UtcT(TimeBase::UtcT& utc) override;
  virtual TimeBase::UtcT utc() override;
  virtual void utc(const TimeBase::UtcT& _v) override;

  // = The skeleton methods
  virtual TimeBase::IntervalT get_IntervalT() override;
  virtual void set_IntervalT(const TimeBase::IntervalT& itv) override;
  virtual void out_IntervalT(TimeBase::IntervalT& itv) override;
  virtual void inout_IntervalT(TimeBase::IntervalT& itv) override;
  virtual TimeBase::IntervalT itv() override;
  virtual void itv(const TimeBase::IntervalT& _v) override;

  virtual void shutdown() override;

private:
  bool equal_const(const TimeBase::IntervalT & itv, const TimeBase::IntervalT & itv2) const;
  bool equal_const(const TimeBase::UtcT& utc, TimeBase::UtcT& utc2) const;
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int &result_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
