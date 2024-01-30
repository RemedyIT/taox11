/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client ami test weak_ref
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
int16_t callback_operation = 0;

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

class Handler final
 :  public virtual CORBA::amic_traits<A::AMI_Test>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void foo (int32_t ami_return_val) override
    {
      TAOX11_TEST_INFO << "Callback method <foo> called: result <"
                  << ami_return_val
                  << ">"<< std::endl;
      callback_operation++;
      // Last reply flips the flag.
      if (ami_return_val == 10)
        {
          received_all_replies = 1;
        }
      else if (!(ami_return_val > 10))
        {
          TAOX11_TEST_ERROR << "ERROR:  method <foo>  ami_return_val not  > 10 : "
                       << ami_return_val << std::endl;
          result = 1;
        }
    }

  void foo_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
    {
      TAOX11_TEST_ERROR
         << "ERROR: Callback method <foo_excep> called."
         << std::endl;
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
               << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
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
      CORBA::amic_traits<A::AMI_Test>::replyhandler_weak_ref_type test_handler_weak;
      CORBA::amic_traits<A::AMI_Test>::replyhandler_weak_servant_ref_type test_svnt_handler_weak;
      // Use block for testing expired weak_ref_type's .
      {
        CORBA::amic_traits<A::AMI_Test>::replyhandler_servant_ref_type test_svnt_handler_ref =
             CORBA::make_reference<Handler> ();
        if (!test_svnt_handler_ref)
         {
           TAOX11_TEST_ERROR << "ERROR: Obtaining reference to Handler failed."
               << std::endl;
           return 1;
         }

        test_svnt_handler_weak = test_svnt_handler_ref.weak_reference ();

        if (test_svnt_handler_weak.expired())
          {
            TAOX11_TEST_ERROR
                 << "ERROR: test_svnt_handler_weak is expired."<< std::endl;
            return 1;
          }
        else
         {
           TAOX11_TEST_INFO << "OK: test_svnt_handler_weak isn't expired."
                       << std::endl;

         }

        CORBA::amic_traits<A::AMI_Test>::replyhandler_servant_ref_type test_handler_impl =
             test_svnt_handler_weak.lock ();

        if (!test_handler_impl)
         {
           TAOX11_TEST_ERROR
               << "ERROR: Obtaining reference test_handler_impl via lock weak_ref failed."
               << std::endl;
           return 1;
         }

        TAOX11_TEST_INFO << "created AMI_Test replyhandler servant" << std::endl;

        PortableServer::ObjectId id = root_poa->activate_object (test_handler_impl);

        TAOX11_TEST_INFO << "activated AMI_Test replyhandler servant" << std::endl;


        IDL::traits<CORBA::Object>::ref_type test_handler_obj =
             root_poa->id_to_reference (id);

        if (test_handler_obj == nullptr)
          {
            TAOX11_TEST_ERROR
                 << "ERROR: root_poa->id_to_reference (id) returned null reference."
                 << std::endl;
            return 1;
          }

        CORBA::amic_traits<A::AMI_Test>::replyhandler_ref_type test_handler_ref =
          CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow (test_handler_obj);

        if (test_handler_ref == nullptr)
          {
            TAOX11_TEST_ERROR
              << "ERROR: CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow"
              << " (test_handler_obj) returned null reference."
              << std::endl;
            return 1;
          }

        test_handler_weak =
            test_handler_ref.weak_reference ();

        CORBA::amic_traits<A::AMI_Test>::replyhandler_ref_type test_handler = test_handler_weak.lock();
        if (test_handler == nullptr)
          {
            TAOX11_TEST_ERROR
              << "ERROR: test_handler_weak.lock() returned null reference."
              << std::endl;
            return 1;
          }
        if (test_handler_weak.expired())
          {
             TAOX11_TEST_ERROR << "ERROR: test_handler_weak is expired."<< std::endl;
             return 1;
          }
        else
          {
            TAOX11_TEST_DEBUG << "OK: test_handler_weak isn't expired."<< std::endl;
          }
        poaman->activate ();

        TAOX11_TEST_INFO << "Client: Sending asynch message foo." << std::endl;

        ami_test_var->sendc_foo (test_handler, 2);

        TAOX11_TEST_INFO << "Client: Sending the last  asynch message foo." << std::endl;

        ami_test_var->sendc_foo (test_handler, 0);

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
            std::chrono::seconds tv (1);
            _orb->perform_work (tv);
          }

        if (callback_operation != 2)
          {
            TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks."
            << " expected -2- , received -" << callback_operation << "-."
            << std::endl;
            result = 1;
          }

      } //end block for testing expired weak_ref_type's .

      if (test_handler_weak.expired())
        {
          TAOX11_TEST_DEBUG << "OK: test_handler_weak is expired."<< std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR
              << "ERROR: test_handler_weak isn't expired."<< std::endl;
          result = 1;
        }

      // Deactivate POA to remove reference to reply handler servant
      poaman->deactivate (true,true);

      if (test_svnt_handler_weak.expired())
        {
          TAOX11_TEST_DEBUG << "OK: test_svnt_handler_weak is expired."<< std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR<< "ERROR: test_svnt_handler_weak isn't expired."<< std::endl;
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
