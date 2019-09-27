/**
 * @file    hello.cpp
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// FUZZ: disable check_for_ace_log_categories

#include "hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Hello::~Hello()
{
}

::CORBA::Any *
Hello::test (
  const CORBA::Any& input)
{
  CORBA::Any * output = new CORBA::Any (input);
  return output;
}


CORBA::Any *
Hello::test_wcharseq (
  const CORBA::Any &input)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Hello::test_wcharseq start\n"));

  const CORBA::WCharSeq* inseq {};
  input >>= inseq;

  CORBA::Any * output = new CORBA::Any;
  if (inseq != 0)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Hello::test_wcharseq - inseq extracted : %u\n", inseq->length ()));
    (*output) <<= (*inseq);
  }
  else
  {
    ACE_ERROR ((LM_ERROR, "(%P|%t) - Hello::test_wcharseq - inseq extraction failed\n"));
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Hello::test_wcharseq end\n"));

  return output;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
