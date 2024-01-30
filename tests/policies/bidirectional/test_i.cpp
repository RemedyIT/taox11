/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Bidir test implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_i.h"

#include "testlib/taox11_testlog.h"

Callback_i::Callback_i (IDL::traits<CORBA::ORB>::ref_type orb)
  :  orb_ (std::move(orb))
{
}

void
Callback_i::shutdown ()
{
  TAOX11_TEST_DEBUG << "Performing clean shutdown" << std::endl;
  this->orb_->shutdown (false);
}

void
Callback_i::callback_method ()
{
  TAOX11_LOG_DEBUG ("Callback method called");
}


// ****************************************************************
Simple_Server_i::Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb,
                                  int no_iterations)
  :  orb_ (std::move(orb)),
     flag_ (false),
     callback_ (0),
     no_iterations_ (no_iterations)
{
}

int32_t
Simple_Server_i::test_method (bool do_callback)
{
  if (do_callback)
    {
      this->flag_ = true;
    }

  return 0;
}

void
Simple_Server_i::callback_object (IDL::traits<Callback>::ref_type callback)
{
  // Store the callback object
  this->callback_ = callback;
}

bool
Simple_Server_i::call_client ()
{
  if (this->flag_)
    {
      for (int times = 0; times < this->no_iterations_; ++times)
        {
          this->callback_->callback_method ();
        }

      this->callback_->shutdown ();
      this->flag_ = false;

      return true;
    }

  return false;
}


void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
