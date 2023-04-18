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
int16_t callback_attrib = 0;
int16_t callback_operation = 0;
int16_t callback_attrib_der = 0;
int16_t callback_operation_der = 0;
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
: public virtual CORBA::amic_traits<MyFoo>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void foo (int32_t ami_return_val, int32_t out_l) override
  {
    TAOX11_TEST_INFO << "Callback method <Handler::foo> called: result <"
                << ami_return_val
                << ">, out_arg <" << out_l << ">"<< std::endl;
    callback_operation++;
    // Last reply flips the flag.
    if (out_l == 931235)
    {
      received_all_replies = 1;
    }
    else if (out_l != 931233)
    {
      TAOX11_TEST_ERROR <<"ERROR: method <Handler::foo> out_l not 931233: "
                   << out_l
                   << std::endl;
      result = 1;
    }

    if (ami_return_val != 931234)
    {
      TAOX11_TEST_ERROR << "ERROR: method <Handler::foo> ami_return_val not 931234: "
                   << ami_return_val << std::endl;
      result = 1;
    }
  }

  void foo_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    TAOX11_TEST_INFO
       << "Callback method <foo_excep> called:"
       << " Testing proper exception handling."<< std::endl;
    callback_excep++;
    try
    {
      excep_holder->raise_exception ();
    }
    catch (const DidTheRightThing& ex)
    {
      TAOX11_TEST_INFO
         << "Callback method <Handler::foo_excep> received successfully <"
         << ex.id() << "> and <" << ex.whatDidTheRightThing() << ">."
         << std::endl;
      if (ex.id() != 42)
      {
        TAOX11_TEST_ERROR << "ERROR: method <Handler::foo_excep> ex.id not 42 but "
                     << ex.id() << std::endl;
        result = 1;
      }
      if (ex.whatDidTheRightThing () != "Hello world")
      {
        TAOX11_TEST_ERROR
          << "ERROR: method <Handler::foo_excep> ex.whatDidTheRightThing not ok: "
          << ex.whatDidTheRightThing() << std::endl;
        result = 1;
      }
    }
    catch (const CORBA::Exception&)
    {
      TAOX11_TEST_ERROR << "Callback method <Handler::foo_excep> "
                   << "caught the wrong exception -> ERROR" << std::endl;
      result = 1;
    }
  }

  void get_yadda (int32_t ret) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <Handler::get_yadda> called: ret " << ret
                << std::endl;
  };

  void get_yadda_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    callback_excep++;
    TAOX11_TEST_INFO << "Callback method <Handler::get_yadda_excep> called:"
                << std::endl;
    try
    {
      excep_holder->raise_exception ();
    }
    catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_INFO << "... caught expected exception -> " << ex << std::endl;
      std::stringstream excep_c;
      excep_c << ex;
      std::string excep_cstr = excep_c.str();
      if (excep_cstr.find ("NO_IMPLEMENT") == std::string::npos)
      {
        TAOX11_TEST_ERROR << "ERROR: Callback method <Handler::get_yadda_excep>"
                     << " returned wrong CORBA::exception! -"
                     << excep_cstr << std::endl;
        result = 1;
      }
    }
  }

  void set_yadda () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <Handler::set_yadda> called:"<< std::endl;
  }

  void set_yadda_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    TAOX11_TEST_INFO << "Callback method <Handler::set_yadda_excep> called:"
                << std::endl;
    callback_excep++;
    try
    {
      excep_holder->raise_exception ();
    }
    catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_INFO << "... caught expected exception -> " << ex
                  << std::endl;
      std::stringstream excep_s;
      excep_s << ex;
      std::string excep_str = excep_s.str();
      if (excep_str.find ("NO_IMPLEMENT") == std::string::npos)
      {
        TAOX11_TEST_ERROR << "ERROR: Callback method <Handler::set_yadda_excep> "
                     << "returned wrong std::exception info! -"
                     << excep_str << std::endl;
        result = 1;
      }
    }
  }

  void get_bool_attr (bool result) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <Handler::get_bool_attr> called:" << result
                << std::endl;
  };

  void get_bool_attr_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    TAOX11_TEST_INFO << "Callback method <Handler::get_bool_attr_excep> called:"
                << std::endl;
    callback_excep++;
    try
    {
      excep_holder->raise_exception ();
    }
    catch (const DidTheRightThing& ex)
    {
      TAOX11_TEST_INFO << "Callback method <Handler::get_bool_attr_excep> exception"
                  << "received successfully <"
                  << ex.id() << "> and <" << ex.whatDidTheRightThing()
                  << ">."<< std::endl;
      if (ex.id() != 42)
      {
        TAOX11_TEST_ERROR << "ERROR: ex.id not 42 but "<< ex.id()
                     <<  std::endl;
        result = 1;
      }
      if (ex.whatDidTheRightThing () != "Hello world")
      {
        TAOX11_TEST_ERROR << "ERROR: method <Handler::get_bool_attr_excep> "
                     << "ex.whatDidTheRightThing not ok: "
                     << ex.whatDidTheRightThing() << std::endl;
        result = 1;
      }
    }
    catch (const CORBA::Exception&)
    {
      TAOX11_TEST_ERROR << "Callback method <Handler::get_bool_attr_excep> "
                   << "caught the wrong exception -> ERROR" << std::endl;
    }
  }

  void set_bool_attr () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <Handler::set_bool_attr> called:"
                << std::endl;
  }

  void set_bool_attr_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    callback_excep++;
    TAOX11_TEST_INFO << "Callback method <Handler::set_bool_attr_excep> called:"
                << std::endl;
  }

  void foo_struct (
      const std::string& ami_return_val, const structType& t) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <fHandler::oo_struct> called: t.as = "
                << t.as() << " return = <" << ami_return_val
                << ">" << std::endl;
    if ((t.as() != 5) || (ami_return_val != "bye from foo_struct"))
    {
      TAOX11_TEST_ERROR << "ERROR: ami_return_val not 'bye from foo_struct' but '"
                   << ami_return_val
                   << "' or t.as not <5> but <" << t.as() << ">" << std::endl;
      result = 1;
    }
  }

  void foo_struct_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
  }
};

