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
int16_t callback_foo_two = 0;
int16_t callback_do = 0;
int16_t callback_sendc_foo = 0;
int16_t callback_sendc_ami_foo = 0;

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
    TAOX11_TEST_INFO << "Callback method <foo> called: result <" << ami_return_val
                << ">."<< std::endl;
    callback_foo++;
    // Last reply flips the flag.
    if (ami_return_val == 931235)
      {
        received_all_replies = 1;
      }
    else if (ami_return_val != 3)
      {
        TAOX11_TEST_ERROR << "ERROR: Callback method <foo> ami_return_val not 3: "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Unexpected callback method <foo_excep> called."<< std::endl;
    result = 1;
  }

  void sendc_foo (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <sendc_foo> called: result <"
                << ami_return_val << ">."<< std::endl;
    callback_sendc_foo++;
    // Last reply flips the flag.
    if (ami_return_val != 4)
      {
        TAOX11_TEST_ERROR << "ERROR: Callback method <sendc_foo> ami_return_val not 4: "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void sendc_foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Unexpected callback method <sendc_foo_excep> called."
                << std::endl;
    result = 1;
  }

  void sendc_ami_foo (int32_t ami_return_val) override
   {
     TAOX11_TEST_INFO << "Callback method <sendc_ami_foo> called: result <" << ami_return_val << ">."<< std::endl;
     callback_sendc_ami_foo++;
     if (ami_return_val != 5)
       {
         TAOX11_TEST_ERROR
            << "ERROR: Callback method <sendc_ami_foo> ami_return_val not 5: "
            << ami_return_val << std::endl;
         result = 1;
       }
   }

   void sendc_ami_foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO << "Error, Unexpected callback method <sendc_ami_foo_excep> called."<< std::endl;
     result = 1;
   }
  //=============
  void get_yadda (int32_t res) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <get_yadda> called: res " << res
                << std::endl;
    if (res != 110)
      {
        TAOX11_TEST_ERROR
           << "ERROR: Callback method <get_yadda> ami_return_val not 110: "
           << res << std::endl;
        result = 1;
      }
  }

  void get_yadda_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Unexpected callback method <get_yadda_excep> called."
                << std::endl;
    result = 1;
  }

  void set_yadda () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <set_yadda> called:"<< std::endl;
  }

  void set_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Unexpected callback method <set_yadda_excep> called."
                << std::endl;
    result = 1;
  }

  void get_sendc_get_yadda (int16_t res) override
   {
     callback_attrib++;
     TAOX11_TEST_INFO << "Callback method <get_sendc_yadda> called: res " << res
                 << std::endl;
     if (res != 300)
       {
         TAOX11_TEST_ERROR
            << "ERROR: Callback method <get_sendc_yadda> ami_return_val not 300: "
            << res << std::endl;
         result = 1;
       }
   }

   void get_sendc_get_yadda_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO
        << "Error, Unexpected callback method <get_sendc_yadda_excep> called."
        << std::endl;
     result = 1;
   }

   void set_sendc_get_yadda () override
   {
     callback_attrib++;
     TAOX11_TEST_INFO << "Callback method <set_sendc_yadda> called:"<< std::endl;
   }

   void set_sendc_get_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO
        << "Error, Unexpected callback method <set_sendc_yadda_excep> called."
        << std::endl;
     result = 1;
   }

   void get_sendc_ami_get_yadda (bool res) override
   {
     callback_attrib++;
     TAOX11_TEST_INFO << "Callback method <get_sendc_ami_yadda> called: res "
                 << res << std::endl;
     if (res != true)
       {
         TAOX11_TEST_ERROR << "ERROR: Callback method <get_sendc_ami_yadda> "
                      << "ami_return_val not true: " << res << std::endl;
         result = 1;
       }
   }

   void get_sendc_ami_get_yadda_excep (
        IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO
        << "Error, Unexpected callback method <get_sendc_ami_yadda_excep> called."
        << std::endl;
     result = 1;
   }

   void set_sendc_ami_get_yadda () override
   {
     callback_attrib++;
     TAOX11_TEST_INFO << "Callback method <set_sendc_ami_yadda> called:"<< std::endl;
   }

   void set_sendc_ami_get_yadda_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO
         << "Error, Unexpected callback method <set_sendc_ami_yadda_excep> called."
         << std::endl;
     result = 1;
   }

   void get_do (int32_t res) override
   {
     callback_attrib++;
     TAOX11_TEST_INFO << "Callback method <get_do> called: res " << res
                 << std::endl;
     if (res != 110)
       {
         TAOX11_TEST_ERROR
            << "ERROR: Callback method <get_do> ami_return_val not 110: "
            << res << std::endl;
         result = 1;
       }
   }

   void get_do_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO << "Error, Unexpected callback method <get_do_excep> called."
                 << std::endl;
     result = 1;
   }

   void set_do () override
   {
     callback_attrib++;
     TAOX11_TEST_INFO << "Callback method <set_do> called:"<< std::endl;
   }

   void set_do_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
   {
     TAOX11_TEST_INFO << "Error, Unexpected callback method <set_do_excep> called."
                 << std::endl;
     result = 1;
   }
};

