/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_FORWARDING_TEST_I_H
#define TAOX11_FORWARDING_TEST_I_H

#include "testS.h"

class Simple_Server_i final
  : public virtual CORBA::servant_traits<Simple_Server>::base_type
{
public:
  Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Simple_Server_i() = default;

  bool test_is_a (const std::string& type) override;

  void shutdown () override;

private:
  Simple_Server_i (const Simple_Server_i&) = delete;
  Simple_Server_i (Simple_Server_i&&) = delete;
  Simple_Server_i& operator= (const Simple_Server_i&) = delete;
  Simple_Server_i& operator= (Simple_Server_i&&) = delete;

  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAOX11_FORWARDING_TEST_I_H */
