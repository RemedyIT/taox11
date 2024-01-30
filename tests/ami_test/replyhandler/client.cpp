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
#include "testAmiC.h"
#include <thread>

#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int16_t result = 0;
int16_t callback_attrib = 0;
int16_t callback_foo = 0;

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
  : public virtual CORBA::amic_traits<A::Test>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void foo (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Handler Callback method <foo> called: result <" << ami_return_val << ">."<< std::endl;
    callback_foo++;
    // Last reply flips the flag.
    if (ami_return_val == 931235)
      {
        received_all_replies = 1;
      }
    else if (ami_return_val != 3)
      {
        TAOX11_TEST_ERROR << "ERROR: Handler ami_return_val not 3: " << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Handler Unexpected callback method <foo_excep> called."<< std::endl;
    result = 1;
  }

  //=============
  void get_yadda (int32_t res) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Handler Callback method <get_yadda_no_r> called: res " << res << std::endl;
    if ((res != 33) && (res != 43))
    {
        TAOX11_TEST_ERROR << "ERROR: Handler ami_return_val not 33 or 43: " << res << std::endl;
        result = 1;
    }
  }

  void get_yadda_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Handler Unexpected callback method <get_yadda_excep> called." << std::endl;
    result = 1;
  }

  void set_yadda () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Handler Callback method <set_yadda> called:"<< std::endl;
  }

  void set_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Handler Unexpected callback method <set_yadda_excep> called." << std::endl;
    result = 1;
  }

  void shutdown () override
  {
  }

  void shutdown_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
  }
};

