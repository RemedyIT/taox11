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

int result_ = 0;

template<class T, class U>
bool test_equal(T exp, U a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: Expected " << exp <<
                          ", received " << a << std::endl;
    ++result_;
    return false;
  }
  return true;
}

int main(int argc, char* argv[])
{
  try
  {
    TAOX11_TEST_LOGGER::verbosity_mask (0);

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

    // Test
    {
      TAOX11_TEST_DEBUG << std::endl << std::endl << "Test inout_forwardArray";
      Test::iforwardArray in_v;
      Test::iforwardArray out_v;
      Test::iforwardArray inout_v;
      TAOX11_TEST_DEBUG << "\n in    ";
      x11_logger::debug ().flush();
      for (IDL::traits<Test::iforward>::ref_type &i : in_v)
      {
        i = hello->get_forward();
      };
      TAOX11_TEST_DEBUG << std::endl << " inout ";
      x11_logger::debug ().flush();
      for (IDL::traits<Test::iforward>::ref_type &i : inout_v)
      {
        i = hello->get_forward();
      };
      Test::iforwardArray return_v = hello->inout_forwardArray(in_v, out_v,
          inout_v);
      test_equal(12, in_v[2]->uid());
      test_equal(22, out_v[2]->uid());
      test_equal(27, inout_v[2]->uid());
      test_equal(12, return_v[2]->uid());
    }

    {
      TAOX11_TEST_DEBUG << std::endl << std::endl << "Test inout_forwardArray2";
      Test::iforwardArray2 in_v;
      Test::iforwardArray2 out_v;
      Test::iforwardArray2 inout_v;
      TAOX11_TEST_DEBUG << "\n in    ";
      x11_logger::debug ().flush();
      for (std::array<IDL::traits<Test::iforward>::ref_type, 5> &i_array : in_v)
      {
        for (IDL::traits<Test::iforward>::ref_type &f : i_array)
        {
          f = hello->get_forward();
        };
      };
      TAOX11_TEST_DEBUG << "\n inout ";
      x11_logger::debug ().flush();
      for (std::array<IDL::traits<Test::iforward>::ref_type, 5> &i_array : inout_v)
      {
        for (IDL::traits<Test::iforward>::ref_type &f : i_array)
        {
          f = hello->get_forward();
        };
      };
      test_equal(37, in_v[1][2]->uid());
      Test::iforwardArray2 return_v = hello->inout_forwardArray2(in_v, out_v,
          inout_v);
      test_equal(37, out_v[1][2]->uid());
      test_equal(37, inout_v[1][2]->uid());
      test_equal(52, return_v[1][2]->uid());
    }

    {
      TAOX11_TEST_DEBUG << std::endl << std::endl << "Test inout_forwardArray3";
      Test::iforwardArray3 in_v3;
      Test::iforwardArray3 out_v3;
      Test::iforwardArray3 inout_v3;
      TAOX11_TEST_DEBUG << "\n in    ";
      x11_logger::debug ().flush();
      for (std::array<std::array<IDL::traits<Test::iforward>::ref_type, 5>, 6> &i_array3 : in_v3)
      {
        for (std::array<IDL::traits<Test::iforward>::ref_type, 5> &i_array : i_array3)
        {
          for (IDL::traits<Test::iforward>::ref_type &f : i_array)
          {
            f = hello->get_forward();
          };
        };
      };
      TAOX11_TEST_DEBUG << "\n inout ";
      x11_logger::debug ().flush();
      for (std::array<std::array<IDL::traits<Test::iforward>::ref_type, 5>, 6> &i_array3 : inout_v3)
      {
        for (std::array<IDL::traits<Test::iforward>::ref_type, 5> &i_array : i_array3)
        {
          for (IDL::traits<Test::iforward>::ref_type &f : i_array)
          {
            f = hello->get_forward();
          };
        };
      };
      test_equal(150, in_v3[3][0][0]->uid());
      Test::iforwardArray3 return_v = hello->inout_forwardArray3(in_v3, out_v3,
          inout_v3);
      test_equal(60, out_v3[0][0][0]->uid());
      test_equal(269, out_v3.at(6).at(5).at(4)->uid());
      test_equal(60, inout_v3[0][0][0]->uid());
      test_equal(269, inout_v3.at(6).at(5).at(4)->uid());
      test_equal(270, return_v[0][0][0]->uid());
      test_equal(479, return_v.at(6).at(5).at(4)->uid());
    }
    TAOX11_TEST_DEBUG << std::endl;
    // End

    TAOX11_TEST_DEBUG << "shutting down...";

    hello->shutdown();
    _orb->destroy ();
    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    return 1;
  }
  return result_;
}
