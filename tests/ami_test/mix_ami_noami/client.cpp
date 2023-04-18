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
int16_t ami_myfoo_foo = 0;

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

class AmiMyFooHandler final
: public virtual CORBA::amic_traits<A::AmiMyFoo>::replyhandler_base_type
{
public:
  /// Constructor.
  AmiMyFooHandler () = default;

  /// Destructor.
  ~AmiMyFooHandler () = default;

  void amifoo (int32_t ami_return_val) override
  {
    TAOX11_TEST_INFO << "Callback method <AmiMyFooHandler::foo> called: ret "
                << ami_return_val << std::endl;
    nr_of_replies--;
    ami_myfoo_foo++;
  }

  void amifoo_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
    nr_of_replies--;
  }

  void get_my_amifoo_attrib (int32_t ret) override
   {
     TAOX11_TEST_INFO << "Callback method <AmiMyFooHandler::get_my_foo_attrib> called: ret "
                 << ret << std::endl;
     nr_of_replies--;
   }

   void get_my_amifoo_attrib_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
   {
     TAOX11_TEST_INFO << "Callback method <get_my_foo_attrib_excep> called."
                 << std::endl;
   }

   void set_my_amifoo_attrib () override
   {
     TAOX11_TEST_INFO << "Callback method <AmiMyFooHandler::set_my_foo_attrib> called:"
                 << std::endl;
     nr_of_replies--;
   }

   void set_my_amifoo_attrib_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type  /*excep_holder*/) override
   {
     TAOX11_TEST_INFO << "Callback method <set_my_foo_attrib_excep> called:"
                 << std::endl;
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
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<A::Hello>::narrow(obj) returned null object."
                       << std::endl;
          return 1;
        }

       TAOX11_TEST_INFO << "narrowed A::Hello interface" << std::endl;


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

      //Handler for interface AmiMyFoo
      CORBA::amic_traits<A::AmiMyFoo>::replyhandler_servant_ref_type test_handler_impl_foo =
             CORBA::make_reference<AmiMyFooHandler> ();

      TAOX11_TEST_INFO << "created AmiMyFooHandler servant" << std::endl;

      PortableServer::ObjectId id_foo = root_poa->activate_object (test_handler_impl_foo);

      TAOX11_TEST_INFO << "activated AmiMyFooHandler servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type test_handler_foo_obj =
             root_poa->id_to_reference (id_foo);

      if (test_handler_foo_obj == nullptr)
        {
          TAOX11_TEST_ERROR
               << "ERROR: root_poa->id_to_reference (id) returned null reference."
               << std::endl;
          return 1;
        }

      CORBA::amic_traits<A::AmiMyFoo>::replyhandler_ref_type test_handler_foo =
          CORBA::amic_traits<A::AmiMyFoo>::replyhandler_traits::narrow (test_handler_foo_obj);

      if (test_handler_foo == nullptr)
        {
          TAOX11_TEST_ERROR
             << "ERROR: CORBA::amic_traits<A::AmiMyFoo>::replyhandler_traits::narrow (test_handler_foo_obj) returned null reference."
             << std::endl;
          return 1;
        }


      poaman->activate ();


      A::MyFoo::_ref_type i_foo = ami_test_var->get_iMyFoo();
      if (!i_foo)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iMyFoo returned null object." << std::endl;
        return 1;
      }
      A::AmiMyFoo::_ref_type i_ami_foo_ref = ami_test_var->get_iAmiMyFoo();
      if (!i_ami_foo_ref)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieve of iAmiMyFoo returned null object." << std::endl;
        return 1;
      }
      CORBA::amic_traits<A::AmiMyFoo>::ref_type i_ami_foo = CORBA::amic_traits<A::AmiMyFoo>::narrow (i_ami_foo_ref);
       if (!i_ami_foo)
         {
           TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::AmiMyFoo>::narrow (i_ami_foo_ref) returned null object."
                        << std::endl;
           return 1;
         }

      TAOX11_TEST_INFO << "Client: Sending 1 synch messages for MyFoo::foo." << std::endl;
      const int32_t res = i_foo->foo (24);
      if (res == 22)
        myfoo_foo++;

      TAOX11_TEST_INFO << "Client: Sending 1 asynch message AmiMyFoo::foo." << std::endl;
      i_ami_foo->sendc_amifoo (test_handler_foo, 11);


      TAOX11_TEST_INFO << "Client: Sending 2 asynch messages for AmiMyFoo::my_foo_attrib attributes." << std::endl;
      i_ami_foo->sendc_set_my_amifoo_attrib (test_handler_foo, 101);
      i_ami_foo->sendc_get_my_amifoo_attrib (test_handler_foo);



      nr_of_replies = 3;

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

      if ((nr_of_replies != 0) || (ami_myfoo_foo != 1) || (myfoo_foo != 1))
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected replies. Expected -3- , received -"
          << (3 - nr_of_replies) << "-. " << " Expected AmiMyFoo- foo <1> , received <"
          << ami_myfoo_foo << ">. Expected MyFoo- foo <1>, received <" << myfoo_foo << ">." << std::endl;
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
