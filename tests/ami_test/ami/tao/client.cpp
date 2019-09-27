/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// FUZZ: disable check_for_ace_log_categories

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ami_testS.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int result = 0;
int callback_attrib = 0;
int callback_operation = 0;
int callback_excep = 0;

// Flag indicates that all replies have been received
static bool received_all_replies = false;

int
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                            "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Handler
  : public POA_A::AMI_AMI_TestHandler
{
public:
  /// Constructor.
  Handler () {};

  /// Destructor.
  ~Handler () {};

  virtual void foo (
    ::CORBA::Long ami_return_val,
    ::CORBA::Long out_l);

  virtual void foo_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void get_yadda (
    ::CORBA::Long ami_return_val);

  virtual void get_yadda_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void set_yadda ();

  virtual void set_yadda_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void bar (
    ::A::enumType out_e);

  virtual void bar_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void foo_struct (
    const char * ami_return_val,
    const ::A::structType & out_t);

  virtual void foo_struct_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void get_bool_attr (
    ::CORBA::Boolean ami_return_val);

  virtual void get_bool_attr_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void set_bool_attr ();

  virtual void set_bool_attr_excep (
    ::Messaging::ExceptionHolder * excep_holder);
};

void Handler::foo (CORBA::Long ami_return_val,
  CORBA::Long out_l)
{
  ACE_DEBUG ((LM_DEBUG, "Callback method <foo> called: result <%d>, out_arg <%d>\n",
              ami_return_val, out_l));
  callback_operation++;
  // Last reply flips the flag.
  if (out_l == 931235)
    {
      received_all_replies = true;
    }
  else if (out_l != 931233)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR:  method <foo> out_l not 931233: %d\n", out_l));
      result = 1;
    }

  if (ami_return_val != 931234)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR:  method <foo> ami_return_val not 931233: %d\n",
                  ami_return_val));
      result = 1;
    }
}

void
Handler::foo_excep (::Messaging::ExceptionHolder * excep_holder)
{
  ACE_DEBUG ((LM_DEBUG,
              "Callback method <foo_excep> called:\n"
              "Testing proper exception handling ...\n"));
  callback_excep++;
  try
    {
      excep_holder->raise_exception ();
    }
  catch (const A::DidTheRightThing& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "... exception received successfully\n"));
      if (ex.id != 42)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: ex.id not 42: %d\n",
                      ex.id));
          result = 1;
        }
      if (ACE_OS::strcmp (ex.whatDidTheRightThing.in (), "Hello world") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: ex.whatDidTheRightThing not ok: <%C>\n",
                      ex.whatDidTheRightThing.in ()));
          result = 1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR");
      ACE_ERROR ((LM_ERROR,
                  "... caught the wrong exception -> ERROR\n"));
    }
}

void
Handler::bar (A::enumType)
{
}


void
Handler::bar_excep (::Messaging::ExceptionHolder * )
{
}

void
Handler::get_yadda (CORBA::Long result)
{
  callback_attrib++;
  ACE_DEBUG ((LM_DEBUG, "Callback method <get_yadda> called: ret %d\n", result));
}

void
Handler::get_yadda_excep (::Messaging::ExceptionHolder *excep_holder)
{
  callback_excep++;
  ACE_DEBUG ((LM_DEBUG, "Callback method <get_yadda_excep> called: \n"));
  try
    {
      excep_holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "... exception received successfully\n"));
      if (ex._info ().find ("NO_IMPLEMENT") == std::string::npos )
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Callback method <get_yadda_excep>"
                    " returned wrong CORBA::exception! - %C\n",
                    ex._info ().c_str ()));
          result = 1;
        }
    }
}

void
Handler::set_yadda ()
{
  callback_attrib++;
  ACE_DEBUG ((LM_DEBUG, "Callback method <set_yadda> called\n"));
}

void
Handler::set_yadda_excep (::Messaging::ExceptionHolder *excep_holder)
{
  ACE_DEBUG ((LM_ERROR,
              "Callback method <set_yadda_excep> called:\n"));
  callback_excep++;
  try
    {
      excep_holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "... exception received successfully\n"));
      if (ex._info ().find ("NO_IMPLEMENT") == std::string::npos )
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Callback method <set_yadda_excep>"
                    " returned wrong CORBA::exception! - %C\n",
                    ex._info ().c_str ()));
          result = 1;
        }
    }
}

void
Handler::get_bool_attr (CORBA::Boolean result)
{
  callback_attrib++;
  ACE_DEBUG ((LM_DEBUG, "Callback method <get_bool_attr> called: %d\n", result));
}

void
Handler::get_bool_attr_excep (::Messaging::ExceptionHolder *excep_holder)
{
  ACE_DEBUG ((LM_DEBUG, "Callback method <get_bool_attr_excep> called: "));
  callback_excep++;
  try
    {
      excep_holder->raise_exception ();
    }
  catch (const A::DidTheRightThing& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                "Callback method <get_bool_attr_excep> exception "
                "received successfully <%d> and <%C>\n",
                ex.id, ex.whatDidTheRightThing.in ()));
      if (ex.id != 42)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: ex.id not 42 but %d\n", ex.id));
          result = 1;
        }
      if (ACE_OS::strcmp (ex.whatDidTheRightThing.in (), "Hello world") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                    "ERROR: ex.whatDidTheRightThing not ok: <%C>\n",
                    ex.whatDidTheRightThing.in ()));
          result = 1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR");
      ACE_ERROR ((LM_ERROR,
                  "... caught the wrong exception -> ERROR\n"));
    }
}

