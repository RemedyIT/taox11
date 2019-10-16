/* -------------------------------------------------------------------
 * @file    hello.cpp
 * @author  Marcel Smit
 *
 * @brief   TAOX11 Naming service test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * -------------------------------------------------------------------*/
#include "hello.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

std::string
Hello::get_string ()
{
  return "Hello there!";
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
