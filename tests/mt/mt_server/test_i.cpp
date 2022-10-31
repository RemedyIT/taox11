/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_i.h"
#include <thread>

#include "testlib/taox11_testlog.h"

Simple_Server_i::Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb)
  :  orb_ (std::move(orb))
{
}

int32_t
Simple_Server_i::test_method (int32_t x)
{
  TAOX11_LOG_DEBUG ("Request in thread " << x11_logger::threadid << " of process "<< x11_logger::processid);
  std::this_thread::sleep_for (std::chrono::milliseconds (15));
  return x;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
