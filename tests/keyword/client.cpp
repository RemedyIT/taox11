/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <limits>

int result_ = 0;

template<class T>
bool TestEqual(const std::string& name, T exp, T a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: For " << name << " expected " << exp << ", received " << a << std::endl;
    ++result_;
    return false;
  }
  return true;
}

int main(int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
        << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object("file://test.ior");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed interface" << std::endl;


    {
      TAOX11_TEST_DEBUG << "Test generated code for enum Test::_cxx_bool" << std::endl;
      Test::_cxx_bool v_out;
      Test::_cxx_bool v_inout;
      Test::_cxx_bool v_result = hello->inout_bool(Test::_cxx_bool::_cxx_char,
          v_out, v_inout);
      TestEqual("hello->inout_bool v_out", Test::_cxx_bool::_cxx_long, v_out);
      TestEqual("hello->inout_bool v_inout", Test::_cxx_bool::someenum, v_inout);
      TestEqual("hello->inout_bool v_result", Test::_cxx_bool::_cxx_asm,
          v_result);
    }

    {
      TAOX11_TEST_DEBUG << "Test generated code for enum Test::int16_t" << std::endl;
      Test::_cxx_int16_t v_out;
      Test::_cxx_int16_t v_inout;
      Test::_cxx_int16_t v_result = hello->inout_int16_t(Test::_cxx_int16_t::double_,
          v_out, v_inout);
      TestEqual("hello->inout_int16_t v_out", Test::_cxx_int16_t::char_, v_out);
      TestEqual("hello->inout_int16_t v_inout", Test::_cxx_int16_t::long_, v_inout);
      TestEqual("hello->inout_int16_t v_result", Test::_cxx_int16_t::float_,
          v_result);
    }

    {
      TAOX11_TEST_DEBUG << "Test generated code for method _cxx_uint32_t" << std::endl;
      int16_t result = hello->_cxx_uint32_t (10);
      int16_t expected = 11;
      TestEqual("hello->_cxx_uint32_t", expected, result);
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down...";

    hello->shutdown();
    _orb->destroy ();
    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result_;
  }
  return result_;
}