void
Handler::set_bool_attr ()
{
  callback_attrib++;
  ACE_DEBUG ((LM_DEBUG, "Callback method <set_bool_attr> called.\n"));
};

void
Handler::set_bool_attr_excep (::Messaging::ExceptionHolder *)
{
  callback_excep++;
  ACE_DEBUG ((LM_DEBUG, "Callback method <set_bool_attr_excep> called.\n"));
}

void
Handler::foo_struct (const char * ami_return_val,
  const ::A::structType & out_t)
{
  callback_operation++;
  ACE_DEBUG ((LM_DEBUG,
            "Callback method <foo_struct> called: t.as = %d return %C\n",
            out_t.as, ami_return_val));
  if ((out_t.as != 5) || ACE_OS::strcmp (ami_return_val, "bye from foo_struct") != 0)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: ami_return_val not 'bye from foo_struct' but '%C' "
                  "or t.as not <5> but <%d>\n",
                  ami_return_val, out_t.as));
      result = 1;
    }
}

void
Handler::foo_struct_excep (::Messaging::ExceptionHolder * )
{
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object_var =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa_var =
        PortableServer::POA::_narrow (object_var.in ());

      PortableServer::POAManager_var poa_manager_var =
        poa_var->the_POAManager ();

      poa_manager_var->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      // We reuse the object_var smart pointer!
      object_var = orb->string_to_object (ior);

      A::AMI_Test_var ami_test_var =
        A::AMI_Test::_narrow (object_var.in ());

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA.
      Handler handler;
      PortableServer::ObjectId_var id =
        poa_var->activate_object (std::addressof(handler));

      CORBA::Object_var object = poa_var->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var the_handler_var =
          A::AMI_AMI_TestHandler::_narrow (object.in ());

      // Trigger the DidTheRightThing exception on the server side
      // by sending 0 to it.
      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch message sendc_foo to "
                "trigger an exception\n"));

      ami_test_var->sendc_foo (the_handler_var.in (),
                               0,
                               "Let's talk AMI.");

      int32_t l = 931247;

      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch message foo.\n"));

      ami_test_var->sendc_foo (the_handler_var.in (),
                              l,
                              "Let's talk AMI.");

      // Begin test of attributes
      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch message sendc_get_yadda to "
                "trigger an exception\n"));
      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch attributes yadda.\n"));
      ami_test_var->sendc_set_yadda (the_handler_var.in (), 4711);
      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch message sendc_set_yadda to "
                "trigger an exception\n"));
      ami_test_var->sendc_set_yadda (the_handler_var.in (), 0);

      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch message sendc_get_bool_attr "
                "to trigger an exception\n"));
      ami_test_var->sendc_get_bool_attr (the_handler_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch attributes bool_attr.\n"));
      ami_test_var->sendc_set_bool_attr (the_handler_var.in (), false);
      ami_test_var->sendc_get_bool_attr (the_handler_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Client: Sending asynch message sendc_set_yadda to "
                "trigger an exception\n"));
      ami_test_var->sendc_set_bool_attr (the_handler_var.in (), true);

      // End test of attributes

      ami_test_var->sendc_foo_struct (the_handler_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Client: Sending another asynch message foo.\n"));

      ami_test_var->sendc_foo (the_handler_var.in (),
                                l,
                                "Let's talk AMI.");

      ACE_DEBUG ((LM_DEBUG, "Client: Sending the last  asynch message foo.\n"));

      ami_test_var->sendc_foo (the_handler_var.in (),
                               931235,
                               "Let's talk AMI the last time.");

      ACE_DEBUG ((LM_DEBUG, "Client: Do something else before coming back for "
                "the replies.\n"));

      for (int i = 0; i < 10; i ++)
        {
          ACE_DEBUG ((LM_DEBUG, " ..."));
          ACE_OS::sleep (ACE_Time_Value (0, 10 * 1000));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "\nClient: Now let's look for the replies.\n"));

      ACE_OS::sleep (2);
      while (!received_all_replies)
        {
          orb->perform_work ();
        }
      if ((callback_operation != 4) ||
           (callback_excep != 5 ) ||
           (callback_attrib != 4 ))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Client didn't recieve expected callbacks. "
                    "Foo: expected -4- received -%d-, "
                    "Attrib: expected -4-, received -%d-, "
                    "Exceptions: expected -5-, received -%d-\n",
                    callback_operation, callback_attrib, callback_excep));
          result = 1;
        }

      ami_test_var->shutdown ();

      poa_var->destroy (1,  // ethernalize objects
                        0); // wait for completion

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return result;
}