class DerivedHandler
: public virtual CORBA::amic_traits<A::MyDerived>::replyhandler_base_type
{
public:
  /// Constructor.
  DerivedHandler () = default;

  /// Destructor.
  ~DerivedHandler () = default;

  void do_something (int32_t ami_return_val) override
  {
    callback_operation_der++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::do_something> called."
               << std::endl;
    if (ami_return_val != 4)
    {
      TAOX11_TEST_ERROR << "ERROR:  method <DerivedHandler::do_something> ami_return_val not 4: "
                   << ami_return_val << std::endl;
      result = 1;
    }
  }

  void do_something_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::do_someting_excep> called."
               << std::endl;
  }

  void get_my_derived_attrib (int32_t ret) override
  {
    callback_attrib_der++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::get_my_derived_attrib> called: ret "
                << ret << std::endl;
    if (ret != 5)
    {
      TAOX11_TEST_ERROR << "ERROR:  method <DerivedHandler::get_my_derived_attrib ret not 5: "
                   << ret << std::endl;
      result = 1;
    }

  }

  void get_my_derived_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::get_my_derived_attrib_excep> called."
                << std::endl;
  }

  void set_my_derived_attrib () override
  {
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::set_my_derived_attrib> called."
                << std::endl;
  }

  void set_my_derived_attrib_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::set_my_derived_attrib_excep> called:"
                << std::endl;
  }

  void foo (int32_t ami_return_val,int32_t out_l) override
  {
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::foo> called: result <" << ami_return_val
                << ">, out_arg <" << out_l << ">"<< std::endl;
  }

  void foo_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO
       << "Callback method <DerivedHandler::foo_excep> called:"
       << " Testing proper exception handling."<< std::endl;
  }

  void get_yadda (int32_t ret) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::get_yadda> called: ret " << ret
                << std::endl;
  };

  void get_yadda_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    callback_excep++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::get_yadda_excep> called:"<< std::endl;
  }

  void set_yadda () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::set_yadda> called:"<< std::endl;
  }

  void set_yadda_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type  /*excep_holder*/) override
  {
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::set_yadda_excep> called:" << std::endl;
    callback_excep++;
  }

  void get_bool_attr (bool result) override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::get_bool_attr> called:" << result
                << std::endl;
  };

  void get_bool_attr_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
   TAOX11_TEST_INFO << "Callback method <DerivedHandler::get_bool_attr_excep> called:" << std::endl;
   callback_excep++;
  }

  void set_bool_attr () override
  {
    callback_attrib++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::set_bool_attr> called:" << std::endl;
  }

  void set_bool_attr_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
    callback_excep++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::set_bool_attr_excep> called:"
                << std::endl;
  }

  void foo_struct (
      const std::string& ami_return_val, const structType& t) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <DerivedHandler::foo_struct> called: t.as = " << t.as()
                << " return = <" << ami_return_val << ">" << std::endl;
  }

  void foo_struct_excep (
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

    IDL::traits<Hello>::ref_type ami_test_var = IDL::traits<Hello>::narrow (obj);

    if (!ami_test_var)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Hello>::narrow (obj) returned null object."
                   << std::endl;
      return 1;
    }
    TAOX11_TEST_INFO << "narrowed Hello interface" << std::endl;

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
           << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj)"
           <<" returned null object." << std::endl;
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

    CORBA::amic_traits<MyFoo>::replyhandler_servant_ref_type test_handler_impl =
         CORBA::make_reference<Handler> ();

    TAOX11_TEST_INFO << "created MyFoo replyhandler servant" << std::endl;

    PortableServer::ObjectId id = root_poa->activate_object (test_handler_impl);

    TAOX11_TEST_INFO << "activated MyFoo replyhandler servant" << std::endl;

    IDL::traits<CORBA::Object>::ref_type test_handler_obj =
         root_poa->id_to_reference (id);

    if (test_handler_obj == nullptr)
    {
      TAOX11_TEST_ERROR
           << "ERROR: root_poa->id_to_reference (id) returned null reference."
           << std::endl;
      return 1;
    }

    CORBA::amic_traits<MyFoo>::replyhandler_ref_type test_handler =
        CORBA::amic_traits<MyFoo>::replyhandler_traits::narrow (test_handler_obj);

    if (test_handler == nullptr)
    {
      TAOX11_TEST_ERROR
        << "ERROR: CORBA::amic_traits<MyFoo>::replyhandler_traits::narrow"
        << " (test_handler_obj) returned null reference."
        << std::endl;
      return 1;
    }

    CORBA::amic_traits<A::MyDerived>::replyhandler_servant_ref_type test_handler_der_impl =
             CORBA::make_reference<DerivedHandler> ();

    TAOX11_TEST_INFO << "created MyDerived replyhandler servant" << std::endl;

    PortableServer::ObjectId id_der = root_poa->activate_object (test_handler_der_impl);

    TAOX11_TEST_INFO << "activated MyDerived replyhandler servant" << std::endl;

    IDL::traits<CORBA::Object>::ref_type test_handler_der_obj =
         root_poa->id_to_reference (id_der);

    if (test_handler_der_obj == nullptr)
    {
      TAOX11_TEST_ERROR
           << "ERROR: root_poa->id_to_reference (id_der) returned null reference."
           << std::endl;
      return 1;
    }

    CORBA::amic_traits<A::MyDerived>::replyhandler_ref_type test_handler_der =
        CORBA::amic_traits<A::MyDerived>::replyhandler_traits::narrow (test_handler_der_obj);

    if (test_handler_der == nullptr)
    {
      TAOX11_TEST_ERROR
        << "ERROR: CORBA::amic_traits<A::MyDerived>::replyhandler_traits::narrow"
        << " (test_handler_obj) returned null reference."
        << std::endl;
      return 1;
    }

    poaman->activate ();

    A::MyDerived::_ref_type i_der_ref = ami_test_var->get_iMyDerived();
    if (!i_der_ref)
    {
      TAOX11_TEST_ERROR << "ERROR: Retrieve of iMyDerived returned null object."
                   << std::endl;
      return 1;
    }
    CORBA::amic_traits<A::MyDerived>::ref_type i_der =
        CORBA::amic_traits<A::MyDerived>::narrow (i_der_ref);
    if (!i_der)
     {
       TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::MyDerived>::narrow (i_der) "
                    << "returned null object."
                    << std::endl;
       return 1;
     }


    MyFoo::_ref_type i_foo_ref = ami_test_var->get_iMyFoo();
    if (!i_foo_ref)
    {
      TAOX11_TEST_ERROR << "ERROR: Retrieve of iMyFoo returned null object." << std::endl;
      return 1;
    }
    CORBA::amic_traits<MyFoo>::ref_type i_foo =
        CORBA::amic_traits<MyFoo>::narrow (i_foo_ref);
    if (!i_foo)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<MyFoo>::narrow (i_foo) "
                   << " returned null object."
                   << std::endl;
      return 1;
    }

    // Trigger the DidTheRightThing exception on the server side
    // by sending 0 to it.
    TAOX11_TEST_INFO << "Client: Sending asynch message sendc_foo to trigger an exception"
                << std::endl;

    i_foo->sendc_foo (test_handler,
                      0,
                      "Let's talk AMI.");

    int32_t l = 931247;

    TAOX11_TEST_INFO << "Client: Sending asynch message foo." << std::endl;

    i_foo->sendc_foo (test_handler,
                      l,
                      "Let's talk AMI.");

    i_der->sendc_do_something(test_handler_der, "Hello");

    // Begin test of attributes
    TAOX11_TEST_INFO << "Client: Sending asynch message sendc_get_yadda to trigger "
                << "an exception" << std::endl;
    i_foo->sendc_get_yadda (test_handler);

    TAOX11_TEST_INFO << "Client: Sending asynch attributes yadda." << std::endl;
    i_foo->sendc_set_yadda (test_handler, 4711);
    i_foo->sendc_get_yadda (test_handler);

    TAOX11_TEST_INFO << "Client: Sending asynch message sendc_set_yadda to trigger "
                << "an exception" << std::endl;
    i_foo->sendc_set_yadda (test_handler, 0);

    TAOX11_TEST_INFO << "Client: Sending asynch message sendc_get_bool_attr to trigger "
                << "an exception" << std::endl;
    i_foo->sendc_get_bool_attr (test_handler);

    TAOX11_TEST_INFO << "Client: Sending asynch attributes bool_attr." << std::endl;
    i_foo->sendc_set_bool_attr (test_handler, false);
    i_foo->sendc_get_bool_attr (test_handler);

    TAOX11_TEST_INFO << "Client: Sending asynch message sendc_set_yadda to trigger "
                << "an exception" << std::endl;
    i_foo->sendc_set_bool_attr (test_handler, true);

    i_der->sendc_get_my_derived_attrib (test_handler_der);

    // End test of attributes

    i_foo->sendc_foo_struct (test_handler);

    TAOX11_TEST_INFO << "Client: Sending another asynch message foo." << std::endl;

    i_foo->sendc_foo (test_handler,
                      l,
                      "Let's talk AMI.");

    TAOX11_TEST_INFO << "Client: Sending the last  asynch message foo." << std::endl;

    i_foo->sendc_foo (test_handler,
                      931235,
                      "Let's talk AMI the last time.");

    TAOX11_TEST_INFO << "Client: Do something else before coming back for the replies."
                << std::endl;

    for (int i = 0; i < 10; i ++)
    {
      TAOX11_TEST_INFO << " ...";
      std::this_thread::sleep_for (std::chrono::milliseconds (10));
    }

    TAOX11_TEST_INFO << std::endl << "Client: Now let's look for the replies." << std::endl;

    std::this_thread::sleep_for (std::chrono::seconds (2));
    while (!received_all_replies)
    {
      std::chrono::seconds tv (1);
      _orb->perform_work (tv);
    }

    if ((callback_operation != 4) ||
         (callback_excep != 5 ) ||
         (callback_attrib != 4 ) ||
         (callback_attrib_der != 1 ) ||
         (callback_operation_der != 1 ))
    {
      TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks."
      << " Foo : expected -4- , received -" << callback_operation << "-"
      << " do_something : expected -1- , received -" << callback_operation_der << "-"
      << " Attrib : expected -4-, received -" << callback_attrib << "-"
      << " Attrib derived : expected -1-, received -" << callback_attrib_der << "-"
      << " Exceptions: expected -5-, received -" << callback_excep << "-."
      << std::endl;
      result = 1;
    }

    ami_test_var->shutdown ();

    _orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    return 1;
  }
  return result;
}
