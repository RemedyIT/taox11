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

#include "tao/x11/log.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int16_t result = 0;

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
        taox11_error << "usage: -k <ior>"
                     << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

class Handler : public virtual CORBA::amic_traits<Test::A>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () override = default;

  void foo (int32_t ami_return_val) override
  {
    taox11_info << "Callback method <foo> called: result <"
                << ami_return_val << ">."<< std::endl;
    // Last reply flips the flag.
    if (ami_return_val == 931235)
      {
        received_all_replies = 1;
      }
  }

  void foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    taox11_info << "Callback method <foo_excep> called."<< std::endl;
  }

  // callbacks attributes
  void get_yadda (int32_t res) override
  {
    taox11_info << "Callback method <get_yadda> called: res " << res
                << std::endl;
  }

  void get_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    taox11_info << "Callback method <get_yadda_excep> called." << std::endl;
  }

  void set_yadda () override
  {
    taox11_info << "Callback method <set_yadda> called:"<< std::endl;
  }

  void set_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    taox11_info << "Callback method <set_yadda_excep> called." << std::endl;
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
  // by default show all messages logged through global logger
  X11_LOGGER::priority_mask (x11_logger::X11_LogMask::LP_ALL);

  try
    {
      // Initialize the ORB
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
        {
          taox11_error
                << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      // Register a replyhandler servant for the Test::A interface with the root POA .
      // First get a reference to the POA , then create the replyhandler servant,
      // activate this servant in the root POA and obtain and narrow its object reference.
      //Acivate the POA manager

      // Get reference to Root POA
      IDL::traits<CORBA::Object>::ref_type poa_obj =
           _orb->resolve_initial_references ("RootPOA");

      if (!poa_obj)
         {
           taox11_error
           << "ERROR: resolve_initial_references (\"RootPOA\") "
           << "returned null reference." << std::endl;
           return 1;
         }

      taox11_info << "retrieved RootPOA object reference" << std::endl;

      // Narrow the object to POA interface
      IDL::traits<PortableServer::POA>::ref_type root_poa =
           IDL::traits<PortableServer::POA>::narrow (poa_obj);

      if (!root_poa)
        {
          taox11_error
             << "ERROR: IDL::traits<PortableServer::POA>::narrow returned null object."
             << std::endl;
           return 1;
        }

      taox11_info << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
           root_poa->the_POAManager ();

      if (!poaman)
         {
           taox11_error
             << "ERROR: root_poa->the_POAManager () returned null object."
             << std::endl;
           return 1;
         }

      // Create  an instance of the replyhandler Handler servant
      CORBA::amic_traits<Test::A>::replyhandler_servant_ref_type test_handler_impl =
           CORBA::make_reference<Handler> ();

      taox11_info << "created replyhandler Handler servant" << std::endl;

      // Activate the instance of the replyhandler Handler servant in the POA
      PortableServer::ObjectId id =
            root_poa->activate_object (test_handler_impl);

      taox11_info << "activated replyhandler Handler servant" << std::endl;

      //  Get a object reference of the replyhandler Handler servant from the POA
      IDL::traits<CORBA::Object>::ref_type test_handler_obj =
            root_poa->id_to_reference (id);

      if (test_handler_obj == nullptr)
        {
          taox11_error
             << "ERROR: root_poa->id_to_reference (id) returned null reference."
             << std::endl;
           return 1;
        }

      // Narrow the object reference to the replyhandler interface.
      CORBA::amic_traits<Test::A>::replyhandler_ref_type test_handler =
        CORBA::amic_traits<Test::A>::replyhandler_traits::narrow(test_handler_obj);

      if (test_handler == nullptr)
        {
          taox11_error
             << "ERROR: CORBA::amic_traits<Test::A>::replyhandler_traits::narrow"
             << "(test_handler_obj) returned null reference."
             << std::endl;
          return 1;
        }

      // Activate POA manager
      poaman->activate ();

      // At this moment we have a , in the POA registered,  replyhandler 'test_handler'.

      // Next step is retrieving the references to the synchronous- and
      // asynchronous interfaces,
      // by first retrieving the object reference from the ior file and then
      // narrow this to both the synchronous- and asynchronous interface.

      // Retrieve the object reference
      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          taox11_error << "ERROR: string_to_object(<" << ior << ">) returned "
                       << "null reference."
                       << std::endl;
          return 1;
        }

      taox11_info << "retrieved object reference" << std::endl;

      // Narrow the object reference to the synchronous interface Test::A
      IDL::traits<Test::A>::ref_type a_sync =
           IDL::traits<Test::A>::narrow (obj);
      if (!a_sync)
        {
          taox11_error << "ERROR: IDL::traits<Test::A>::narrow returned null object."
                       << std::endl;
          return 1;
        }

      taox11_info << "narrowed obj to synchronous interface A::Test."
                  << std::endl;


      // Narrow the object reference to the asynchronous interface of Test::A
      CORBA::amic_traits<Test::A>::ref_type a_async =
           CORBA::amic_traits<Test::A>::narrow (obj);
      if (!a_async)
        {
          taox11_error << "ERROR: CORBA::amic_traits<A::TestAmiC>::narrow (obj)"
                       <<"returned null object." << std::endl;
          return 1;
        }
     taox11_info << "narrowed obj to asynchronous interface A::Test ."
                   << std::endl;

     // At this moment we have an object reference 'a_sync' to
     // the synchronous interface of Test::A and an object reference 'a_async'
     // to the asynchronous interface of Test::A


     // Now lets invoke asynchronous and synchronous calls.

     // Invoke synchronous message foo, via reference a_sync to synchronous interface.
     // Wait for the reply.
      taox11_info
         << "Client: Sending synchronous message foo via synchronous interface,"
         << std::endl << "wait for the reply."
         << std::endl;
      int32_t res = a_sync->foo(22);
      if (res != 3)
        {
          taox11_error
              << "ERROR: a_sync->foo didn't return <3> but <" << res << ">."
              << std::endl;
          result =1;
        }

      // Invoke asynchronous message foo,  via reference a_async, don't wait for the reply.
      taox11_info << "Client: Sending asynchronous message foo." << std::endl;
      a_async->sendc_foo (test_handler,12);

      // Invoke synchronous message foo, via asynchronous interface (a_async).
      // Wait for the reply.
      taox11_info
        << "Client: Sending synchronous message foo via asynchronous interface."
        << std::endl << "wait for the reply."
        << std::endl;
      res = a_async->foo(222);
      if (res != 3)
        {
          taox11_error
              << "ERROR: a_async->foo didn't return <3> but <" << res << ">."
              << std::endl;
          result =1;
        }

      // Invoke asynchronous attribute methods.
      taox11_info
        << "Client: Sending asynchronous message sendc_get_yadda."
        << std::endl;
      a_async->sendc_get_yadda (test_handler);

      taox11_info
        << "Client: Sending asynchronous message sendc_set_yadda."
        << std::endl;
      a_async->sendc_set_yadda (test_handler, 103);

      // Invoke last asynchronous message foo, don't wait for the reply.
      taox11_info << "Client: Sending the last  asynchronous message foo."
                  << std::endl;
      a_async->sendc_foo (test_handler, 10);

      taox11_info
         << "Client: Do something else before coming back for the replies."
         << std::endl;

      for (int i = 0; i < 10; i ++)
        {
          taox11_info << " ...";
          std::this_thread::sleep_for (std::chrono::milliseconds (10));
        }

      taox11_info << std::endl << "Client: Now let's look for the replies."
                  << std::endl;

      std::this_thread::sleep_for (std::chrono::seconds (2));
      while (!received_all_replies)
        {
         _orb->perform_work ();
        }

      a_sync->shutdown ();

      root_poa->destroy (true, false);

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      taox11_error << "exception caught: " << e << std::endl;
      return 1;
    }
  return result;
}