class Handler3 final
  : public virtual CORBA::amic_traits<A::Test>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler3 () = default;

  /// Destructor.
  ~Handler3 () = default;

  void foo (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Handler3: Callback method <foo> called: result <" << ami_return_val << ">."<< std::endl;
    callback_foo++;
    if (ami_return_val != 111)
      {
        TAOX11_TEST_ERROR << "ERROR: Handler3: ami_return_val not 111: " << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Handler3: Unexpected callback method <foo_excep> called."<< std::endl;
    result = 1;
  }


  //=============
  void get_yadda (int32_t res) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Handler3: Callback method <get_yadda_no_r> called: res " << res << std::endl;
    if (res != 333)
      {
        TAOX11_TEST_ERROR << "ERROR: ami_return_val not 333: " << res << std::endl;
        result = 1;
      }
  }

  void get_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Handler3: Unexpected callback method <get_yadda_excep> called." << std::endl;
    result = 1;
  }

  void set_yadda () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Handler3: Callback method <set_yadda> called:"<< std::endl;
  }

  void set_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Handler3: Unexpected callback method <set_yadda_excep> called." << std::endl;
    result = 1;
  }

  void shutdown () override
  {
  }

  void shutdown_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
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

      IDL::traits<A::Test>::ref_type a_sync = IDL::traits<A::Test>::narrow (obj);

      if (!a_sync)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<A::Test>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed A::Test interface" << std::endl;

      CORBA::amic_traits<A::Test>::ref_type a_async = CORBA::amic_traits<A::Test>::narrow (obj);
      if (!a_async)
         {
           TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Test>::narrow (obj) returned null object."
                        << std::endl;
           return 1;
         }

      // Instantiate the ReplyHandler and register that with the POA.
      IDL::traits<CORBA::Object>::ref_type poa_obj =
           _orb->resolve_initial_references ("RootPOA");

      if (!poa_obj)
         {
           TAOX11_TEST_ERROR
           << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
           return 1;
         }

      TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa =
           IDL::traits<PortableServer::POA>::narrow (poa_obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
             << "ERROR: IDL::traits<PortableServer::POA>::narrow (poa_obj) returned null object."
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
      //test_handler1 Handler
      CORBA::amic_traits<A::Test>::replyhandler_servant_ref_type test_handler_impl =
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

      CORBA::amic_traits<A::Test>::replyhandler_ref_type test_handler1 =
          CORBA::amic_traits<A::Test>::replyhandler_traits::narrow (test_handler_obj);

      if (test_handler1 == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::Test>::replyhandler_traits::narrow (test_handler_obj) returned null reference."
             << std::endl;
          return 1;

        }
      //test_handler2 , same as test_handler1: ref. of Handler
      CORBA::amic_traits<A::Test>::replyhandler_servant_ref_type test_handler_impl2 =
                 CORBA::make_reference<Handler> ();

      TAOX11_TEST_INFO << "created second TestHandler servant" << std::endl;

      PortableServer::ObjectId id2 = root_poa->activate_object (test_handler_impl2);

      TAOX11_TEST_INFO << "activated second TestHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_obj2 =
           root_poa->id_to_reference (id2);

      if (test_handler_obj2 == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: root_poa->id_to_reference (id2) returned null reference."
             << std::endl;
           return 1;
        }

      CORBA::amic_traits<A::Test>::replyhandler_ref_type test_handler2 =
          CORBA::amic_traits<A::Test>::replyhandler_traits::narrow (test_handler_obj2);

      if (test_handler2 == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::Test>::replyhandler_traits::narrow(test_handler_obj2) returned null reference."
             << std::endl;
          return 1;
        }

      //test_handler3  ref. from  Handler3
      CORBA::amic_traits<A::Test>::replyhandler_servant_ref_type test_handler_impl3 =
                       CORBA::make_reference<Handler3> ();

      TAOX11_TEST_INFO << "created third TestHandler servant" << std::endl;

      PortableServer::ObjectId id3 = root_poa->activate_object (test_handler_impl3);

      TAOX11_TEST_INFO << "activated TestHandler 3 servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_obj3 =
           root_poa->id_to_reference (id3);

      if (test_handler_obj3 == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: root_poa->id_to_reference (id3) returned null reference."
             << std::endl;
           return 1;
        }


      CORBA::amic_traits<A::Test>::replyhandler_ref_type test_handler3 =
          CORBA::amic_traits<A::Test>::replyhandler_traits::narrow (test_handler_obj3);

      if (test_handler3 == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::Test>::replyhandler_traits::narrow(test_handler_obj3) returned null reference."
             << std::endl;
          return 1;
        }

      poaman->activate ();

      TAOX11_TEST_INFO << "Client: Sending synch message foo." << std::endl;
      int32_t res = a_sync->foo(22);
      if (res != 22)
        {
          TAOX11_TEST_ERROR
              << "ERROR: a_sync->foo didn't return <22> but <" << res << ">."
              << std::endl;
          result =1;
        }


      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_foo,sendc_get_yadda,sendc_set_yadda"
                  << " without a replyhandler registered." << std::endl;
      try
        {
          // Try out sending asynchronous messages without a replyhandler
          // registered. Things fail if we get an exception.
          a_async->sendc_foo (nullptr ,12);
          a_async->sendc_get_yadda (nullptr /*test_handler*/);
          a_async->sendc_set_yadda (nullptr /*test_handler*/, 103);
          // 0 should generate a exception, but without a reply handler
          // registered, we don't excpect receiving the exception.
          a_async->sendc_foo (nullptr ,0);
        }
      catch (const CORBA::Exception& ex)
        {
          TAOX11_TEST_ERROR << "Sending asynch messages without a replyhandler, "
                       << "shouldn't throw an exception -> ERROR " << ex << std::endl;
          result = 1;
        }

      TAOX11_TEST_INFO << "Client: Sending synch message foo." << std::endl;
      res = a_async->foo(222);
      if (res != 222)
        {
          TAOX11_TEST_ERROR
              << "ERROR: a_sync->foo didn't return <222> but <" << res << ">."
              << std::endl;
          result =1;
        }

      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_set_yadda with test_handler1." << std::endl;
      a_async->sendc_set_yadda (test_handler1, 43);

      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_get_yadda with test_handler2" << std::endl;
      a_async->sendc_get_yadda (test_handler2);

      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_set_yadda with test_handler2" << std::endl;
      a_async->sendc_set_yadda (test_handler2, 33);

      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_get_yadda with test_handler1" << std::endl;
      a_async->sendc_get_yadda (test_handler1);

      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_set_yadda with test_handler3" << std::endl;
      a_async->sendc_set_yadda (test_handler3, 333);

      TAOX11_TEST_INFO << "Client: Sending asynch message sendc_get_yadda with test_handler3." << std::endl;
      a_async->sendc_get_yadda (test_handler3);

      TAOX11_TEST_INFO << "Client: Sending asynch message foo with test_handler3" << std::endl;
      a_async->sendc_foo (test_handler3, 111);

      TAOX11_TEST_INFO << "Client: Sending the last  asynch message foo." << std::endl;
      a_async->sendc_foo (test_handler2, 10);

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
         _orb->perform_work ();
        }

      if ((callback_foo != 2) || (callback_attrib != 6 ))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks. Foo : expected -2- , received -" << callback_foo << "-"
          << " Attrib : expected -6-, received -" << callback_attrib << "-."
          << std::endl;
          result = 1;
        }

      a_sync->shutdown ();

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
