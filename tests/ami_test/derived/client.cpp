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
int16_t nr_of_replies = 0;
int16_t myfoo_foo = 0;
int16_t mybar_foo = 0;
int16_t myfoo_attrib = 0;
int16_t mybar_attrib = 0;
int16_t myfoo_foo_excep = 0;
int16_t mybar_foo_excep = 0;

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

class MyFooHandler
: public virtual CORBA::amic_traits<A::MyFoo>::replyhandler_base_type
{
public:
  /// Constructor.
  MyFooHandler () = default;

  /// Destructor.
  ~MyFooHandler () = default;

  void foo (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <MyFooHandler::foo> called: ret "
                << ami_return_val << std::endl;
    nr_of_replies--;
    myfoo_foo++;
  }

  void foo_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const A::InternalError& ex)
      {
        myfoo_foo_excep++;
        TAOX11_TEST_INFO << "Callback method <MyFoo::foo_excep> exception received successfully <"
                   << ex.id() << "> and <" << ex.error_string() << ">." << std::endl;
        if (ex.id() != 2)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <MyFoo::foo_excep> exception ex.id not 2 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
        if (ex.error_string() != "Hello from foo")
          {
            TAOX11_TEST_ERROR
            << "ERROR: Callback method <MyFoo::foo_excep> exception ex.error_string() not ok: "
                         << ex.error_string() << std::endl;
            result = 1;
          }
      }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <MyFoo::foo_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;
  }

  void get_my_foo_attrib (int32_t ret) override
   {
     TAOX11_TEST_INFO << "Callback method <MyFooHandler::get_my_foo_attrib> called: ret " << ret << std::endl;
     if (ret != 3)
       {
         TAOX11_TEST_ERROR
            << "ERROR: Callback method <MyFooHandler::get_my_foo_attrib> ret not 3 but "
            << ret <<  std::endl;
         result = 1;
       }
     nr_of_replies--;
     myfoo_attrib++;
   }

   void get_my_foo_attrib_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
   {
     TAOX11_TEST_INFO << "Callback method <get_my_foo_attrib_excep> called." << std::endl;
   }

   void set_my_foo_attrib () override
   {
     TAOX11_TEST_INFO << "Callback method <MyFooHandler::set_my_foo_attrib> called:"<< std::endl;
     nr_of_replies--;
     myfoo_attrib++;
   }

   void set_my_foo_attrib_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type  /*excep_holder*/) override
   {
     TAOX11_TEST_INFO << "Callback method <set_my_foo_attrib_excep> called:" << std::endl;
   }
};


