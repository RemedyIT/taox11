/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

#include "iomanip"

int result_ = 0;
std::string ior = "file://test.ior";
int message_size = 2048;
int message_count = 10 * 1024;
int test_runs = 6;
int do_shutdown = 1;

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:b:i:n:x"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'k':
        ior = get_opts.opt_arg();
        break;

      case 'b':
        message_size = std::atoi(get_opts.opt_arg());
        break;

      case 'i':
        message_count = std::atoi(get_opts.opt_arg());
        break;

      case 'n':
        test_runs = std::atoi(get_opts.opt_arg());
        break;

      case 'x':
        do_shutdown = 1;
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
                            "-k <ior>\n"
                            "-b <message_size>\n"
                            "-i <message_count>\n"
                            "-n <test_repetitions>\n"
              << std::endl;

        return false;
    }
  // Indicates successful parsing of the command line
  return true;
}

int
main(int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
      return 1;
    }
    if (parse_args(argc, argv) == false)
      return 1;

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object(ior);

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
            << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    Test::Receiver_Factory::_ref_type factory = IDL::traits<Test::Receiver_Factory>::narrow (obj);

    if (!factory)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Receiver_Factory::_narrow (obj) returned null object."
            << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed factory interface" << std::endl;

    // Test
    ACE_High_Res_Timer::global_scale_factor_type gsf = ACE_High_Res_Timer::global_scale_factor();

    Test::Message message;

    for (int j = 0; j != test_runs; ++j)
    {
      TAOX11_TEST_DEBUG << "Testing with "
            << message_size << " bytes per message"
            << std::endl;

      //message.the_payload.length(message_size);
      message.the_payload().resize (message_size);
      Test::Receiver::_ref_type receiver = factory->create_receiver();

      ACE_hrtime_t start = ACE_OS::gethrtime();
      for (int i = 0; i != message_count; ++i)
      {
        message.message_id(i);
        receiver->receive_data(message);
      }

      receiver->done();
      ACE_hrtime_t elapsed_time = ACE_OS::gethrtime() - start;

      // convert to microseconds
#if !defined ACE_LACKS_LONGLONG_T
      ACE_UINT32 usecs = ACE_UINT32(elapsed_time / gsf);
#else  /* ! ACE_LACKS_LONGLONG_T */
      ACE_UINT32 usecs = elapsed_time / gsf;
#endif /* ! ACE_LACKS_LONGLONG_T */

      double bytes = (1000000.0 * message_count * message_size) / usecs;
      double kbytes = bytes / 1024;
      double mbytes = kbytes / 1024;
      double mbits = bytes * 8 / 1000000;

      TAOX11_TEST_DEBUG << "Sender[" << message_size
            << "] " << std::setprecision (6) << bytes
            << " (bytes/sec), " << std::setprecision (4) << kbytes
            << " (kb/sec)" << std::endl;
      TAOX11_TEST_DEBUG << "Sender[" << message_size
            << "] " << std::setprecision (6) << mbytes
            << " (Mb/sec), " << std::setprecision (4) << mbits
            << " (Mbits)" << std::endl;

      message_size *= 2;
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down...";
    if (do_shutdown)
    {
      factory->shutdown();
    }
    _orb->destroy ();
    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result_;
  }

  return result_;
}
