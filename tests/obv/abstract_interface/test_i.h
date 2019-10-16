/**
 * @file    test_i.h
 * @author  Martin Corino
 *
 * @brief   test servant implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_TEST_I_H
#define TAOX11_TEST_I_H

#include "testS.h"
#include "tc_i.h"

class foo_i final : public virtual CORBA::servant_traits<foo>::base_type
{
public:
  virtual std::string foo_op (const std::string& inarg) override;

  virtual std::string base_op (const std::string& inarg) override;

  virtual int32_t rw_foo () override;

  virtual void rw_foo (int32_t a) override;

  virtual int32_t r_foo () override;
private:
  int32_t rw_foo_ {};
};

class passer_i final : public virtual CORBA::servant_traits<passer>::base_type
{
public:
  passer_i (IDL::traits<CORBA::ORB>::ref_type orb, IDL::traits<PortableServer::POA>::ref_type poa);

  virtual void pass_ops (IDL::traits< base>::ref_type&  outarg) override;

  virtual void pass_state (IDL::traits< base>::ref_type&  outarg) override;

  virtual void pass_nil (IDL::traits< base>::ref_type&  outarg) override;

  virtual void shutdown () override;
private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* TAOX11_TEST_I_H */
