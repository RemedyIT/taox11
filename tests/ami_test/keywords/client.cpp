/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   Keyword test for AMI
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "client.h"

#include "testlib/taox11_testlog.h"

uint16_t result_ = 0;

uint16_t replies_to_receive_ = 10;
uint16_t received_replies_ = 0;

template<class T>
bool TestEqual(std::string name, T exp, T a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: For " << name << " expected " << exp << ", received " << a << std::endl;
    ++result_;
    return false;
  }
  return true;
}

void
Handler::inout_bool (::Test::_cxx_bool ami_return_val,
  ::Test::_cxx_bool v_out,
  ::Test::_cxx_bool v_inout)
{
  ++received_replies_;
  TestEqual("hello->inout_bool v_out", Test::_cxx_bool::_cxx_long, v_out);
  TestEqual("hello->inout_bool v_inout", Test::_cxx_bool::someenum, v_inout);
  TestEqual("hello->inout_bool v_result", Test::_cxx_bool::_cxx_asm, ami_return_val);
}

void
Handler::inout_bool_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'inout_bool_excep'" << std::endl;
}

void
Handler::_cxx_uint32_t (int16_t ami_return_val)
{
  ++received_replies_;
  if (ami_return_val != 11)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for '_cxx_uint32_t' received : "
        << "expected <11> - received <" << ami_return_val << ">" << std::endl;
    }
}

void
Handler::uint32_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of '_cxx_uint32_t_excep'" << std::endl;
}

void
Handler::inout_int16_t (::Test::_cxx_int16_t ami_return_val,
  ::Test::_cxx_int16_t v_out,
  ::Test::_cxx_int16_t v_inout)
{
  ++received_replies_;
  TestEqual("hello->inout_int16_t v_out", Test::_cxx_int16_t::char_, v_out);
  TestEqual("hello->inout_int16_t v_inout", Test::_cxx_int16_t::long_, v_inout);
  TestEqual("hello->inout_int16_t v_result", Test::_cxx_int16_t::float_, ami_return_val);
}

void
Handler::inout_int16_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'inout_int16_t_excep'" << std::endl;
}

void
Handler::get_attr_int32_t (
  ::Test::_cxx_int32_t ami_return_val)
{
  ++received_replies_;
  if (ami_return_val != 1234)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'attr_int32_t' received : "
        << "expected <1234> - received <" << ami_return_val << ">" << std::endl;
    }
}

void
Handler::get_attr_int32_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_int32_t_excep'" << std::endl;
}

void
Handler::set_attr_int32_t ()
{
  ++received_replies_;
}

void
Handler::set_attr_int32_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_int32_t_excep'" << std::endl;
}

void
Handler::get_attr_bool (
  ::Test::_cxx_bool ami_return_val)
{
  ++received_replies_;
  if (ami_return_val != Test::_cxx_bool::_cxx_char)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'attr_bool' received : "
        << "expected <" << Test::_cxx_bool::_cxx_char << "> - received <"
        << ami_return_val << ">" << std::endl;
    }
}

void
Handler::get_attr_bool_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_bool_excep'" << std::endl;
}

void
Handler::set_attr_bool ()
{
  ++received_replies_;
}

void
Handler::set_attr_bool_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_bool_excep'" << std::endl;
}

void
Handler::get_attr_int16_t (
  ::Test::_cxx_int16_t ami_return_val)
{
  ++received_replies_;
  if (ami_return_val != Test::_cxx_int16_t::float_)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'attr_int16_t' received : "
        << "expected <" << Test::_cxx_int16_t::float_ << "> - received <"
        << ami_return_val << ">" << std::endl;
    }

}

void
Handler::get_attr_int16_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_int16_t_excep'" << std::endl;
}

void
Handler::set_attr_int16_t ()
{
  ++received_replies_;
}

void
Handler::set_attr_int16_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_int16_t_excep'" << std::endl;
}

void
Handler::get_attr_least8_t (
  ::Test::_cxx_int_least8_t ami_return_val)
{
  ++received_replies_;

  if (ami_return_val != Test::_cxx_int_least8_t::floa8t_)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'attr_least8_t' received : "
        << "expected <" << Test::_cxx_int_least8_t::floa8t_ << "> - received <"
        << ami_return_val << ">" << std::endl;
    }
}

void
Handler::get_attr_least8_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_least8_t_excep'" << std::endl;
}

void
Handler::set_attr_least8_t ()
{
  ++received_replies_;
}

void
Handler::set_attr_least8_t_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_least8_t_excep'" << std::endl;
}

