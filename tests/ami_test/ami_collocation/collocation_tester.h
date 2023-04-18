/**
 * @file    collocation_tester.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 AMI Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_AMI_COLLOCATION_TEST_H
#define TAOX11_AMI_COLLOCATION_TEST_H

#pragma once

#include "ami_testAmiC.h"

class Handler
: public virtual CORBA::amic_traits<A::AMI_Test>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void foo (int32_t ami_return_val,
            int32_t out_l) override;
  void foo_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override;
};


class Collocation_Test
{
public:
  Collocation_Test ();

  int shutdown ();

  int init (int argc, ACE_TCHAR *argv[]);

  int run ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type root_poa_;
  IDL::traits<PortableServer::POAManager>::ref_type poa_manager_;

  CORBA::amic_traits<A::AMI_Test>::replyhandler_ref_type test_handler_;
  CORBA::amic_traits<A::AMI_Test>::ref_type ami_test_var_;

};

#endif /* TAOX11_AMI_COLLOCATION_TEST_H */
