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
int16_t id_o = 0;
int16_t ic_o = 0;
int16_t id_excep = 0;
int16_t ic_excep = 0;

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

class AHandler
: public virtual CORBA::amic_traits<Test::A>::replyhandler_base_type
{
public:
  /// Constructor.
  AHandler () = default;

  /// Destructor.
  ~AHandler () = default;

  void op_A (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO <<
        "ERROR: Callback method <AHandler::op_A> called unexpectedly: ret "
        << ami_return_val << std::endl;
    result = 1;
    nr_of_replies--;
  }
  void op_A_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    TAOX11_TEST_INFO
       << "ERROR:: Callback method <AHandler::op_A_excep> called unexpectedly."
       << std::endl;
    result = 1;
    nr_of_replies--;
  }

};

class BHandler
: public virtual CORBA::amic_traits<Test::B>::replyhandler_base_type
{
public:
  /// Constructor.
  BHandler () = default;

  /// Destructor.
  ~BHandler () = default;

  void op_A (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO
         << "ERROR: Callback method <BHandler::op_A> called unexpectedly: ret "
         << ami_return_val << std::endl;
    result = 1;
    nr_of_replies--;
  }

  void op_A_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type  /*excep_holder*/) override
  {
    TAOX11_TEST_INFO
       << "ERROR:: Callback method <BHandler::op_A_excep> called unexpectedly."
       << std::endl;
    result = 1;
    nr_of_replies--;
  }

  void op_B (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO
          << "ERROR: Callback method <BHandler::op_A> called unexpectedly: ret "
          << ami_return_val << std::endl;
    result = 1;
    nr_of_replies--;
  }
  void op_B_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type  /*excep_holder*/) override
  {
    TAOX11_TEST_INFO
       << "ERROR:: Callback method <BHandler::op_B_excep> called unexpectedly."
       << std::endl;
    result = 1;
    nr_of_replies--;
  }

};

class CHandler
: public virtual CORBA::amic_traits<Test::C>::replyhandler_base_type
{
public:
  /// Constructor.
  CHandler () = default;

  /// Destructor.
  ~CHandler () = default;

  void op_A (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <CHandler::op_A> called: ret "
                << ami_return_val << std::endl;
    if (ami_return_val != 111)
      {
      TAOX11_TEST_ERROR << "ERROR: Callback method <CHandler::op_A> ami_return_val not 1111: "
                   << ami_return_val << std::endl;
        result = 1;
      }
    nr_of_replies--;
    ic_o++;
  }

  void op_A_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Test::IntErr& ex)
      {
        ic_excep++;
        TAOX11_TEST_INFO
          << "Callback method <CHandler::op_A_excep> exception received successfully <"
          << ex << ">." << std::endl;
        if (ex.id() != 111)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <CHandler::op_A_excep> exception ex.id not 111 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
       }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <CHandler::op_A_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;

  }

  void op_C (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <CHandler::op_C> called: ret "
                << ami_return_val << std::endl;
    if (ami_return_val != 113)
      {
      TAOX11_TEST_ERROR << "ERROR: Callback method <CHandler::op_C> ami_return_val not 113: "
                   << ami_return_val << std::endl;
        result = 1;
      }
    nr_of_replies--;
    ic_o++;
  }

  void op_C_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Test::IntErr& ex)
      {
        ic_excep++;
        TAOX11_TEST_INFO
           << "Callback method <CHandler::op_C_excep> exception received successfully <"
           << ex << ">." << std::endl;
        if (ex.id() != 113)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <CHandler::op_C_excep> exception ex.id not 113 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
       }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <CHandler::op_C_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;
  }

};

