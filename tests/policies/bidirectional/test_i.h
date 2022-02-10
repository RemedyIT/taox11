/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Bidir test implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_BIDIRECTIONAL_TEST_I_H
#define TAOX11_BIDIRECTIONAL_TEST_I_H

#include "testS.h"

class Callback_i
  : public  CORBA::servant_traits<Callback>::base_type
{
public:
  Callback_i (IDL::traits<CORBA::ORB>::ref_type orb);

  void shutdown ();

  void callback_method ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

class Simple_Server_i
  : public virtual CORBA::servant_traits<Simple_Server>::base_type
{
public:
  Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb,
                   int no_iterations);

  int32_t test_method (bool do_callback) override;

  void callback_object (IDL::traits<Callback>::ref_type callback) override;

  void shutdown () override;

  bool call_client ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;

  bool flag_;

  IDL::traits<Callback>::ref_type callback_;

  int no_iterations_;
};

#endif /* TAOX11_BIDIRECTIONAL_TEST_I_H */
