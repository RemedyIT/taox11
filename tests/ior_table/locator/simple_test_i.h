/**
 * @file    simple_test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef SIMPLE_TEST_I_H
#define SIMPLE_TEST_I_H

#include "simple_testS.h"

class Simple_Test_i final
  : public virtual CORBA::servant_traits<Simple::SimpleTest>::base_type
{
public:
  /// Constructor
  Simple_Test_i (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual void shutdown ();

private:
  Simple_Test_i (const Simple_Test_i&) = delete;
  Simple_Test_i (Simple_Test_i&&) = delete;
  Simple_Test_i& operator= (const Simple_Test_i&) = delete;
  Simple_Test_i& operator= (Simple_Test_i&&) = delete;

  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* SIMPLE_TEST_I_H */
