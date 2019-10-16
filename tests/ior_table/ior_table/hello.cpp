/**
 * @file    hello.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb,
  bool rebinded)
  : orb_ (orb)
  , rebinded_ (rebinded)
{
}

std::string
Hello::get_string ()
{
  if (!this->rebinded_)
    {
      return "ERROR";
    }
  return "OK";
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
