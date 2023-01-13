/**
 * @file    shutdown_helper.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "shutdown_helper.h"

Shutdown_Helper::Shutdown_Helper (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

void
Shutdown_Helper::shutdown ()
{
  this->orb_->shutdown (false);
}
