/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object ("file://test.ior");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed Foo interface" << std::endl;

      Test::F f = { { 1, 2, 3, 4, 5} };
      Test::G g = { { 5, 4, 3, 2, 1} };
      Test::V v = { { "A", "B", "C", "D", "E" } };

      Test::M m = { {
          { {
            { {"nulnulnul","nulnuleen", "nulnultwee"} },
            { {"nuleennul","nuleeneen", "nuleentwee"} }
          } }
       } };

      TAOX11_TEST_DEBUG << "write multidim array Test::M m = " << IDL::traits<Test::M>::write (m) << std::endl;

      TAOX11_TEST_DEBUG << "ostreamtest array f: " << f <<  std::endl;
      TAOX11_TEST_DEBUG << "ostreamtest array g: " << g <<  std::endl;
      TAOX11_TEST_DEBUG << "ostreamtest array v: " << v <<  std::endl;
      TAOX11_TEST_DEBUG << "ostreamtest array m: " << m <<  std::endl;

      TAOX11_TEST_DEBUG << "Testing value initialization" << std::endl;

      // Check that array members as part of a structured type are all value initialized
      uint16_t default_uint16_t {};
      Test::FooStruct foostruct;

      if (foostruct.my_a() != default_uint16_t) {
        TAOX11_TEST_ERROR << "Struct member my_a not value initialized, " << foostruct.my_a() << " instead of " << default_uint16_t << std::endl;
        return 1;
      }

      for (const auto& foo_member : foostruct.my_f()) {
        if (foo_member != default_uint16_t) {
          TAOX11_TEST_ERROR << "Array member my_f not value initialized, " << foo_member << " instead of " << default_uint16_t << std::endl;
          return 1;
        }
      }

      Test::ArrayStruct as;
      int32_t default_int32_t {};
      for (const auto& aaal_member : as.aaal()) {
        for (const auto& aal_member : aaal_member) {
          for (const auto& al_member : aal_member) {
            if (al_member != default_int32_t) {
              TAOX11_TEST_ERROR << "Array member of aaal not value initialized, " << al_member << " instead of " << default_int32_t << std::endl;
              return 1;
            }
          }
        }
      }

      for (const auto& aal_member : as.aal()) {
        for (const auto& al_member : aal_member) {
          if (al_member != default_int32_t) {
            TAOX11_TEST_ERROR << "Array member of aal not value initialized, " << al_member << " instead of " << default_int32_t << std::endl;
            return 1;
          }
        }
      }

      Test:: enumType default_enumType {};
      for (const auto& aae_member : as.aae()) {
        for (const auto& ae_member : aae_member) {
          if (ae_member != default_enumType) {
            TAOX11_TEST_ERROR << "Array member of aae not value initialized, " << ae_member << " instead of " << default_enumType << std::endl;
            return 1;
          }
        }
      }

       for (const auto& tdtdaol_member1 : as.tdtdaol()) {
        for (const auto& tdtdaol_member2 : tdtdaol_member1) {
          for (const auto& tdtdaol_member3 : tdtdaol_member2) {
            for (const auto& tdtdaol_member4 : tdtdaol_member3) {
              if (tdtdaol_member4 != default_int32_t) {
                TAOX11_TEST_ERROR << "Array member of aal not value initialized, " << tdtdaol_member4 << " instead of " << default_int32_t << std::endl;
                return 1;
              }
            }
          }
        }
      }

      if (f.size () != 5 || f.max_size () != 5)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::F has wrong size." << std::endl;
        return 1;
      }

      if (!foo->pass_array (f, v, m))
      {
        TAOX11_TEST_ERROR << "ERROR: Test::pass_array call failed." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "successfully called Foo::pass_array ()" << std::endl;

      // Not a copy here but a move!
      Test::F f2 = foo->return_array();

      if (f2 != f)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::return_array call returned different array." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "successfully called Foo::return_array()" << std::endl;

      TAOX11_TEST_DEBUG << "checking array dimensions ..." << std::endl;

      if (IDL::traits<Test::F>::dimensions () == 1)
        {
          TAOX11_TEST_DEBUG << "OK, dimensions for Test::F are correct." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR, incorrect dimensions for Test::F found. "
            << "expected<1> - found <" << IDL::traits<Test::F>::dimensions ()
            << ">" << std::endl;
        }
      if (IDL::traits<Test::V>::dimensions () == 1)
        {
          TAOX11_TEST_DEBUG << "OK, dimensions for Test::V are correct." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR, incorrect dimensions for Test::V found. "
            << "expected<1> - found <" << IDL::traits<Test::V>::dimensions ()
            << ">" << std::endl;
        }
      if (IDL::traits<Test::M>::dimensions () == 3)
        {
          TAOX11_TEST_DEBUG << "OK, dimensions for Test::M are correct." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR, incorrect dimensions for Test::M found. "
            << "expected<3> - found <" << IDL::traits<Test::M>::dimensions ()
            << ">" << std::endl;
        }
      if (IDL::traits<Test::o>::dimensions () == 1)
        {
          TAOX11_TEST_DEBUG << "OK, dimensions for Test::o are correct." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR, incorrect dimensions for Test::o found. "
            << "expected<1> - found <" << IDL::traits<Test::o>::dimensions ()
            << ">" << std::endl;
        }
      if (IDL::traits<Test::a>::dimensions () == 1)
        {
          TAOX11_TEST_DEBUG << "OK, dimensions for Test::a are correct." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR, incorrect dimensions for Test::a found. "
            << "expected<1> - found <" << IDL::traits<Test::a>::dimensions ()
            << ">" << std::endl;
        }

      TAOX11_TEST_DEBUG << "shutting down...";

      foo->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }
  return 0;
}
