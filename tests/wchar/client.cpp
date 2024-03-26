/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"

int main(int argc, char* argv[])
{
  int result = 0;
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

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    // Test
    {
      TAOX11_TEST_DEBUG << "Test wchar get and set." << std::endl;
      wchar_t const text = hello->getset_wchar (L'H');
      if (text != L'H')
      {
        TAOX11_TEST_ERROR_W << L"ERROR: hello->get_string() returned an unexpected value. "
          << L"expected <H>, received <" << text << L">" << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG_W << L"ostream test wchar: " << text << std::endl;
    }

    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown();
    TAOX11_TEST_DEBUG << std::endl;

    orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    ++result;
  }

  return result;
}
