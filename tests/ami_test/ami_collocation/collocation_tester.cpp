/**
 * @file    collocation_tester.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 AMI Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "collocation_tester.h"
#include "ami_test_i.h"

#include "testlib/taox11_testlog.h"

static int16_t received_last_reply {};
int16_t callback_operation {};
bool received_reply {};

void
Handler::foo (int32_t ami_return_val,
              int32_t out_l)
{
  TAOX11_TEST_INFO << "Callback method <foo> called: result <" << ami_return_val
                   << ">, out_arg <" << out_l << ">"<< std::endl;
  received_reply = true;
  callback_operation++;

  // Last reply flips the flag.
  if (out_l == 931235)
  {
    received_last_reply = 1;
  }
}

void
Handler::foo_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder)
{
  TAOX11_TEST_ERROR
         << "ERROR: Unexpected exception <foo_excep> called"
         << std::endl;
  received_reply = true;
  excep_holder->raise_exception ();
}


Collocation_Test::Collocation_Test ()
{
}

int
Collocation_Test::shutdown ()
{
  int result {};
  if (callback_operation != 6)
  {
    TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks."
        << " Foo : expected -6- , received -" << callback_operation << "-"
        << std::endl;
    result = 1;
  }

  this->root_poa_->destroy (true, true);
  this->orb_->destroy ();
  return result;
}

int
Collocation_Test::init (int argc, char *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);
  if (this->orb_ == nullptr)
  {
    TAOX11_TEST_ERROR
         << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
         << std::endl;
    return 1;
  }

  IDL::traits<CORBA::Object>::ref_type obj =
    this->orb_->resolve_initial_references ("RootPOA");
  if (!obj)
  {
    TAOX11_TEST_ERROR
        << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
        << std::endl;
    return 1;
  }
  TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;

  this->root_poa_ = IDL::traits<PortableServer::POA>::narrow (obj);
  if (!this->root_poa_)
  {
    TAOX11_TEST_ERROR
         << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
         << std::endl;
    return 1;
  }

  TAOX11_TEST_INFO << "narrowed POA interface" << std::endl;


  this->poa_manager_ = this->root_poa_->the_POAManager ();
  if (!this->poa_manager_)
  {
    TAOX11_TEST_ERROR
        << "ERROR: root_poa->the_POAManager () returned null object."
        << std::endl;
    return 1;
  }


  CORBA::servant_traits<A::AMI_Test>::ref_type hello_impl =
     CORBA::make_reference<AMI_Test_i> ();


  TAOX11_TEST_INFO << "created A::AMI_Test servant" << std::endl;

  PortableServer::ObjectId id = this->root_poa_->activate_object (hello_impl);

  TAOX11_TEST_INFO << "activated A::AMI_Test servant" << std::endl;

  IDL::traits<CORBA::Object>::ref_type ami_obj_= this->root_poa_->id_to_reference (id);
  this->ami_test_var_ = CORBA::amic_traits<A::AMI_Test>::narrow (ami_obj_);
  if (!this->ami_test_var_)
  {
    TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::AMI_Test>::narrow (obj) returned null object."
                      << std::endl;
    return 1;
  }


  CORBA::amic_traits<A::AMI_Test>::replyhandler_servant_ref_type test_handler_impl =
          CORBA::make_reference<Handler> ();

  TAOX11_TEST_INFO << "created AMI_Test replyhandler servant" << std::endl;

  PortableServer::ObjectId idrh = this->root_poa_->activate_object (test_handler_impl);

  TAOX11_TEST_INFO << "activated AMI_Test replyhandler servant" << std::endl;

  IDL::traits<CORBA::Object>::ref_type test_handler_obj =
         this->root_poa_->id_to_reference (idrh);

  if (test_handler_obj == nullptr)
  {
    TAOX11_TEST_ERROR
              << "ERROR: root_poa->id_to_reference (id) returned null reference."
              << std::endl;
    return 1;
  }
  this->test_handler_ =
         CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow (test_handler_obj);

  if (this->test_handler_ == nullptr)
  {
    TAOX11_TEST_ERROR
              << "ERROR: CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow"
              << " (test_handler_obj) returned null reference."
              << std::endl;
    return 1;
  }
  TAOX11_TEST_DEBUG << "Init - Finished" << std::endl;
  return 0;
}

int
Collocation_Test::run ()
{
  this->poa_manager_->activate ();

  int result {};
  received_reply = true;
  //asynchronous callbacks should become received synchronous.
  for (int32_t i = 0; i < 5; i ++)
  {
    // Expected to have received an answer before you come here again.
    // if !received_reply, error because no answer received in
    // Handler::foo
    if (!received_reply)
    {
      TAOX11_TEST_ERROR << "Error: ami doesn't handle replies synchronously" << std::endl;
      result = 1;
      break;
    }
    else
    {
      received_reply = false;
      if (i > 0)
      {
        TAOX11_TEST_INFO << "Client: Received reply  foo " << i -1 << " synchronous." << std::endl;
      }
      TAOX11_TEST_INFO << "Client: Sending asynch message foo - " << i << std::endl;
      this->ami_test_var_->sendc_foo (this->test_handler_,
                                      i,
                                      "Let's talk AMI.");
    }
  }
  TAOX11_TEST_INFO << "Client: Sending the last  asynch message foo." << std::endl;

  this->ami_test_var_->sendc_foo (this->test_handler_,
                                  931235,
                                 "Let's talk AMI the last time.");

  // Now let's look if we have alredy received  the last reply.
  if (!received_last_reply)
  {
    TAOX11_TEST_ERROR << "Error: didn't receive the last reply" << std::endl;
  }

  return result;
}
