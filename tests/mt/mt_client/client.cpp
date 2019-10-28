/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "testlib/taox11_testlog.h"

std::string ior = "file://test.ior";
int nthreads = 5;
int niterations = 5;
bool server_shutdown = false;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:i:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        nthreads = std::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        niterations = std::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        server_shutdown = true;
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
              << "-k <ior>\n"
              << "-n <nthreads>\n"
              << "-i <niterations>\n"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

/// Run the client thread
class Client : public ACE_Task_Base
{
public:
  Client (IDL::traits<Simple_Server>::ref_type server,
          int niterations);

  virtual int svc ();

private:
  void validate_connection ();

private:
  IDL::traits<Simple_Server>::ref_type server_;

  int niterations_;
};

int
main (int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type object =
        orb->string_to_object (ior);

      IDL::traits<Simple_Server>::ref_type server =
        IDL::traits<Simple_Server>::narrow (object);

      if (!server)
        {
          TAOX11_TEST_ERROR << "Object reference <"
                << ior << "> is nil."
                << std::endl;
          return 1;
        }

      Client client (server, niterations);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
      {
        TAOX11_TEST_ERROR << "Cannot activate client threads"
              << std::endl;
        return 1;
      }

      client.thr_mgr ()->wait ();

      TAOX11_TEST_DEBUG << "threads finished" << std::endl;

      if (server_shutdown)
      {
        server->shutdown ();
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception <client::main> :"
            << ex << std::endl;
      return 1;
    }

  return 0;
}

// ****************************************************************

Client::Client (IDL::traits<Simple_Server>::ref_type server,
                int niterations)
  :  server_ (server),
     niterations_ (niterations)
{
}

void
Client::validate_connection ()
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!
  for (int j = 0; j != 100; ++j)
    {
      try
      {
        this->server_->test_method ();
      }
      catch (const CORBA::Exception&)
      {
      }
    }
}

int
Client::svc ()
{
  try
    {
      this->validate_connection ();

      for (int i = 0; i < this->niterations_; ++i)
        {
          this->server_->test_method ();

          if (i % 100 == 0)
            {
              TAOX11_LOG_DEBUG ("iteration = " << i);
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception <Client::svc> :" << ex << std::endl;
      return 1;
    }
  return 0;
}