void
Handler::get_attr_cpplib_keyword_struct (
  const ::Test::cpplib_keyword_struct& ami_return_val)
{
  ++received_replies_;
  if (ami_return_val._cxx_wchar_t () != L'^' || ami_return_val.std () != 54321 ||
    ami_return_val.printf () != 54322 || ami_return_val.cout () != 54323 ||
    ami_return_val.CORBA () != 54324 || ami_return_val.define () != 54325 ||
    ami_return_val.include () != 54326)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'cpplib_keyword_struct' received : "
        << "received <" << ami_return_val << ">" << std::endl;
    }
}

void
Handler::get_attr_cpplib_keyword_struct_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_cpplib_keyword_struct_excep'" << std::endl;
}

void
Handler::set_attr_cpplib_keyword_struct ()
{
  ++received_replies_;
}

void
Handler::set_attr_cpplib_keyword_struct_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_cpplib_keyword_struct_excep'" << std::endl;
}

void
Handler::get_attr_int_keyword_struct (
  const ::Test::int_keyword_struct& ami_return_val)
{
  ++received_replies_;
  const Test::int_keyword_struct val_int_keyword_struct (12345, 12346);
  if (ami_return_val._cxx_int () != 12345 || ami_return_val.int_ () != 12346)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'attr_int_keyword_struct' received : "
        << "received <" << ami_return_val << ">" << std::endl;
    }
}

void
Handler::get_attr_int_keyword_struct_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_int_keyword_struct_excep'" << std::endl;
}

void
Handler::set_attr_int_keyword_struct ()
{
  ++received_replies_;
}

void
Handler::set_attr_int_keyword_struct_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_int_keyword_struct_excep'" << std::endl;
}

