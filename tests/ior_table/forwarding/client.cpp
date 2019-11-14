/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testC.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");
uint16_t niterations = 10;
uint16_t nthreads = 1;
bool     do_shutdown = false;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = true;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = std::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "ERROR : usage -k <ior> -i <niterations>"
          << std::endl;
        return false;
      }
  return true;
}

class Worker final
  : public ACE_Task_Base
{
public:
  explicit Worker (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual int svc () override;

  void run_test ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};


Worker::Worker (IDL::traits<CORBA::ORB>::ref_type orb)
  :  orb_ (std::move(orb))
{
}

int
Worker::svc ()
{
  try
    {
      this->run_test ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected exception caught in thread: "
        << ex << std::endl;
    }

  return 0;
}

void
Worker::run_test ()
{
  for (uint16_t j = 0; j < niterations; ++j)
    {
      IDL::traits<CORBA::Object>::ref_type obj =
        this->orb_->string_to_object (ior);

      bool is_simple_server =
        obj->_is_a ("IDL:Simple_Server:1.0");
      if (!is_simple_server)
        TAOX11_TEST_ERROR << "ERROR: Unexpected result from _is_a ()"
          << std::endl;
    }

  IDL::traits<CORBA::Object>::ref_type obj =
    this->orb_->string_to_object (ior);

  IDL::traits<Simple_Server>::ref_type server =
    IDL::traits<Simple_Server>::narrow (obj);
  if (server == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: Object reference is nil"
        << std::endl;
      return;
    }

  for (uint16_t j = 0; j < niterations; ++j)
    {
      if (server->test_is_a ("IDL:Foo:1.0"))
        TAOX11_TEST_ERROR << "ERROR: Unexpected result for "
          << "'_is_a (IDL::Foo::1.0)'" << std::endl;
    }
}

int
main (int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      Worker worker (orb);

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
      {
        TAOX11_TEST_ERROR << "ERROR: Cannot activate worker threads"
          << std::endl;
        return 1;
      }

      std::chrono::seconds tv (5);
      orb->run (tv);

      // Now run a test in the main thread, just to confuse matters a
      // little more.
      worker.run_test ();

      worker.thr_mgr ()->wait ();

      if (do_shutdown)
        {
          IDL::traits<CORBA::Object>::ref_type obj =
            orb->string_to_object (ior);

          IDL::traits<Simple_Server>::ref_type simple_server =
            IDL::traits<Simple_Server>::narrow (obj);

          simple_server->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR (client): Caught unexpected exception : "
        << ex << std::endl;
      return 1;
    }

  return 0;
}
