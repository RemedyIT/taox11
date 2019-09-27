/**
 * @file    shutdown_helper.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "shutdown_helper.h"

Shutdown_Helper::Shutdown_Helper (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

void
Shutdown_Helper::shutdown ()
{
  this->orb_->shutdown (false);
}