void
Handler::get_attr_cpp_keyword_struct (
  const ::Test::cpp_keyword_struct& ami_return_val)
{
  ++received_replies_;
  if (ami_return_val._cxx_alignas()!= 123456 || ami_return_val._cxx_alignof()!= 123457 ||
    ami_return_val._cxx_and()!= 123458 || ami_return_val._cxx_and_eq()!=  123459 ||
    ami_return_val._cxx_asm()!= 123460 || ami_return_val._cxx_auto()!=  123461 ||
    ami_return_val._cxx_bitand()!=  123462 || ami_return_val._cxx_bitor()!= 123463 ||
    ami_return_val._cxx_bool()!=  123464 || ami_return_val._cxx_break()!= 123465 ||
    ami_return_val._cxx_catch()!= 123466 || ami_return_val._cxx_char16_t()!=  123467 ||
    ami_return_val._cxx_char32_t()!=  123468 || ami_return_val._cxx_class()!= 123469 ||
    ami_return_val._cxx_compl()!= 123470 || ami_return_val._cxx_constexpr()!= 123471 ||
    ami_return_val._cxx_const_cast()!=  123472 || ami_return_val._cxx_continue()!=  123473 ||
    ami_return_val._cxx_decltype()!=  123474 || ami_return_val._cxx_delete()!=  123475 ||
    ami_return_val._cxx_do()!=  123476 || ami_return_val._cxx_dynamic_cast()!=  123477 ||
    ami_return_val._cxx_else()!=  123478 || ami_return_val._cxx_explicit()!=  123479 ||
    ami_return_val._cxx_export()!=  123480 || ami_return_val._cxx_extern()!=  123481 ||
    ami_return_val._cxx_for()!= 123482 || ami_return_val._cxx_friend()!=  123483 ||
    ami_return_val._cxx_goto()!=  123484 || ami_return_val._cxx_if()!=  123485 ||
    ami_return_val._cxx_inline()!=  123486 || ami_return_val._cxx_int()!= 123487 ||
    ami_return_val._cxx_mutable()!= 123488 || ami_return_val._cxx_namespace()!= 123489 ||
    ami_return_val._cxx_new()!= 123490 || ami_return_val._cxx_not()!= 123491 ||
    ami_return_val._cxx_not_eq()!=  123492 || ami_return_val._cxx_noexcept()!=  123493 ||
    ami_return_val._cxx_nullptr()!= 123494 || ami_return_val._cxx_operator()!=  123495 ||
    ami_return_val._cxx_or()!=  123496 || ami_return_val._cxx_or_eq()!= 123497 ||
    ami_return_val._cxx_protected()!= 123498 || ami_return_val._cxx_register()!=  123499 ||
    ami_return_val._cxx_reinterpret_cast()!=  123500 || ami_return_val._cxx_return()!=  123501 ||
    ami_return_val._cxx_signed()!=  123502 || ami_return_val._cxx_sizeof()!=  123503 ||
    ami_return_val._cxx_static()!=  123504 || ami_return_val._cxx_static_assert()!= 123505 ||
    ami_return_val._cxx_static_cast()!= 123506 || ami_return_val._cxx_template()!=  123507 ||
    ami_return_val._cxx_this()!=  123508 || ami_return_val._cxx_thread_local()!=  123509 ||
    ami_return_val._cxx_throw()!= 123510 || ami_return_val._cxx_try()!= 123511 ||
    ami_return_val._cxx_using()!= 123512 || ami_return_val._cxx_virtual()!= 123513 ||
    ami_return_val._cxx_volatile()!=  123514 || ami_return_val._cxx_wchar_t()!= 123515 ||
    ami_return_val._cxx_while()!= 123516 || ami_return_val._cxx_xor()!= 123517 ||
    ami_return_val._cxx_xor_eq()!=  123518 || ami_return_val._cxx_uint8_t()!=  123519 ||
    ami_return_val._cxx_int16_t()!= 123520 || ami_return_val._cxx_uint16_t()!=  123521 ||
    ami_return_val._cxx_int32_t()!= 123522 || ami_return_val._cxx_uint32_t()!=  123523 ||
    ami_return_val._cxx_int64_t()!= 123524 || ami_return_val._cxx_uint64_t()!=  123525 ||
    ami_return_val._cxx_int_fast8_t()!= 123526 || ami_return_val._cxx_int_fast16_t()!= 123527 ||
    ami_return_val._cxx_int_fast32_t()!= 123528 || ami_return_val._cxx_int_fast64_t()!= 123529 ||
    ami_return_val._cxx_int_least8_t()!=  123530 || ami_return_val._cxx_int_least16_t()!=  123531 ||
    ami_return_val._cxx_int_least32_t()!=  123532 || ami_return_val._cxx_int_least64_t()!=  123533 ||
    ami_return_val._cxx_intmax_t()!= 123534 || ami_return_val._cxx_intptr_t()!= 123535 ||
    ami_return_val._cxx_uint_fast8_t()!=  123536 || ami_return_val._cxx_uint_fast16_t()!=  123537 ||
    ami_return_val._cxx_uint_fast32_t()!=  123538 || ami_return_val._cxx_uint_fast64_t()!=  123539 ||
    ami_return_val._cxx_uint_least8_t()!= 123540 || ami_return_val._cxx_uint_least16_t()!= 123541 ||
    ami_return_val._cxx_uint_least32_t()!= 123542 || ami_return_val._cxx_uint_least64_t()!= 123543 ||
    ami_return_val._cxx_uintmax_t()!=  123544 || ami_return_val._cxx_uintptr_t()!=  123545)
    {
      ++result_;
      TAOX11_TEST_ERROR << "ERROR : Unexpected value for 'attr_cpp_keyword_struct' received : "
        << "received <" << ami_return_val << ">" << std::endl;
    }
}

void
Handler::get_attr_cpp_keyword_struct_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_attr_cpp_keyword_struct_excep'" << std::endl;
}

void
Handler::set_attr_cpp_keyword_struct ()
{
  ++received_replies_;
}

void
Handler::set_attr_cpp_keyword_struct_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_attr_cpp_keyword_struct_excep'" << std::endl;
}

void
Handler::bar ()
{
}

void
Handler::bar_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
}

void
Handler::shutdown ()
{
}

void
Handler::shutdown_excep (
  IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
}

void
Handler::_cxx_class ()
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of '_cxx_class'" << std::endl;
}

void
Handler::class_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'class_excep'" << std::endl;
}

void
Handler::_cxx_void (
    const std::string&,
    const std::string&)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of '_cxx_void'" << std::endl;
}

void
Handler::void_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'void_excep'" << std::endl;
}

void
Handler::get_private (::Test::_cxx_bool)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_private'" << std::endl;
}

void
Handler::get_private_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'get_private_excep'" << std::endl;
}

void
Handler::set_private ()
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_private'" << std::endl;
}

