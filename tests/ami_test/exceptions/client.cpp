/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_testAmiC.h"
#include <thread>

#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int16_t result = 0;
int16_t callback = 0;
int16_t callback_excep = 0;

// Flag indicates that all replies have been received
static int16_t received_all_replies = 0;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
       case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -k <ior>"
               << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

class Handler
: public virtual CORBA::amic_traits<A::AMI_Test>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void foo (int32_t ami_return_val) override
  {
    callback++;
    // Last reply flips the flag.
    if (ami_return_val == 931235)
      {
        received_all_replies = 1;
      }
  }

  void foo_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    TAOX11_TEST_INFO << "Callback method <foo_excep> called: "
                << "Testing proper exception handling."<< std::endl;
   try
      {
        excep_holder->raise_exception ();
      }
    catch (const A::DidTheRightThing& ex)
      {
        callback_excep++;
        TAOX11_TEST_INFO << "Callback method <foo_excep> exception received successfully <"
                    << ex << ">." << std::endl;
        if (ex.id() != 42)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <foo_excep> ex.id not 42 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
        if (ex.whatDidTheRightThing () != "Hello world")
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <foo_excep> ex.whatDidTheRightThing not ok: "
              << ex.whatDidTheRightThing() << std::endl;
            result = 1;
          }
      }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "Callback method <foo_excep> caught the wrong exception -> ERROR"
          << std::endl;
        result = 1;
      }

  }

  void foo_no_raises (int32_t /*ami_return_val*/) override
  {
    callback++;
  }

  void foo_no_raises_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    TAOX11_TEST_INFO
       << "Callback method <foo_no_raises_excep> called: "
       << "Testing proper exception handling."<< std::endl;
    //Expect not an A::DidTheRightThing exception, but a UNKNOWN exception.
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const A::DidTheRightThing&)
      {
        TAOX11_TEST_ERROR
          << "Callback method <foo_no_raises_excep> caught the wrong exception -> ERROR"
          << std::endl;
        result = 1;
      }
    catch (const CORBA::Exception& exc)
      {
        callback_excep++;
        TAOX11_TEST_INFO
            << "Callback method <foo_no_raises_excep> caught exception : <"
            << exc << ">"<< std::endl;
        std::stringstream excep_std;
        excep_std << exc;
        std::string excep_str = excep_std.str();
        if (excep_str.find ("UNKNOWN") == std::string::npos)
          {
            TAOX11_TEST_ERROR
              << "ERROR: foo_no_raises_excep() returned wrong CORBA::Exception: "
              << excep_str << std::endl;
            result = 1;
          }
      }

  }

  void bar (int32_t /*ami_return_val*/) override
    {
      callback++;
    }

  void bar_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    TAOX11_TEST_INFO << "Callback method <bar_excep> called: "
                << "Testing proper exception handling ..."<< std::endl;
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const A::DidTheRightThing& ex)
      {
        TAOX11_TEST_ERROR
          << "Callback method <bar_excep> caught the wrong exception -> ERROR"
          << ex << std::endl;
        result = 1;
      }
    catch (const A::DidAnOtherThing& exc)
      {
        callback_excep++;
        TAOX11_TEST_INFO << "Callback method <bar_excep> exception received successfully <"
                    << exc << ">." << std::endl;
        if (exc.id() != 42)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <bar_excep> exc.id not 42 but "
              << exc.id() <<  std::endl;
            result = 1;
          }
        if (exc.whatDidAnOtherThing () != L"Hello there!")
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <bar_excep> exc.whatDidAnOtherThing not ok: "
              << exc.whatDidAnOtherThing() << std::endl;
            result = 1;
          }
        }
      catch (const CORBA::Exception& )
        {
          TAOX11_TEST_ERROR
             << "Callback method <bar_excep> caught the wrong exception -> ERROR"
             << std::endl;
          result = 1;
        }
    }

    void shutdown () override
  {
  }

  void shutdown_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
  }
};

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
        {
          TAOX11_TEST_ERROR <<  "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                       << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "retrieved object reference" << std::endl;

      CORBA::amic_traits<A::AMI_Test>::ref_type ami_test_var = CORBA::amic_traits<A::AMI_Test>::narrow (obj);
      if (!ami_test_var)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::AMI_Test>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed A::AMI_Test interface" << std::endl;


      // Instantiate the ReplyHandler and register that with the POA.

      IDL::traits<CORBA::Object>::ref_type poa_obj =
           _orb->resolve_initial_references ("RootPOA");

      if (!poa_obj)
         {
           TAOX11_TEST_ERROR
             << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
             << std::endl;
           return 1;
         }

      TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa =
           IDL::traits<PortableServer::POA>::narrow (poa_obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
             << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
             << std::endl;
           return 1;
        }

      TAOX11_TEST_INFO << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
           root_poa->the_POAManager ();

      if (!poaman)
         {
           TAOX11_TEST_ERROR
             << "ERROR: root_poa->the_POAManager () returned null object."
             << std::endl;
           return 1;
         }

      CORBA::amic_traits<A::AMI_Test>::replyhandler_servant_ref_type test_handler_impl =
           CORBA::make_reference<Handler> ();

      TAOX11_TEST_INFO << "created TestHandler servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (test_handler_impl);

      TAOX11_TEST_INFO << "activated TestHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_obj =
           root_poa->id_to_reference (id);

      if (test_handler_obj == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: root_poa->id_to_reference (id) returned null reference."
             << std::endl;
           return 1;
        }

      CORBA::amic_traits<A::AMI_Test>::replyhandler_ref_type test_handler =
          CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow (test_handler_obj);

      if (test_handler == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow (test_handler_obj) returned null reference."
             << std::endl;
          return 1;
        }

      poaman->activate ();

      // Trigger the DidTheRightThing exception on the server side
      // by sending 0 to it.
      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_foo to trigger an exception" << std::endl;
      ami_test_var->sendc_foo (test_handler,0);

      TAOX11_TEST_INFO << "Client: Sending asynch message foo." << std::endl;
      ami_test_var->sendc_foo (test_handler, 1);

      // Trigger an exceptionon the server side
      // by sending 0 to it.
      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_foo_no_raises to trigger an exception" << std::endl;
      ami_test_var->sendc_foo_no_raises (test_handler,0);

      TAOX11_TEST_INFO << "Client: Sending asynch message foo_no_raises." << std::endl;
      ami_test_var->sendc_foo_no_raises (test_handler, 1);

      // Trigger the DidAnOtherThing exception on the server side
      // by sending 0 to it.
      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_bar to trigger an exception" << std::endl;
      ami_test_var->sendc_bar (test_handler,0);

      TAOX11_TEST_INFO << "Client: Sending asynch message bar." << std::endl;
      ami_test_var->sendc_bar (test_handler, 1);

      TAOX11_TEST_INFO << "Client: Sending the last  asynch message foo." << std::endl;
      ami_test_var->sendc_foo (test_handler, 931235);

      TAOX11_TEST_INFO << "Client: Do something else before coming back for the replies." << std::endl;

      for (int i = 0; i < 10; i ++)
        {
          TAOX11_TEST_INFO << " ...";
          std::this_thread::sleep_for (std::chrono::milliseconds (10));
        }

      TAOX11_TEST_INFO << std::endl << "Client: Now let's look for the replies." << std::endl;

      std::this_thread::sleep_for (std::chrono::seconds (2));
      while (!received_all_replies)
        {
          bool pending = _orb->work_pending();
          if (pending)
            {
              _orb->perform_work ();
            }
        }

      if ((callback != 4) || (callback_excep != 3 ))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks. Callbacks expected -4- , received -" << callback << "-"
          << " Exceptions: expected -3-, received -" << callback_excep << "-."<< std::endl;
          result = 1;
        }

      ami_test_var->shutdown ();

      root_poa->destroy (true, false);

      _orb->destroy ();
    }
  catch (const std::exception& e)
      {
        TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
        return 1;
      }
  return result;
}
