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

#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int16_t result = 0;

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

  void foo (int32_t /*ami_return_val*/) override
  {
  }

  void foo_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type) override
  {
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


      IDL::traits<A::Test>::ref_type a_sync = IDL::traits<A::Test>::narrow (obj);

      if (!a_sync)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<A::Test>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      std::stringstream ostream_a_sync;
      ostream_a_sync << a_sync;
      std::string ostream_a_sync_str = ostream_a_sync.str();
      if ((ostream_a_sync_str.find ("IDL:A/Test:1.0") == std::string::npos) ||
          (ostream_a_sync_str.find ("<A::Test>") == std::string::npos))
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<A::Test>::ref_type returned wrong ostream info: "
                       << ostream_a_sync_str << std::endl;
          result = 1;
        }

      TAOX11_TEST_INFO << "ostream test IDL::traits<A::Test>::ref_type: <" << a_sync
                  << std::endl;

      CORBA::amic_traits<A::Test>::ref_type a_async = CORBA::amic_traits<A::Test>::narrow (obj);
      if (!a_async)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::TestAmiC>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      std::stringstream ostream_a_async;
      ostream_a_async << a_async;
      std::string ostream_a_async_str = ostream_a_async.str();
      if ((ostream_a_async_str.find ("IDL:A/Test:1.0") == std::string::npos) ||
          (ostream_a_async_str.find ("amic_traits") == std::string::npos) ||
          (ostream_a_async_str.find ("A::Test") == std::string::npos))
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Test>::ref_type returned wrong ostream info: "
                       << ostream_a_async_str << std::endl;
          result = 1;
        }

      TAOX11_TEST_INFO << "ostream test CORBA::amic_traits<A::Test>::ref_type: <" << a_async
                  << std::endl;

      // Instantiate the ReplyHandler and register that with the POA.
      IDL::traits<CORBA::Object>::ref_type poa_obj =
           _orb->resolve_initial_references ("RootPOA");

      if (!poa_obj)
         {
           TAOX11_TEST_ERROR
           << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
           return 1;
         }

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

      CORBA::amic_traits<A::Test>::replyhandler_servant_ref_type test_handler_impl =
           CORBA::make_reference<Handler> ();

      TAOX11_TEST_INFO << "created TestHandler servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (test_handler_impl);

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

      poaman->activate ();

      std::stringstream ostream_rh;
      ostream_rh << test_handler;
      std::string ostream_rh_str = ostream_rh.str();
      if ((ostream_rh_str.find ("IDL:A/AMI_TestHandler:1.0") == std::string::npos)||
          (ostream_rh_str.find ("<A::AMI_TestHandler>") == std::string::npos))
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<A::Test>::ref_type returned wrong ostream info: "
                       << ostream_rh_str << std::endl;
          result = 1;
        }

      TAOX11_TEST_INFO << "ostream test CORBA::amic_traits<A::Test>::replyhandler_ref_type: <" << test_handler
                  << std::endl;

      a_async->shutdown ();

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
