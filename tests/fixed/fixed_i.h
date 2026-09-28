/**
 * @file fixed_i.h
 * @author Johnny Willemsen
 * @brief Servant for the IDL fixed decimal regression test
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef FIXED_I_H
#define FIXED_I_H

#include "testS.h"

class FixedTest_i final
  : public virtual CORBA::servant_traits<FixedTest>::base_type
{
public:
  explicit FixedTest_i(IDL::traits<CORBA::ORB>::ref_type orb);

  fixed_type echo_fixed(const fixed_type& value) override;
  fixed_array echo_fixed_array(const fixed_array& value) override;
  large_type echo_large(const large_type& value) override;
  pi_type echo_pi(const pi_type& value) override;
  V::F::f_type echo_fraction(const V::F::f_type& value) override;
  void shutdown() override;

  int errors() const { return this->errors_; }

private:
  void check(bool ok, const char* what);

  IDL::traits<CORBA::ORB>::ref_type orb_;
  int errors_ {};
};

#endif // FIXED_I_H