class TwoHandler final
 : public virtual CORBA::amic_traits<A::Two>::replyhandler_base_type
{
public:
  /// Constructor.
  TwoHandler () = default;

  /// Destructor.
  ~TwoHandler () = default;

  void foo_two (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <foo_two> called: result <"
                << ami_return_val << ">."<< std::endl;
    callback_foo_two++;
    if (ami_return_val != 19)
      {
       TAOX11_TEST_ERROR << "ERROR: Callback method <foo_two> ami_return_val not 19: "
                    << ami_return_val << std::endl;
       result = 1;
      }
  }

  void foo_two_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Unexpected callback method <foo_two_excep> called."
                << std::endl;
    result = 1;
  }

  void _cxx_do (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <do> called: result <"
                << ami_return_val << ">."<< std::endl;
    callback_do++;
    if (ami_return_val != 20)
      {
       TAOX11_TEST_ERROR << "ERROR: Callback method <do> ami_return_val not 20: "
                    << ami_return_val << std::endl;
       result = 1;
      }
  }

  void do_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    TAOX11_TEST_INFO << "Error, Unexpected callback method <_cxx_do_excep> called."
                << std::endl;
    result = 1;
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


      TAOX11_TEST_INFO << "client:retrieved object reference" << std::endl;

      IDL::traits<A::Hello>::ref_type ami_test_var = IDL::traits<A::Hello>::narrow (obj);

      if (!ami_test_var)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<A::Hello>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed A::Hello interface" << std::endl;




      A::Test::_ref_type i_a = ami_test_var->get_iTest();
      if (!i_a)
        {
          TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
          return 1;
        }

      IDL::traits<A::Test>::ref_type a_sync = IDL::traits<A::Test>::narrow (i_a);

      if (!a_sync)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<A::Test>::narrow(i_a) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed A::Test interface" << std::endl;

      CORBA::amic_traits<A::Test>::ref_type a_async = CORBA::amic_traits<A::Test>::narrow (i_a);
      if (!a_async)
         {
           TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Test>::narrow (i_a) returned null object."
                        << std::endl;
           return 1;
         }

      A::Two::_ref_type i_b = ami_test_var->get_iTwo();
          if (!i_b)
          {
            TAOX11_TEST_ERROR << "ERROR: Retrieve of iB returned null object." << std::endl;
            return 1;
          }

      TAOX11_TEST_INFO << "narrowed A::Two interface" << std::endl;

      CORBA::amic_traits<A::Two>::ref_type a_async_two = CORBA::amic_traits<A::Two>::narrow (i_b);
      if (!a_async_two)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Two>::narrow (i_b) returned null object."
                       << std::endl;
          return 1;
        }



      // Instantiate the ReplyHandlers and register that with the POA.

      IDL::traits<CORBA::Object>::ref_type poa_obj =
           _orb->resolve_initial_references ("RootPOA");

      if (!obj)
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
      //Handler for interface Test
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


      CORBA::amic_traits<A::Test>::replyhandler_ref_type test_handler =
          CORBA::amic_traits<A::Test>::replyhandler_traits::narrow (test_handler_obj);

      if (test_handler == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::Test>::replyhandler_traits::narrow (test_handler_obj) returned null reference."
             << std::endl;
          return 1;
        }

      //Handler for interface Two
      CORBA::amic_traits<A::Two>::replyhandler_servant_ref_type two_handler_impl =
                CORBA::make_reference<TwoHandler> ();

      TAOX11_TEST_INFO << "created TwoHandler servant" << std::endl;

      PortableServer::ObjectId id_two = root_poa->activate_object (two_handler_impl);

      TAOX11_TEST_INFO << "activated TwoHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type two_handler_obj =
                root_poa->id_to_reference (id_two);

      if (two_handler_obj == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
            return 1;
        }


      CORBA::amic_traits<A::Two>::replyhandler_ref_type two_handler =
         CORBA::amic_traits<A::Two>::replyhandler_traits::narrow (two_handler_obj);

      if (two_handler == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: A::Two::_narrow (two_handler_obj) returned null reference."
            << std::endl;
            return 1;
        }

      poaman->activate ();


      std::stringstream ostream_rh;
      ostream_rh << test_handler;
      std::string ostream_rh_str = ostream_rh.str();
      if ((ostream_rh_str.find ("IDL:A/AMI_AMI_AMI_TestHandler:1.0") == std::string::npos)||
          (ostream_rh_str.find ("<A::AMI_AMI_AMI_TestHandler>") == std::string::npos))
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Test>::ref_type returned wrong ostream info: "
                       << ostream_rh_str << std::endl;
          result = 1;
        }

      TAOX11_TEST_INFO << "ostream test CORBA::amic_traits<A::Test>::replyhandler_ref_type: <" << test_handler
                     << std::endl;

      ostream_rh.str("");
      ostream_rh << two_handler;
      ostream_rh_str = ostream_rh.str();
      if ((ostream_rh_str.find ("IDL:A/AMI_TwoHandler:1.0") == std::string::npos)||
          (ostream_rh_str.find ("<A::AMI_TwoHandler>") == std::string::npos))
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Two>::ref_type returned wrong ostream info: "
                       << ostream_rh_str << std::endl;
          result = 1;
        }

      TAOX11_TEST_INFO << "ostream test CORBA::amic_traits<A::Test>::replyhandler_ref_type: <" << two_handler
                  << std::endl;

      TAOX11_TEST_INFO << "Client: Sending synch messages foo, sendc_foo and sendc_ami_foo." << std::endl;
      int32_t res = a_sync->foo(22);
      if (res != 3)
        {
          TAOX11_TEST_ERROR
              << "ERROR: a_sync->foo didn't return ,3> but <" << res << ">."
              << std::endl;
          result =1;
        }
      res = a_sync->sendc_foo(122);
      if (res != 4)
        {
          TAOX11_TEST_ERROR
               << "ERROR: a_sync->sendc_foo didn't return ,4> but <" << res << ">."
               << std::endl;
          result =1;
        }
      res = a_sync->sendc_ami_foo(false);
      if (res != 5)
        {
          TAOX11_TEST_ERROR
                << "ERROR: a_sync->sendc_ami_foo didn't return ,5> but <" << res << ">."
                << std::endl;
          result =1;

        }

      TAOX11_TEST_INFO << "Client: Sending via asynch interface, synch messages foo, sendc_foo and sendc_ami_foo." << std::endl;
      res = a_async->foo(12);
      if (res != 3)
        {
          TAOX11_TEST_ERROR
              << "ERROR: a_async->foo didn't return ,3> but <" << res << ">."
              << std::endl;
          result =1;
        }
      res = a_sync->sendc_foo(222);
      if (res != 4)
        {
          TAOX11_TEST_ERROR
               << "ERROR: a_async->sendc_foo didn't return ,4> but <" << res << ">."
               << std::endl;
          result =1;
        }
      res = a_sync->sendc_ami_foo(false);
      if (res != 5)
        {
          TAOX11_TEST_ERROR
                << "ERROR: a_async->sendc_ami_foo didn't return ,5> but <" << res << ">."
                << std::endl;
          result =1;
        }

      TAOX11_TEST_INFO << "Client: Sending asynch message for foo, sendc_foo and sendc_ami_foo." << std::endl;
      a_async->sendc_ami_ami_foo (test_handler,12);
      a_async->sendc_sendc_foo (test_handler,11);
      a_async->sendc_sendc_ami_foo (test_handler,false);

      TAOX11_TEST_INFO << "Client: Sending asynch message for foo_two." << std::endl;
      a_async_two->sendc_foo_two (two_handler,9);

      TAOX11_TEST_INFO << "Client: Sending asynch message for do." << std::endl;
      a_async_two->sendc_do (two_handler,10);

      TAOX11_TEST_INFO << "Client: Sending asynch message for yadda, sendc_get_yadda,sendc_ami_get_yadda." << std::endl;
      //yadda
      a_async->sendc_set_yadda (test_handler, 100);
      a_async->sendc_ami_ami_get_yadda (test_handler);
      //sendc_get_yadda
      a_async->sendc_set_sendc_get_yadda (test_handler, 200);
      a_async->sendc_get_sendc_get_yadda (test_handler);

      //sendc_ami_get_yadda
      a_async->sendc_set_sendc_ami_get_yadda (test_handler, false);
      a_async->sendc_get_sendc_ami_get_yadda (test_handler);

      TAOX11_TEST_INFO << "Client: Sending asynch message for attribute do." << std::endl;
      //do
      a_async->sendc_set_do (test_handler, 100);
      a_async->sendc_get_do (test_handler);

      TAOX11_TEST_INFO << "Client: Sending the last  asynch message foo." << std::endl;
      a_async->sendc_ami_ami_foo (test_handler, 10);

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

      if ((callback_foo != 2) || (callback_sendc_foo != 1) ||
          (callback_sendc_ami_foo != 1) || (callback_attrib != 8 ) ||
          (callback_foo_two != 1) || (callback_do != 1))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks. Foo : expected -2- , received -" << callback_foo << "-"
          << " sendc_foo : expected -1-, received -" << callback_sendc_foo << "-."
          << " sendc_ami_foo : expected -1-, received -" << callback_sendc_ami_foo << "-."
          << " foo_two : expected -1-, received -" << callback_foo_two << "-."
          << " do : expected -1-, received -" << callback_do << "-."
          << " Attrib : expected -6-, received -" << callback_attrib << "-."
          << std::endl;
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
