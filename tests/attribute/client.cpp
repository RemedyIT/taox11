/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"

int result_ = 0;

template<class T>
void TestEqual(std::string name, T exp, T a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: For " << name << " expected " << exp << " , received "
        << a << std::endl;
    ++result_;
  }
}

void
TestEqual(const std::string name, std::wstring exp, std::wstring a) {
  if (a != exp) {
    TAOX11_TEST_ERROR << "ERROR: value for " << name << " expected ";
    TAOX11_TEST_DEBUG_W << exp << L", received " << a << std::endl;
    ++result_;
  }
}

template<>
void TestEqual(std::string name, Test::unionType exp, Test::unionType a)
{
  if (exp._d() != a._d())
  {
    TAOX11_TEST_ERROR << "Different descriminator , expected " << exp._d() << " found "
        << a._d() << std::endl;
    ++result_;
  }
  switch (exp._d())
  {
    case 'L':
      if (a.longData() != exp.longData())
      {
        TAOX11_TEST_ERROR << "For " << name << ".longData expected " << exp.longData()
            << " found " << a.longData() << std::endl;
        ++result_;
      }
      break;
    case 'Z':
      if (a.wchar_tData() != exp.wchar_tData())
      {
        TAOX11_TEST_ERROR << "For " << name << ".wchar_tData expected "
            << exp.wchar_tData() << " found " << a.wchar_tData() << std::endl;
        ++result_;
      }
      break;
    case 'H':
      if (a.boolData() != exp.boolData())
      {
        TAOX11_TEST_ERROR << "For " << name << ".boolData expected " << exp.boolData()
            << " found " << a.boolData() << std::endl;
        ++result_;
      }
      break;
    case 'W':
      if (a.doubleData() != exp.doubleData())
      {
        TAOX11_TEST_ERROR << "For " << name << ".doubleData expected "
            << exp.doubleData() << " found " << a.doubleData() << std::endl;
        ++result_;
      }
      break;
    case 's':
      if (a.stringData() != exp.stringData())
      {
        TAOX11_TEST_ERROR << "For " << name << ".stringData expected "
            << exp.stringData() << " found " << a.stringData() << std::endl;
        ++result_;
      }
      break;
    case 'w':
      if (a.wstringData() != exp.wstringData())
      {
        TAOX11_TEST_ERROR << "For " << name << ".wstringData expected ";
        TAOX11_TEST_ERROR_W << exp.wstringData() << L" found " << a.wstringData()
            << std::endl;
        ++result_;
      }
      break;
  }
}

// Test if vector content is equal.
template<typename T>
void eqVector(const std::vector<T>& v1, std::vector<T> v2)
{
  typename std::vector<T>::iterator first2 = v2.begin();
for (const T &el : v1)
{
  if (el != *first2)
  {
    TAOX11_TEST_ERROR << "ERROR: Sequence element "<< el << " <> " << *first2 << "  ";
    return;
  }
  ++first2;
}
}

