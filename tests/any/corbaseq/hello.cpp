/**
 * @file    hello.cpp
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

Hello::~Hello()
{
}

CORBA::Any
Hello::test (
  const CORBA::Any &input)
{
  return input;
}

CORBA::Any
Hello::test_wcharseq (
  const CORBA::Any &input)
{
  TAOX11_TEST_DEBUG << "Test::Hello::test_wcharseq start" << std::endl;

  CORBA::WCharSeq inseq;
  input >>= inseq;

  TAOX11_TEST_DEBUG << "inseq = " << inseq << std::endl;

  CORBA::Any output;
  output <<= inseq;

  TAOX11_TEST_DEBUG << "Test::Hello::test_wcharseq end" << std::endl;

  return output;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