class DHandler
: public virtual CORBA::amic_traits<Test::D>::replyhandler_base_type
{
public:
  /// Constructor.
  DHandler () = default;

  /// Destructor.
  ~DHandler () = default;

  void op_A (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <DHandler::op_A> called: ret "
        << ami_return_val << std::endl;
    if (ami_return_val != 1111)
      {
      TAOX11_TEST_ERROR << "ERROR: Callback method <DHandler::op_A>  ami_return_val not 1111: "
                   << ami_return_val << std::endl;
        result = 1;
      }
    nr_of_replies--;
    id_o++;
  }
  void op_A_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Test::IntErr& ex)
      {
        id_excep++;
        TAOX11_TEST_INFO
          << "Callback method <DHandler::op_A_excep> exception received successfully <"
          << ex << ">." << std::endl;
        if (ex.id() != 1111)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <DHandler::op_A_excep> exception ex.id not 1111 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
       }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <DHandler::op_A_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;
  }

  void op_B (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <DHandler::op_B> called: ret "
                << ami_return_val << std::endl;
    if (ami_return_val != 1112)
      {
      TAOX11_TEST_ERROR << "ERROR: Callback method <DHandler::op_B> ami_return_val not 1112: "
                   << ami_return_val << std::endl;
        result = 1;
      }
    nr_of_replies--;
    id_o++;
  }

  void op_B_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Test::IntErr& ex)
      {
        id_excep++;
        TAOX11_TEST_INFO
            << "Callback method <DHandler::op_B_excep> exception received successfully <"
            << ex << ">." << std::endl;
        if (ex.id() != 1112)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <DHandler::op_B_excep> exception ex.id not 1112 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
       }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <DHandler::op_B_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;
  }

  void op_C (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <DHandler::op_C> called: ret "
                << ami_return_val << std::endl;
    if (ami_return_val != 1113)
      {
      TAOX11_TEST_ERROR << "ERROR: Callback method <DHandler::op_C> ami_return_val not 1113: "
                   << ami_return_val << std::endl;
        result = 1;
      }
    nr_of_replies--;
    id_o++;
  }

  void op_C_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Test::IntErr& ex)
      {
        id_excep++;
        TAOX11_TEST_INFO
           << "Callback method <DHandler::op_C_excep> exception received successfully <"
           << ex << ">." << std::endl;
        if (ex.id() != 1113)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <DHandler::op_C_excep> exception ex.id not 1113 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
       }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <DHandler::op_C_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;  }

  void op_D (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <DHandler::op_D> called: ret "
                << ami_return_val << std::endl;
    if (ami_return_val != 1114)
      {
        TAOX11_TEST_ERROR << "ERROR: Callback method <DHandler::op_D>  ami_return_val not 1114: "
                     << ami_return_val << std::endl;
        result = 1;
      }
    nr_of_replies--;
    id_o++;
  }

  void op_D_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type excep_holder) override
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Test::IntErr& ex)
      {
        id_excep++;
        TAOX11_TEST_INFO
               << "Callback method <DHandler::op_D_excep> exception received successfully <"
               << ex << ">." << std::endl;
        if (ex.id() != 1114)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <DHandler::op_D_excep> exception ex.id not 1114 but "
              << ex.id() <<  std::endl;
            result = 1;
          }
       }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <DHandler::op_D_excep> caught the wrong exception"
          << std::endl;
        result = 1;
      }
    nr_of_replies--;
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

      IDL::traits<Test::Hello>::ref_type ami_test_var = IDL::traits<Test::Hello>::narrow (obj);

      if (!ami_test_var)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed Test::Hello interface" << std::endl;


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

      //Handler for interface D
      CORBA::amic_traits<Test::D>::replyhandler_servant_ref_type test_handler_impl_d =
           CORBA::make_reference<DHandler> ();

      TAOX11_TEST_INFO << "created DHandler servant" << std::endl;

      PortableServer::ObjectId id_d = root_poa->activate_object (test_handler_impl_d);

      TAOX11_TEST_INFO << "activated DHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_d_obj =
           root_poa->id_to_reference (id_d);

      if (test_handler_d_obj == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: root_poa->id_to_reference (id) returned null reference."
             << std::endl;
           return 1;
        }

      CORBA::amic_traits<Test::D>::replyhandler_ref_type test_handler_d =
          CORBA::amic_traits<Test::D>::replyhandler_traits::narrow (test_handler_d_obj);

      if (test_handler_d == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<Test::D>::replyhandler_traits::narrow(test_handler_d_obj) returned null reference."
             << std::endl;
          return 1;
        }

      //Handler for interface C
      CORBA::amic_traits<Test::C>::replyhandler_servant_ref_type test_handler_impl_c =
             CORBA::make_reference<CHandler> ();

      TAOX11_TEST_INFO << "created CHandler servant" << std::endl;

      PortableServer::ObjectId id_c = root_poa->activate_object (test_handler_impl_c);

      TAOX11_TEST_INFO << "activated CHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_c_obj =
             root_poa->id_to_reference (id_c);

      if (test_handler_c_obj == nullptr)
        {
          TAOX11_TEST_ERROR
               << "ERROR: root_poa->id_to_reference (id) returned null reference."
               << std::endl;
          return 1;
        }

      CORBA::amic_traits<Test::C>::replyhandler_ref_type test_handler_c =
          CORBA::amic_traits<Test::C>::replyhandler_traits::narrow (test_handler_c_obj);

      if (test_handler_c == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<Test::C>::replyhandler_traits::narrow(test_handler_a_obj) returned null reference."
             << std::endl;
          return 1;
        }

      poaman->activate ();

      Test::D::_ref_type i_d = ami_test_var->get_iD();
      if (!i_d)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iD returned null object." << std::endl;
        return 1;
      }
      CORBA::amic_traits<Test::D>::ref_type test_async_d = CORBA::amic_traits<Test::D>::narrow (i_d);
      if (!test_async_d)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<Test::D>::narrow (i_d) returned null object."
                     << std::endl;
        return 1;
      }

      Test::C::_ref_type i_c = ami_test_var->get_iC();
      if (!i_c)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iD returned null object." << std::endl;
        return 1;
      }
      CORBA::amic_traits<Test::C>::ref_type test_async_c = CORBA::amic_traits<Test::C>::narrow (i_c);
      if (!test_async_c)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<Test::C>::narrow (i_c) returned null object."
                     << std::endl;
        return 1;
      }

      TAOX11_TEST_INFO << "Client: Sending 6 asynch messages D::A, D::B, D::C, D::D, C::A and C::C ." << std::endl;

      test_async_d->sendc_op_A (test_handler_d, 9);
      test_async_d->sendc_op_B (test_handler_d, 8);
      test_async_d->sendc_op_C (test_handler_d, 7);
      test_async_d->sendc_op_D (test_handler_d, 6);

      test_async_c->sendc_op_A (test_handler_c, 5);
      test_async_c->sendc_op_C (test_handler_c, 4);

      TAOX11_TEST_INFO << "Client: Sending 6 asynch messages to trigger an exception: "
                  << " D::A, D::B, D::C, D::D, C::A and C::C ." << std::endl;

      test_async_d->sendc_op_A (test_handler_d, 0);
      test_async_d->sendc_op_B (test_handler_d, 0);
      test_async_d->sendc_op_C (test_handler_d, 0);
      test_async_d->sendc_op_D (test_handler_d, 0);

      test_async_c->sendc_op_A (test_handler_c, 0);
      test_async_c->sendc_op_C (test_handler_c, 0);


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

      if ((nr_of_replies != 0) || (id_o != 4) || (ic_o != 2))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected replies. Expected -12- , received -"
          << (12 - nr_of_replies) << "-. " << " Expected D callbacks <4> , received <"
          << id_o << ">. Expected C callbacks <2>, received <" << ic_o << ">." << std::endl;
          result = 1;
        }

      if ((id_excep != 4) || (ic_excep != 2))
         {
           TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected  exceptions. "
            << "Expected D callbacks exceptions <4> , received <"
            << id_excep << ">. Expected C callbacks exceptions <2>, received <"
            << ic_excep << ">." << std::endl;
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