void
Handler::set_private_excep (IDL::traits<::Messaging::ExceptionHolder>::ref_type)
{
  ++result_;
  TAOX11_TEST_ERROR << "ERROR : Unexpected invocation of 'set_private_excep'" << std::endl;
}

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);

      if (orb == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object("file://test.ior");

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
            << std::endl;
          return 1;
        }

      CORBA::amic_traits<Test::Hello>::ref_type hello =
        CORBA::amic_traits<Test::Hello>::narrow (obj);

      if (!hello)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type poa_obj =
          orb->resolve_initial_references ("RootPOA");

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

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
        root_poa->the_POAManager ();

      if (!poaman)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->the_POAManager () returned null object."
            << std::endl;
          return 1;
        }

      CORBA::amic_traits<Test::Hello>::replyhandler_servant_ref_type test_handler_impl =
        CORBA::make_reference<Handler> ();

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

      CORBA::amic_traits<Test::Hello>::replyhandler_ref_type test_handler =
        CORBA::amic_traits<Test::Hello>::replyhandler_traits::narrow (test_handler_obj);

      if (test_handler == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow (test_handler_obj) returned null reference."
            << std::endl;
          return 1;
        }

      poaman->activate ();

      TAOX11_TEST_DEBUG << "Invoke methods asynchronously." << std::endl;

      Test::_cxx_bool bool_inout;
      hello->sendc_inout_bool(test_handler, Test::_cxx_bool::_cxx_char,  bool_inout);
      Test::_cxx_int16_t int16_t_inout;
      hello->sendc_inout_int16_t(test_handler, Test::_cxx_int16_t::double_, int16_t_inout);
      hello->sendc_uint32_t (test_handler, 10);

      TAOX11_TEST_DEBUG << "Setting the attributes asynchronously." << std::endl;
      hello->sendc_set_attr_bool (test_handler, Test::_cxx_bool::_cxx_char);
      hello->sendc_set_attr_int16_t (test_handler, Test::_cxx_int16_t::float_);
      hello->sendc_set_attr_int32_t (test_handler, 1234);
      hello->sendc_set_attr_least8_t (test_handler, Test::_cxx_int_least8_t::floa8t_);
      const Test::cpplib_keyword_struct val_cpplib_keyword_struct (L'^', 54321, 54322,
        54323, 54324, 54325, 54326);
      hello->sendc_set_attr_cpplib_keyword_struct (test_handler, val_cpplib_keyword_struct);
      const Test::int_keyword_struct val_int_keyword_struct (12345, 12346);
      hello->sendc_set_attr_int_keyword_struct (test_handler, val_int_keyword_struct);
      const Test::cpp_keyword_struct val_cpp_keyword_struct (123456, 123457, 123458,
        123459, 123460, 123461, 123462, 123463, 123464, 123465, 123466, 123467, 123468,
        123469, 123470, 123471, 123472, 123473, 123474, 123475, 123476, 123477, 123478,
        123479, 123480, 123481, 123482, 123483, 123484, 123485, 123486, 123487, 123488,
        123489, 123490, 123491, 123492, 123493, 123494, 123495, 123496, 123497, 123498,
        123499, 123500, 123501, 123502, 123503, 123504, 123505, 123506, 123507, 123508,
        123509, 123510, 123511, 123512, 123513, 123514, 123515, 123516, 123517, 123518,
        123519, 123520, 123521, 123522, 123523, 123524, 123525, 123526, 123527, 123528,
        123529, 123530, 123531, 123532, 123533, 123534, 123535, 123536, 123537, 123538,
        123539, 123540, 123541, 123542, 123543, 123544, 123545);
      hello->sendc_set_attr_cpp_keyword_struct (test_handler, val_cpp_keyword_struct);

      // Wait until all methods/setters have returned before getting the values of the
      // attributes.
      while (received_replies_ != replies_to_receive_)
        {
          std::chrono::seconds tv (1);
          orb->perform_work (tv);
        }

      // reset for the next test.
      replies_to_receive_ = 7;
      received_replies_ = 0;

      TAOX11_TEST_DEBUG << "Getting the attributes asynchronously." << std::endl;
      hello->sendc_get_attr_bool (test_handler);
      hello->sendc_get_attr_cpp_keyword_struct (test_handler);
      hello->sendc_get_attr_cpplib_keyword_struct (test_handler);
      hello->sendc_get_attr_int16_t (test_handler);
      hello->sendc_get_attr_int32_t (test_handler);
      hello->sendc_get_attr_int_keyword_struct (test_handler);
      hello->sendc_get_attr_least8_t (test_handler);

      hello->sendc_bar (nullptr);

      while (received_replies_ != replies_to_receive_)
        {
          std::chrono::seconds tv (1);
          orb->perform_work (tv);
        }

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      ++result_;
    }
  return result_;
}
