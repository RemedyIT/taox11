/**
 * @file    sleep_service.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "sleep_service.h"
#include <thread>

#include "testlib/taox11_testlog.h"

Sleep_Service::Sleep_Service (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

void
Sleep_Service::go_to_sleep (uint32_t microseconds)
{
  std::this_thread::sleep_for (std::chrono::microseconds (microseconds));
}

void
Sleep_Service::shutdown ()
{
  TAOX11_TEST_DEBUG << "Shutting down the ORB" << std::endl;

  this->orb_->shutdown (false);
}
