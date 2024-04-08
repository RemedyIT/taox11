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
#include <thread>

#include "hello2C.h"
#include "hello1AmiC.h"

#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int16_t result = 0;
int16_t nr_of_replies = 0;
int16_t cb_a = 0;
int16_t cb_a_excep = 0;

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
    TAOX11_TEST_INFO << "Callback method <AHandler::op_A> called: ret "
                << ami_return_val << std::endl;
    if (ami_return_val != 109)
      {
        TAOX11_TEST_ERROR << "ERROR: Callback method <AHandler::op_A> ami_return_val not 109: "
                    << ami_return_val << std::endl;
        result = 1;
      }
    cb_a++;
    nr_of_replies--;
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
        cb_a_excep++;
        TAOX11_TEST_INFO
           << "Callback method <AHandler::op_A_excep> exception received successfully <"
           << ex << ">." << std::endl;
        if (ex.id() != 1)
          {
            TAOX11_TEST_ERROR
              << "ERROR: Callback method <AHandler::op_A_excep> exception ex.id not 1 but "
              << ex.id() <<  std::endl;
             result = 1;
          }
      }
    catch (const CORBA::Exception& )
      {
        TAOX11_TEST_ERROR
          << "ERROR: Callback method <AHandler::op_A_excep> caught the wrong exception"
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

      IDL::traits<Test::Hello>::ref_type test_var = IDL::traits<Test::Hello>::narrow (obj);

      if (!test_var)
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

      //Handler for interface A
      CORBA::amic_traits<Test::A>::replyhandler_servant_ref_type test_handler_impl_a =
           CORBA::make_reference<AHandler> ();

      TAOX11_TEST_INFO << "created DHandler servant" << std::endl;

      PortableServer::ObjectId ia_o = root_poa->activate_object (test_handler_impl_a);

      TAOX11_TEST_INFO << "activated DHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_a_obj =
           root_poa->id_to_reference (ia_o);

      if (test_handler_a_obj == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: root_poa->id_to_reference (id) returned null reference."
             << std::endl;
           return 1;
        }

      CORBA::amic_traits<Test::A>::replyhandler_ref_type test_handler_a =
          CORBA::amic_traits<Test::A>::replyhandler_traits::narrow (test_handler_a_obj);

      if (test_handler_a == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR:  CORBA::amic_traits<Test::A>::replyhandler_traits::narrow (test_handler_a_obj) returned null reference."
             << std::endl;
          return 1;
        }

      poaman->activate ();

      Test::A::_ref_type i_a = test_var->get_iA();
      if (!i_a)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned null object." << std::endl;
        return 1;
      }
      CORBA::amic_traits<Test::A>::ref_type test_async_a = CORBA::amic_traits<Test::A>::narrow (i_a);
      if (!test_async_a)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<Test::A>::narrow (i_a) returned null object."
                     << std::endl;
        return 1;
      }

      Test::B::_ref_type i_b = test_var->get_iB();
      if (!i_b)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iB returned null object." << std::endl;
        return 1;
      }
      CORBA::amic_traits<Test::B>::ref_type test_async_b = CORBA::amic_traits<Test::B>::narrow (i_b);
      if (!test_async_b)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<Test::B>::narrow (i_b) returned null object."
                     << std::endl;
        return 1;
      }

      TAOX11_TEST_INFO << "Client: Sending synch messages B::op_B." << std::endl;

      int32_t res_b = i_b->op_B (8);
      if (res_b != 108)
         {
           TAOX11_TEST_ERROR << "ERROR: return value op_B  not 108: "
                       << res_b << std::endl;
           result = 1;
         }

      TAOX11_TEST_INFO << "Client: Sending asynch message A::op_A." << std::endl;

      test_async_a->sendc_op_A (test_handler_a, 9, i_b);

      TAOX11_TEST_INFO << "Client: Sending asynch messages A::op_A to trigger an exception." << std::endl;
      test_async_a->sendc_op_A (test_handler_a, 0, i_b);

      nr_of_replies = 2;

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

      if ((nr_of_replies != 0) || (cb_a != 1))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected replies. Expected -2- , received -"
          << (2 - nr_of_replies) << "-. " << " Expected A callbacks <1> , received <"
          << cb_a << ">." << std::endl;
          result = 1;
        }

      if (cb_a_excep != 1)
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected  exceptions. "
            << "Expected A callbacks exceptions <1> , received <"
            << cb_a_excep << ">." << std::endl;
            result = 1;
        }

      test_var->shutdown ();

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