// Test if array content is equal.
template<typename T, size_t N>
bool eqArray(std::array<T, N> v1, std::array<T, N> v2)
{
  typename std::array<T, N>::iterator first2 = v2.begin();
  for (const T &el : v1)
  {
    if (el != *first2)
    {
      TAOX11_TEST_DEBUG << "Error: array "<< el << "<>" << *first2 << " ";
      return false;
    }
    ++first2;
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

    // char attribute
    {
      TAOX11_TEST_DEBUG << "Test IDL char attribute." << std::endl;
      char lv_char = hello->at_char_ro();
      TestEqual("alv_char", lv_char, 'a');
      hello->at_char('b');
      lv_char = hello->at_char();
      TestEqual("alv_char", lv_char, 'b');
    }

    // wchar_t attribute
    {
      const wchar_t lc_wchar_t = L'b';
      TAOX11_TEST_DEBUG << "Test IDL wchar / wchar_t attribute." << std::endl;
      hello->at_wchar_t(lc_wchar_t);
      wchar_t lv_wchar_t = hello->at_wchar_t();
      TestEqual("lv_wchar_t", lv_wchar_t, lc_wchar_t);
    }

    // bool attribute
    {
      const bool lc_bool = true;
      TAOX11_TEST_DEBUG << "Test IDL boolean / bool attribute." << std::endl;
      hello->at_bool(lc_bool);
      bool lv_bool = hello->at_bool();
      TestEqual("lv_bool", lv_bool, lc_bool);
    }

    // octet / uint8_t attribute
    {
      const uint8_t lc_uint8_t = 246;
      TAOX11_TEST_DEBUG << "Test IDL octet / uint8_t attribute." << std::endl;
      hello->at_uint8_t(lc_uint8_t);
      uint8_t lv_uint8_t = hello->at_uint8_t();
      TestEqual("lv_uint8_t", lv_uint8_t, lc_uint8_t);
    }

    // string attribute
    {
      const std::string lc_string = "abc";
      TAOX11_TEST_DEBUG << "Test IDL string attribute." << std::endl;
      hello->at_string(lc_string);
      std::string lv_string = hello->at_string();
      TestEqual("lv_string", lv_string, lc_string);
    }

    // wstring attribute
    {
      const std::wstring lc_wstring = L"ABC";
      TAOX11_TEST_DEBUG << "Test IDL wstring attribute." << std::endl;
      hello->at_wstring(lc_wstring);
      std::wstring lv_wstring = hello->at_wstring();
      TestEqual("lv_wstring", lv_wstring, lc_wstring);
    }

    // int16_t attribute
    {
      const int16_t lc_int16_t = -246;
      TAOX11_TEST_DEBUG << "Test IDL short / int16_t attribute." << std::endl;
      hello->at_int16_t(lc_int16_t);
      int16_t lv_int16_t = hello->at_int16_t();
      TestEqual("lv_int16_t", lv_int16_t, lc_int16_t);
    }

    // uint16_t attribute
    {
      const uint16_t lc_uint16_t = 246;
      TAOX11_TEST_DEBUG << "Test IDL unsigned short / uint16_t attribute." << std::endl;
      hello->at_uint16_t(lc_uint16_t);
      uint16_t lv_uint16_t = hello->at_uint16_t();
      TestEqual("lv_uint16_t", lv_uint16_t, lc_uint16_t);
    }

    // int32_t attribute
    {
      const int32_t lc_int32_t = -246;
      TAOX11_TEST_DEBUG << "Test IDL long / int32_t attribute." << std::endl;
      hello->at_int32_t(lc_int32_t);
      int32_t lv_int32_t = hello->at_int32_t();
      TestEqual("lv_int32_t", lv_int32_t, lc_int32_t);
    }

    // uint32_t attribute
    {
      const uint32_t lc_uint32_t = 246;
      TAOX11_TEST_DEBUG << "Test IDL unsigned long / uint32_t attribute." << std::endl;
      hello->at_uint32_t(lc_uint32_t);
      uint32_t lv_uint32_t = hello->at_uint32_t();
      TestEqual("lv_uint32_t", lv_uint32_t, lc_uint32_t);
    }

    // int64_t attribute
    {
      const int64_t lc_int64_t = -246;
      TAOX11_TEST_DEBUG << "Test IDL long long / int64_t attribute." << std::endl;
      hello->at_int64_t(lc_int64_t);
      int64_t lv_int64_t = hello->at_int64_t();
      TestEqual("lv_int64_t", lv_int64_t, lc_int64_t);
    }

    // uint64_t attribute
    {
      const uint64_t lc_uint64_t = 246;
      TAOX11_TEST_DEBUG << "Test IDL unsigned long long / uint64_t attribute."
          << std::endl;
      hello->at_uint64_t(lc_uint64_t);
      uint64_t lv_uint64_t = hello->at_uint64_t();
      TestEqual("lv_uint64_t", lv_uint64_t, lc_uint64_t);
    }

    // float attribute
    {
      const float lc_float = 123.45F;
      TAOX11_TEST_DEBUG << "Test IDL float attribute." << std::endl;
      hello->at_float(lc_float);
      float lv_float = hello->at_float();
      TestEqual("lv_float", lv_float, lc_float);
    }

    // double attribute
    {
      const double lc_double = 456.78;
      TAOX11_TEST_DEBUG << "Test IDL double attribute." << std::endl;
      hello->at_double(lc_double);
      double lv_double = hello->at_double();
      TestEqual("lv_double", lv_double, lc_double);
    }

    // long double attribute
    {
      const long double lc_ldouble = 246;
      TAOX11_TEST_DEBUG << "Test IDL long double attribute." << std::endl;
      hello->at_ldouble(lc_ldouble);
      long double lv_ldouble = hello->at_ldouble();
      TestEqual("lv_ldouble", lv_ldouble, lc_ldouble);
    }

    // enum attribute
    {
      const Test::enumType lc_enum = Test::enumType::a;
      TAOX11_TEST_DEBUG << "Test IDL enum attribute." << std::endl;
      hello->at_enum(lc_enum);
      Test::enumType lv_enum = hello->at_enum();
      TestEqual("lv_enum", (int) lv_enum, (int) lc_enum);
    }

    // array attribute
    {
      const Test::arrayType lc_array = { { 1.9F, 2.8F, 3.7F, 4.6F, 5.5F } };
      TAOX11_TEST_DEBUG << "Test IDL array attribute." << std::endl;
      hello->at_array(lc_array);
      Test::arrayType lv_array = hello->at_array();
      eqArray(lv_array, lc_array);
    }

    // sequence attribute
    {
      const Test::sequenceType lc_sequence = { 1.9F, 2.8F, 3.7F, 4.6F, 5.5F };
      TAOX11_TEST_DEBUG << "Test IDL sequence attribute." << std::endl;
      hello->at_sequence(lc_sequence);
      Test::sequenceType lv_sequence = hello->at_sequence();
      eqVector(lv_sequence, lc_sequence);
    }

    // struct attribute
    {
      const Test::structType lc_struct { 'a', (short) 77, L"Hello!" };
      TAOX11_TEST_DEBUG << "Test IDL struct attribute." << std::endl;
      hello->at_struct(lc_struct);
      Test::structType lv_struct = hello->at_struct();
      TestEqual("lv_struct", lv_struct.c(), lc_struct.c());
    }

    // union attribute
    {
      Test::unionType lc_union;
      TAOX11_TEST_DEBUG << "Test IDL union attribute." << std::endl;
      // Test long in union.
      lc_union.longData(1234567890);
      hello->at_union(lc_union);
      Test::unionType lv_union = hello->at_union();
      TestEqual("lv_union", lc_union, lv_union);
      // Test wchar in union.
      lc_union.wchar_tData(L'a');
      hello->at_union(lc_union);
      lv_union = hello->at_union();
      TestEqual("lv_union", lc_union, lv_union);
      // Test double in union.
      lc_union.doubleData(12.0);
      hello->at_union(lc_union);
      lv_union = hello->at_union();
      TestEqual("lv_unione", lc_union, lv_union);
      // Test bool in union.
      lc_union.boolData(true);
      hello->at_union(lc_union);
      lv_union = hello->at_union();
      TestEqual("lv_union", lc_union, lv_union);
      // Test string in union.
      lc_union.stringData("Lean season");
      hello->at_union(lc_union);
      lv_union = hello->at_union();
      TestEqual("lv_union", lc_union, lv_union);
      // Test wstring in union.
      lc_union.wstringData(L"Wide season");
      hello->at_union(lc_union);
      lv_union = hello->at_union();
      TestEqual("lv_union", lc_union, lv_union);
    }

    // char attribute with c++11 keyword name
    {
      TAOX11_TEST_DEBUG << "Test IDL char attribute with c++11 keyword name 'do'." << std::endl;
      hello->_cxx_do('c');
      char char_key = hello->_cxx_do();
      TestEqual("char_with_key_name ", char_key, 'c');
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
    return 1;
  }
  return result_;
}
