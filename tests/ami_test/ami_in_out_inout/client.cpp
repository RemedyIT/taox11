/**
 * @file   client.cpp
 * @author Marijke Hengstmengel
 *
 * @brief  CORBA C++11 client ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_testAmiC.h"
#include <thread>
#include <cmath>

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

class Handler
: public virtual CORBA::amic_traits<A::AMI_Test>::replyhandler_base_type
{
public:
  /// Constructor.
  Handler () = default;

  /// Destructor.
  ~Handler () = default;

  void foo_char (char ami_return_val,
                 char out_c,
                 char inout_c) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_char> called: " << ami_return_val
                 << " " << out_c << " " << inout_c << std::endl;

    if (out_c == 'x')
      {
        received_all_replies = 1;
      }
    else
      {
        if ((out_c != 'c') || (inout_c != 'd') || (ami_return_val != 'e'))
          {
            TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_char out/inout/ami_return_val: "
                         << "expected 'c','d' and 'e', received: "
                         << out_c << " , " << inout_c  << " and "
                         << ami_return_val << std::endl;
            result = 1;
          }
      }
  }

  void foo_char_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_wchar (wchar_t ami_return_val,
                  wchar_t out_wc,
                  wchar_t inout_wc) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_wchar> called: " << ami_return_val
                << " " << out_wc << " " << inout_wc << std::endl;
    if ((out_wc != 'g') || (inout_wc != 'h') || (ami_return_val != 'i'))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_wchar out/inout/ami_return_val: "
                     << "expected 'g','h' and 'i', received: "
                     << out_wc << " , " << inout_wc  << " and "
                     << ami_return_val << std::endl;
        result = 1;
     }
  }

  void foo_wchar_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_octet (uint8_t ami_return_val,
                  uint8_t out_o,
                  uint8_t inout_o) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_octet> called: " << ami_return_val
                << " " << out_o << " " << inout_o << std::endl;
    if ((out_o != 14) || (inout_o != 15) || (ami_return_val != 16))
      {
       TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_octet out/inout/ami_return_val: "
                    << "expected '14','15' and '16', received: "
                    << out_o << " , " << inout_o  << " and "
                    << ami_return_val << std::endl;
       result = 1;
     }
  };

  void foo_octet_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_bool (bool ami_return_val,
                 bool inout_b,
                 bool out_b) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_bool> called: " << ami_return_val
                << " " << out_b << " " << inout_b << std::endl;
    if ((out_b != true) || (inout_b != false) || (ami_return_val != true))
       {
         TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_bool out/inout/ami_return_val: "
                      << "expected 'true','false' and 'true', received: "
                      << out_b << " , " << inout_b  << " and "
                      << ami_return_val << std::endl;
         result = 1;
      }

  }

  void foo_bool_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_enum (A::enumType ami_return_val,
                 A::enumType out_e,
                 A::enumType inout_e) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_enum> called: " << ami_return_val
                << " " << out_e << " " << inout_e << std::endl;
     if ((out_e != A::enumType::ce) || (inout_e != A::enumType::de) || (ami_return_val != A::enumType::ee))
        {
          TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_enum out/inout/ami_return_val: "
                       << "expected 'ce','de' and 'ee', received: "
                       << out_e << " , " << inout_e  << " and "
                       << ami_return_val << std::endl;
          result = 1;
       }

  }

  void foo_enum_excep (
    IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_struct (const A::structType& ami_return_val,
                   const A::structType& out_t,
                   const A::structType& inout_t) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_struct> called: " << ami_return_val
                << " " << out_t << " " << inout_t << std::endl;
    if ((out_t.as() != 3) || (inout_t.as() != 40) || (ami_return_val.bs() != 6))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_struct out/inout/ami_return_val: "
                     << "expected '3','40' and '6', received: "
                     << out_t.as() << " , " << inout_t.as()  << " and "
                     << ami_return_val.bs() << std::endl;
        result = 1;
     }

  }

  void foo_struct_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_sequence (const A::longSeq& ami_return_val,
                     const A::longSeq& out_seq,
                     const A::longSeq& inout_seq) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_sequence> called: " << ami_return_val
                << " " << out_seq << " " << inout_seq << std::endl;
    if ((out_seq[1] != 3) || (inout_seq[1]!= 20) || (ami_return_val[1] != 50))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_sequence out/inout/ami_return_val: "
                     << "expected '3','20' and '50', received: "
                     << out_seq[1] << " , " << inout_seq[1]  << " and "
                     << ami_return_val[1] << std::endl;
        result = 1;
      }
  }


  void foo_sequence_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_array (const A::arrayType& ami_return_val,
                   const A::arrayType& out_a,
                   const A::arrayType& inout_a) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_array> called: " << ami_return_val
                << " " << out_a << " " << inout_a << std::endl;
    A::arrayType expected_out = { { 1, 2, 3, 4, 5 } };
    A::arrayType expected_ret = { { 10, 20, 30, 40, 50 } };
    A::arrayType expected_inout = { { 100, 200, 300, 400, 500 } };
    if ((out_a != expected_out) || (inout_a != expected_inout) || (ami_return_val != expected_ret))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_sequence out/inout/ami_return_val: "
                     << "expected: " << expected_out << " , " << expected_inout
                     << " , " << expected_ret << " received: "
                     << out_a << " , " << inout_a  << " and "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_array_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_union ( const A::unionData& ami_return_val,
                   const A::unionData& out_u,
                   const A::unionData& inout_u) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_union> called: " << ami_return_val
                << " " << out_u << " " << inout_u << std::endl;
    if ((out_u.longData() != 3) || (inout_u.longData() != 4) || (ami_return_val.shortData() != 5))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_union out/inout/ami_return_val: "
                     << "expected '3','4' and '5', received: "
                     << out_u << " , " << inout_u  << " and "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_union_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  };


  void foo_wstring (const std::wstring& ami_return_val,
                     const std::wstring& out_w,
                     const std::wstring& inout_w) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_wstring> called: " << ami_return_val
                << " " << out_w << " " << inout_w << std::endl;
    if ((out_w != L"cc") || (inout_w != L"dd") || (ami_return_val != L"ee"))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_wstring out/inout/ami_return_val: "
                     << "expected 'cc','dd' and 'ee', received: "
                     << out_w << " , " << inout_w  << " and "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_wstring_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_string (const std::string& ami_return_val,
                   const std::string& out_s,
                   const std::string& inout_s) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_string> called: " << ami_return_val
                << " " << out_s << " " << inout_s << std::endl;
    if ((out_s != "hh") || (inout_s != "ii") || (ami_return_val != "jj"))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_string out/inout/ami_return_val: "
                     << "expected 'hh','ii' and 'jj', received: "
                     << out_s << " , " << inout_s  << " and "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_string_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  };

  void foo_u_l_l (uint64_t ami_return_val,
                  uint64_t out_ull,
                  uint64_t inout_ull) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_u_l_l> called: " << ami_return_val
                << " " << out_ull << " " << inout_ull << std::endl;
    if ((out_ull != 30000) || (inout_ull != 40000) || (ami_return_val != 50000))
       {
         TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_u_l_l out/inout/ami_return_val: "
                      << "expected '30000','40000' and '50000', received: "
                      << out_ull << " , " << inout_ull  << " and "
                      << ami_return_val << std::endl;
         result = 1;
      }
  }

  void foo_u_l_l_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_float (float ami_return_val,
                  float out_f,
                  float inout_f) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_float> called: " << ami_return_val
                 << " " << out_f << " " << inout_f << std::endl;
    float exp_out_f = 1.3f;
    float exp_inout_f = 1.4f;
    float exp_ret_f = 1.5f;

    if ((out_f != exp_out_f) || (inout_f != exp_inout_f) || (ami_return_val != exp_ret_f))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_float out/inout/ami_return_val: "
                     << "expected '1.3','1.4' and '1.5', received: "
                     << out_f << " , " << inout_f  << " and "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_float_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_long_double (long double ami_return_val,
                        long double out_ld,
                        long double inout_ld) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_long_double> called: " << ami_return_val
                 << " " << out_ld << " " << inout_ld << std::endl;
    long double exp_out_ld = 2.3;
    long double exp_inout_ld = 2.4;
    long double exp_ret_ld = 2.5;
    if ((std::fabs (out_ld - exp_out_ld) > std::numeric_limits<long double>::epsilon ()) ||
        (std::fabs (inout_ld - exp_inout_ld) > std::numeric_limits<long double>::epsilon ()) ||
        (std::fabs (ami_return_val - exp_ret_ld) > std::numeric_limits<long double>::epsilon ()))
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_long_double out/inout/ami_return_val: "
                     << "expected '2.3','2.4' and '2.5', received: "
                     << out_ld << " , " << inout_ld  << " and "
                     << ami_return_val << std::endl;
        result = 1;
      }
  }

  void foo_long_double_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_void_in () override
  {
    TAOX11_TEST_INFO << "Callback method <foo_void_in> called: " << std::endl;
    callback_operation++;
  }

  void foo_void_in_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_void_out (int32_t out_l) override
  {
    TAOX11_TEST_INFO << "Callback method <foo_void_out> called: " << std::endl;
    callback_operation++;
    if (out_l != 250)
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_void_out : "
                     << "expected 250, received: "
                     << out_l << std::endl;
        result = 1;
      }
  }

  void foo_void_out_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_void_inout (int32_t inout_l) override
  {
    TAOX11_TEST_INFO << "Callback method <foo_void_inout> called: " << std::endl;
    callback_operation++;
    if (inout_l != 450)
      {
        TAOX11_TEST_ERROR << "ERROR:  Client Handler::foo_void_inout : "
                     << "expected 450, received: "
                     << inout_l << std::endl;
        result = 1;
      }
  }

  void foo_void_inout_excep (
       IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_no_arg (int32_t ami_return_val) override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_no_arg> called: " << ami_return_val
                << std::endl;
  }


  void foo_no_arg_excep (
     IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
  {
  }

  void foo_void_no_arg () override
  {
    callback_operation++;
    TAOX11_TEST_INFO << "Callback method <foo_void_no_arg> called: " << std::endl;

  }

  void foo_void_no_arg_excep (
      IDL::traits<::Messaging::ExceptionHolder>::ref_type /*excep_holder*/) override
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

      CORBA::amic_traits<A::AMI_Test>::replyhandler_servant_ref_type test_handler_impl =
           CORBA::make_reference<Handler> ();

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

      CORBA::amic_traits<A::AMI_Test>::replyhandler_ref_type test_handler =
          CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow (test_handler_obj);

      if (test_handler == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow"
            << " (test_handler_obj) returned null reference."
            << std::endl;
          return 1;
        }

      poaman->activate ();

      char inout_c = 'b';
      ami_test_var->sendc_foo_char (test_handler,
                                    'a',
                                    inout_c);

      wchar_t inout_wc = L'f';
      ami_test_var->sendc_foo_wchar (test_handler,
                                     L'e',
                                     inout_wc);

      uint8_t inout_octet = 13;
      ami_test_var->sendc_foo_octet (test_handler,
                          12,
                          inout_octet);

      bool inout_b = true;
      ami_test_var->sendc_foo_bool (test_handler,
                       inout_b,
                       true);

      A::enumType inout_e = A::enumType::be;
      ami_test_var->sendc_foo_enum (test_handler,
                              inout_e,
                              A::enumType::ae);
      A::structType inout_t;
      inout_t.as(20000);
      inout_t.bs(2);
      A::structType in_t;
      in_t.as(1);
      in_t.bs(10000);
      ami_test_var->sendc_foo_struct (test_handler,
                                      in_t,
                                      inout_t);
      A::longSeq in_seq;
      A::longSeq inout_seq;
      for (int i=0; i<5 ; ++i)
        {
          in_seq.push_back (i);
          inout_seq.push_back (i*10);
        }
      ami_test_var->sendc_foo_sequence (test_handler,
                                        in_seq,
                                        inout_seq) ;
      A::arrayType in_a = { { 1, 2, 3, 4, 5 } };
      A::arrayType inout_a = { { 10, 20, 30, 40, 50 } };

      ami_test_var->sendc_foo_array (test_handler,
                                     in_a,
                                     inout_a) ;
      A::unionData in_u ;
      in_u.longData(1);

      A::unionData inout_u ;
      inout_u.shortData(2);

      ami_test_var->sendc_foo_union (test_handler,
                                     in_u,
                                     inout_u) ;

      std::wstring inout_w = L"bb";
      ami_test_var->sendc_foo_wstring (test_handler,
                                 L"aa",
                                 inout_w);
      std::string inout_s = "gg";
      ami_test_var->sendc_foo_string (test_handler,
                                "ff",
                                inout_s) ;
      uint64_t in_ull = 10000;
      uint64_t inout_ull = 20000;
      ami_test_var->sendc_foo_u_l_l (test_handler,
                            in_ull,
                            inout_ull) ;

      float inout_f = 1.2f;
      ami_test_var->sendc_foo_float (test_handler,
                         1.1f,
                         inout_f) ;

      long double inout_ld = 2.2;
      ami_test_var->sendc_foo_long_double (test_handler,
                                    2.1,
                                    inout_ld);

      int32_t inout_l = 350;
      ami_test_var->sendc_foo_void_in (test_handler,200);
      ami_test_var->sendc_foo_void_out (test_handler);
      ami_test_var->sendc_foo_void_inout (test_handler,inout_l);
      ami_test_var->sendc_foo_no_arg (test_handler) ;
      ami_test_var->sendc_foo_void_no_arg (test_handler) ;

      ami_test_var->sendc_foo_char (test_handler,
                                    'x',
                                    inout_c);


      TAOX11_TEST_INFO << "Client: Do something else before coming back for the replies." << std::endl;

      for (int i = 0; i < 10; i ++)
        {
          TAOX11_TEST_INFO << " ...";
          std::this_thread::sleep_for (std::chrono::milliseconds (10));
        }

      TAOX11_TEST_INFO << std::endl << "Client: Now let's look for the replies." << std::endl;

      std::chrono::seconds sec (1);
      std::this_thread::sleep_for (std::chrono::seconds (2));
      while (!received_all_replies)
        {
          _orb->perform_work (sec);
        }

      if (callback_operation != 20)
        {
          TAOX11_TEST_ERROR << "ERROR: Client didn't receive expected callbacks."
          << " Expected -20- , received -" << callback_operation
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
