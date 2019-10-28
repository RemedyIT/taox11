/**
 * @file    log_test_backend.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 logging test for a custom backend
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"
#include "ace/Log_Msg_Backend.h"

class Backend : public ACE_Log_Msg_Backend
{
public:
  Backend () = default;
  ~Backend () = default;

  virtual int open (const ACE_TCHAR *logger_key) override;

  virtual int reset (void) override;

  virtual int close (void) override;

  virtual ssize_t log (ACE_Log_Record &log_record) override;

  // Test probes to see if things worked as specified.
  size_t log_count (void) const { return this->log_count_; }

private:
  size_t log_count_ {};
};

int
Backend::open (const ACE_TCHAR *)
{
  return 0;
}

int
Backend::reset (void)
{
  return 0;
}

int
Backend::close (void)
{
  return 0;
}

ssize_t
Backend::log (ACE_Log_Record &)
{
  ++this->log_count_;
  return 1;
}

int log_backend_test ()
{
  int retval {};

  Backend b;
  ACE_Log_Msg_Backend *old_b = ACE_Log_Msg::msg_backend (&b);

  // Make sure messages are getting routed correctly.
  TAOX11_TEST_DEBUG << "Message 1" << std::endl;
  TAOX11_TEST_DEBUG << "Message 2" << std::endl;
  TAOX11_TEST_DEBUG << "Message 3" << std::endl;

  // Reset the backend to avoid references to our soon-to-be-destroyed one.
  ACE_Log_Msg::msg_backend (old_b);

  size_t const count = b.log_count ();
  if (count != 3)
  {
    TAOX11_TEST_ERROR << "ERROR: Backend counted <" << count << " expected <3>" << std::endl;
    ++retval;
  }
  else
  {
    TAOX11_TEST_DEBUG << "OK: Backend counted <" << count << ">" << std::endl;
  }

  return retval;
}

int main(int /*argc*/, char** /*argv[]*/)
{
  int retval {};

  try
  {
    retval += log_backend_test();

    if (retval != 0)
    {
      TAOX11_TEST_ERROR << "Custom backend logging test failed" << std::endl;
    }

    TAOX11_TEST_INFO << "shutting down...";

    TAOX11_TEST_INFO << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    return 1;
  }
  return retval;
}
