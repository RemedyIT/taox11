/**
 * @file    reactor.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 server application testing the usage
 *          of the ORB ACE Reactor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"
#include "tao/x11/orb.h"
#include "ace/Reactor.h"
#include "ace/Time_Value.h"

class TestHandler final
  : public ACE_Event_Handler
{
public:
  explicit TestHandler (IDL::traits<CORBA::ORB>::ref_type orb)
    : orb_ (std::move(orb)),
      timeout_triggered_ (false)
  {}

  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg) override;

  bool trigger_in (long seconds);

  bool timeout_triggered () const;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  bool timeout_triggered_;
};

bool
TestHandler::timeout_triggered () const
{
  return this->timeout_triggered_;
}

int
TestHandler::handle_timeout (const ACE_Time_Value &,
                             const void *)
{
  TAOX11_TEST_DEBUG << "TestHandler::handle_timeout - timeout triggered" << std::endl;
  this->timeout_triggered_ = true;
  this->orb_->shutdown (false);
  return 0;
}

bool
TestHandler::trigger_in (long seconds)
{
  return -1 != this->orb_->reactor ()->schedule_timer (this, 0, std::chrono::seconds {seconds});
}

int
main(int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      TestHandler test_handler (_orb);

      // trigger in 1 second
      if (!test_handler.trigger_in (1))
        {
          TAOX11_TEST_ERROR << "Error: unable to schedule trigger" << std::endl;
        }

      std::chrono::seconds timeout (2);
      _orb->run (timeout);

      TAOX11_TEST_INFO << "event loop finished" << std::endl;

      if (!test_handler.timeout_triggered ())
        {
          TAOX11_TEST_ERROR << "Error: timer handler did not trigger" << std::endl;
          ++retval;
        }

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }

  return retval;
}
