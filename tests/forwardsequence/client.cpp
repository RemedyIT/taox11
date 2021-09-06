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
      Test::iforwardSeq in_v;
      Test::iforwardSeq out_v;
      Test::iforwardSeq inout_v;
      for (int ix = 0; ix < 7; ix++)
        in_v.push_back(hello->get_forward());
      for (int ix = 0; ix < 7; ix++)
        inout_v.push_back(hello->get_forward());
      Test::iforwardSeq return_v =
          hello->inout_forwardSeq(in_v, out_v, inout_v);
      test_equal(13, in_v[3]->uid());
      test_equal(26, out_v[2]->uid());
      test_equal(33, inout_v[2]->uid());
    }

    {
      Test::forwardSeq2 in_v;
      Test::forwardSeq2 out_v;
      Test::forwardSeq2 inout_v;
      for (int ix = 0; ix < 7; ix++)
      {
        Test::iforwardSeq fs;
        for (int ix1 = 0; ix1 < 3; ix1++)
          fs.push_back(hello->get_forward());
        in_v.push_back(fs);
      }
      for (int ix = 0; ix < 6; ix++)
      {
        Test::iforwardSeq fs;
        for (int ix1 = 0; ix1 < 2; ix1++)
          fs.push_back(hello->get_forward());
        inout_v.push_back(fs);
      }
      Test::forwardSeq2 return_v = hello->inout_forwardSeq2 (in_v, out_v,
          inout_v);
      test_equal ((Test::forwardSeq2::size_type) 7, in_v.size ());
      test_equal ((Test::iforwardSeq::size_type) 3, in_v[0].size ());
      test_equal (58, in_v[6][2]->uid ());
      test_equal ((Test::forwardSeq2::size_type) 7, out_v.size ());
      test_equal ((Test::iforwardSeq::size_type) 2, out_v[0].size ());
      test_equal (84, out_v[6][1]->uid ());
      test_equal ((Test::forwardSeq2::size_type) 6, inout_v.size ());
      test_equal ((Test::iforwardSeq::size_type) 3, inout_v[0].size ());
      test_equal (102, inout_v[5][2]->uid ());
    }

    hello->createLocal();

    // End
    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown();
    _orb->destroy ();
    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    ++result_;
  }
  return result_;
}