class MyBarHandler
  : public virtual CORBA::amic_traits<A::MyBar>::replyhandler_base_type
{
public:
  /// Constructor.
  MyBarHandler () = default;

  /// Destructor.
  ~MyBarHandler () = default;

  void bye (int32_t ami_return_val, int32_t /*answer*/) override
  {
    nr_of_replies--;
    TAOX11_TEST_INFO << "Callback method <MyBarHandler::bye> called: ret "
                << ami_return_val << std::endl;
  }

  void bye_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <bye_excep> called."<< std::endl;
  }

  void foo (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <MyBarHandler::foo> called: ret "
                << ami_return_val << std::endl;
    nr_of_replies--;
    mybar_foo++;
  }

  void foo_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const A::InternalError& ex)
      {
        mybar_foo_excep++;
        TAOX11_TEST_INFO << "Callback method <MyBar::foo_excep> exception received successfully <"
                   << ex.id() << "> and <" << ex.error_string() << ">." << std::endl;
        if (ex.id() != 3)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <MyBar::foo_excep> exception ex.id not 3 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
        if (ex.error_string() != "Hello from bar")
          {
            TAOX11_TEST_ERROR
               << "ERROR: Callback method <MyBar::foo_excep> exception ex.error_string() not ok: "
               << ex.error_string() << std::endl;
            result = 1;
          }
      }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
         << "ERROR: Callback method <MyFoo::foo_excep> caught the wrong exception"
         << std::endl;
        result = 1;
      }
    nr_of_replies--;
  }

  void get_my_foo_attrib (int32_t ret) override
  {
     TAOX11_TEST_INFO << "Callback method <MyBarHandler::get_my_foo_attrib> called: ret "
                 << ret << std::endl;
     if (ret != 11)
       {
         TAOX11_TEST_ERROR
            << "ERROR: Callback method <MyBarHandler::get_my_foo_attrib> ret not 11 but "
            << ret <<  std::endl;
         result = 1;
       }
     nr_of_replies--;
     mybar_attrib++;
  }

  void get_my_foo_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <get_my_foo_attrib_excep> called." << std::endl;
  }

  void set_my_foo_attrib () override
  {
    TAOX11_TEST_INFO << "Callback method <MyBarHandler::set_my_foo_attrib> called:"<< std::endl;
    nr_of_replies--;
    mybar_attrib++;
  }

  void set_my_foo_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type  /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <set_my_foo_attrib_excep> called:" << std::endl;
  }

  void do_something (int32_t /*ami_return_val*/) override
  {
  }

  void do_something_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <do_someting_excep> called."<< std::endl;
  }

  void get_my_derived_attrib (int32_t ret) override
  {
    TAOX11_TEST_INFO << "Callback method <get_my_derived_attrib> called: ret " << ret << std::endl;
  }

  void get_my_derived_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <get_my_derived_attrib_excep> called." << std::endl;
  }

  void set_my_derived_attrib () override
  {
    TAOX11_TEST_INFO << "Callback method <set_my_derived_attrib> called." << std::endl;
  }

  void set_my_derived_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <set_my_derived_attrib_excep> called:" << std::endl;
  }

  void get_my_bar_attrib (int32_t ret) override
  {
    nr_of_replies--;
    TAOX11_TEST_INFO << "Callback method <get_my_bar_attrib> called: ret " << ret << std::endl;
    if (ret != 9)
      {
        TAOX11_TEST_ERROR
           << "ERROR: Callback method <MyBarHandler::get_my_foo_attrib> ret not 9 but "
           << ret <<  std::endl;
        result = 1;
      }
  }

  void get_my_bar_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <get_my_bar_attrib_excep> called." << std::endl;
  }

  void set_my_bar_attrib () override
  {
    nr_of_replies--;
    TAOX11_TEST_INFO << "Callback method <set_my_bar_attrib> called:"<< std::endl;
  }

  void set_my_bar_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
     TAOX11_TEST_INFO << "Callback method <set_my_bar_attrib_excep> called:" << std::endl;
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

      TAOX11_TEST_INFO << "narrowed ::A::Hello interface" << std::endl;


      // Instantiate the ReplyHandlers and register that with the POA.

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

      //Handler for interface MyBar
      CORBA::amic_traits<A::MyBar>::replyhandler_servant_ref_type test_handler_impl_bar =
           CORBA::make_reference<MyBarHandler> ();

      TAOX11_TEST_INFO << "created MyBarHandler servant" << std::endl;

      PortableServer::ObjectId id_bar = root_poa->activate_object (test_handler_impl_bar);

      TAOX11_TEST_INFO << "activated MyBarHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_bar_obj =
           root_poa->id_to_reference (id_bar);

      if (test_handler_bar_obj == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: root_poa->id_to_reference (id) returned null reference."
             << std::endl;
           return 1;
        }

      CORBA::amic_traits<A::MyBar>::replyhandler_ref_type test_handler_bar =
          CORBA::amic_traits<A::MyBar>::replyhandler_traits::narrow (test_handler_bar_obj);

      if (test_handler_bar == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::MyBar>::replyhandler_traits::narrow (test_handler_bar_obj) returned null reference."
             << std::endl;
          return 1;
        }

      //Handler for interface MyFoo
      CORBA::amic_traits<A::MyFoo>::replyhandler_servant_ref_type test_handler_impl_foo =
             CORBA::make_reference<MyFooHandler> ();

      TAOX11_TEST_INFO << "created MyFooHandler servant" << std::endl;

      PortableServer::ObjectId id_foo = root_poa->activate_object (test_handler_impl_foo);

      TAOX11_TEST_INFO << "activated MyFooHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_foo_obj =
             root_poa->id_to_reference (id_foo);

      if (test_handler_foo_obj == nullptr)
        {
          TAOX11_TEST_ERROR
               << "ERROR: root_poa->id_to_reference (id) returned null reference."
               << std::endl;
          return 1;
        }

      CORBA::amic_traits<A::MyFoo>::replyhandler_ref_type test_handler_foo =
          CORBA::amic_traits<A::MyFoo>::replyhandler_traits::narrow (test_handler_foo_obj);

      if (test_handler_foo == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::MyFoo>::replyhandler_traits::narrow (test_handler_foo_obj) returned null reference."
             << std::endl;
          return 1;
        }


      poaman->activate ();

      A::MyBar::_ref_type i_bar = ami_test_var->get_iMyBar();
      if (!i_bar)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iMyBar returned null object." << std::endl;
        return 1;
      }
      CORBA::amic_traits<A::MyBar>::ref_type i_bar_async = CORBA::amic_traits<A::MyBar>::narrow (i_bar);
      if (!i_bar_async)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traitsA::MyBar>::narrow (i_bar) returned null object."
                       << std::endl;
          return 1;
        }
      A::MyFoo::_ref_type i_foo = ami_test_var->get_iMyFoo();
      if (!i_foo)
        {
          TAOX11_TEST_ERROR << "ERROR: Retrieve of iMyFoo returned null object." << std::endl;
          return 1;
        }
        CORBA::amic_traits<A::MyFoo>::ref_type i_foo_async = CORBA::amic_traits<A::MyFoo>::narrow (i_foo);
       if (!i_foo_async)
         {
           TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traitsA::MyFoo>::narrow (i_foo) returned null object."
                        << std::endl;
           return 1;
         }
      TAOX11_TEST_INFO << "Client: Sending 3 asynch messages MyBar::bye, MyBar::foo and MyFoo::foo." << std::endl;
      i_bar_async->sendc_bye (test_handler_bar);

      i_bar_async->sendc_foo (test_handler_bar, 12);
      i_foo_async->sendc_foo (test_handler_foo, 11);

      TAOX11_TEST_INFO << "Client: Sending 2 asynch messages for MyBar::my_bar_attrib attributes." << std::endl;
      i_bar_async->sendc_set_my_bar_attrib (test_handler_bar, 101);
      i_bar_async->sendc_get_my_bar_attrib (test_handler_bar);

      TAOX11_TEST_INFO << "Client: Sending 2 asynch messages for MyBar::my_foo_attrib attributes." << std::endl;
      i_bar_async->sendc_set_my_foo_attrib (test_handler_bar, 101);
      i_bar_async->sendc_get_my_foo_attrib (test_handler_bar);

      TAOX11_TEST_INFO << "Client: Sending 2 asynch messages for MyFoo::my_foo_attrib attributes." << std::endl;
      i_foo_async->sendc_set_my_foo_attrib (test_handler_foo, 101);
      i_foo_async->sendc_get_my_foo_attrib (test_handler_foo);

      TAOX11_TEST_INFO << "Client: Sending asynch messages MyBar::foo (2) and MyFoo::foo (1) to trigger exceptions." << std::endl;
      i_bar_async->sendc_foo (test_handler_bar, 0);
      i_foo_async->sendc_foo (test_handler_foo, 0);
      i_bar_async->sendc_foo (test_handler_bar, 0);


      nr_of_replies = 12;

      TAOX11_TEST_INFO << "Client: Do something else before coming back for the replies." << std::endl;

      for (int i = 0; i < 10; i ++)
        {
          TAOX11_TEST_INFO << " ...";
          std::this_thread::sleep_for (std::chrono::milliseconds (10));
        }

      TAOX11_TEST_INFO << std::endl << "Client: Now let's look for the replies." << std::endl;

      std::this_thread::sleep_for (std::chrono::seconds (2));
      while ((nr_of_replies > 0))
        {
          bool pending = _orb->work_pending();
          if (pending)
            {
              _orb->perform_work ();
            }
        }

      if ((nr_of_replies != 0) || (myfoo_foo != 1) || (mybar_foo != 1))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected replies. Expected -12- , received -"
          << (5 - nr_of_replies) << "-. " << " Expected MyFoo- foo <1> , received <"
          << myfoo_foo << ">. Expected MyBar- foo <1>, received <" << mybar_foo << ">." << std::endl;
          result = 1;
        }

      if ((myfoo_attrib != 2) || (mybar_attrib != 2))
         {
           TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected replies on my_foo_attrib. "
           << "Expected MyFoo- my_foo_attrib <2> , received <"
           << myfoo_attrib << ">. Expected MyBar- my_foo_attrib <2>, received <"
           << mybar_attrib << ">." << std::endl;
           result = 1;
         }

      if ((myfoo_foo_excep != 1) || (mybar_foo_excep != 2))
          {
            TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected exceptions on my_foo. "
            << "Expected MyFoo- my_foo_excep <1> , received <"
            << myfoo_foo_excep << ">. Expected MyBar- my_foo_excep <2>, received <"
            << mybar_foo_excep << ">." << std::endl;
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
