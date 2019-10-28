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

// Test if vector content is equal.
template<typename T>
bool eqVector(const std::vector<T>& v1, std::vector<T> v2)
{
  typename std::vector<T>::iterator first2 = v2.begin();
  for (const T &el : v1)
    {
      if (el != *first2)
        {
          TAOX11_TEST_ERROR << "ERROR: Sequence element " << el << " <> " << *first2 << std::endl;
          ++result_;
          return false;
        }
      ++first2;
    }
  return true;
}

// Test if array content is equal.
template<typename T, size_t N>
bool eqArray(std::string name, std::array<T, N> v1, std::array<T, N> v2)
{
  typename std::array<T, N>::iterator first2 = v2.begin();
  for (const T &el : v1)
    {
      if (el != *first2)
        {
          TAOX11_TEST_ERROR << "ERROR: For " << name << " array element " << el << " <> " << *first2 << std::endl
                            << " v1: " << v1 << " v2: " << v2 << std::endl;
          return false;
        }
      ++first2;
    }
  return true;
}

bool eq_structType(Test::structType s1, Test::structType s2)
{
  if (!TestEqual("eq_structType.c", s1.c(), s2.c()))
    return false;
  if (!TestEqual("eq_structType.si", s1.si(), s2.si()))
    return false;
  if (!TestEqual("eq_structType.ws", s1.ws(), s2.ws()))
    return false;
  return true;
}

bool eq_structType(std::string name, Test::structType s1, Test::structType s2)
{
  if (!TestEqual("eq_structType " + name + ".c", s1.c(), s2.c()))
    return false;
  if (!TestEqual("eq_structType " + name + ".si", s1.si(), s2.si()))
    return false;
  if (!TestEqual("eq_structType " + name + ".ws", s1.ws(), s2.ws()))
    return false;
  return true;
}

bool eq_unionType(std::string name, Test::unionType s1, Test::unionType s2)
{
  if (!TestEqual("eq_unionType " + name + "._d", s1._d(), s2._d()))
    return false;
  switch (s1._d())
  {
    case 'l':
      if (!TestEqual("eq_unionType " + name + ".longData", s1.longData(),
          s2.longData()))
        return false;
      break;
    case 'w':
      if (!TestEqual("eq_unionType " + name + ".wchar_tData", s1.wchar_tData(),
          s2.wchar_tData()))
        return false;
      break;
    case 'b':
      if (!TestEqual("eq_unionType " + name + ".boolData", s1.boolData(),
          s2.boolData()))
        return false;
      break;
    case 'd':
      if (!TestEqual("eq_unionType " + name + ".doubleData", s1.doubleData(),
          s2.doubleData()))
        return false;
      break;
    default:
      break;
  }
  return true;
}

bool eq_all_struct(Test::all_struct s1, Test::all_struct s2)
{
  return (s1.enu() == s2.enu() && eqArray("eq_all_struct(s2.arr)", s1.arr(),
      s2.arr()) && eqVector(s1.seq(), s2.seq()) && eq_structType(s1.struc(),
      s2.struc()) && eq_unionType("eq_all_struct(s2.unio)", s1.unio(),
      s2.unio()));
  //  && s1.unio() == s2.unio());
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

    // array

    // Test array of enum type.
    {
      TAOX11_TEST_DEBUG << "Test IDL array of enum type." << std::endl;
      Test::enum_array v_in = { { Test::enumType::a, Test::enumType::b,
        Test::enumType::c, Test::enumType::a, Test::enumType::b } };
      Test::enum_array v_org = { { Test::enumType::c, Test::enumType::b,
        Test::enumType::c, Test::enumType::a, Test::enumType::b } };
      Test::enum_array v_inout = v_org;
      Test::enum_array v_out = { { Test::enumType::b, Test::enumType::b,
        Test::enumType::b, Test::enumType::b, Test::enumType::b } };
      Test::enum_array v_return = hello->inout_enum_array(v_in, v_out, v_inout);
      if (!eqArray("out", v_out, v_org) || !eqArray("inout", v_inout, v_in)
          || !eqArray("return", v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_enum_array unexpected value."
              << std::endl;
          ++result_;
        }

      hello->at_enum_array(v_in);
      v_return = hello->at_enum_array();
      if (!eqArray("return", v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->at_enum_array unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test array of array type.
    {
      TAOX11_TEST_DEBUG << "Test IDL array of array type." << std::endl;
      Test::array_array v_in = { { Test::arrayType {
        { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::arrayType { { 1.0, 2.0, 3.0, 4.0,
        5.0 } }, Test::arrayType { { 1.0, 2.0, 3.0, 4.0, 5.0 } },
        Test::arrayType { { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::arrayType { {
          1.0, 2.0, 3.0, 4.0, 5.0 } } } };
      Test::array_array v_org = { { Test::arrayType { { 99.99f, 2.0f, 3.0f, 4.0f,
        5.0f } }, Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f } },
        Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f } }, Test::arrayType { {
          1.0f, 2.0f, 3.0f, 4.0f, 5.0f } }, Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f,
          5.0f } } } };
      Test::array_array v_inout = v_org;
      Test::array_array v_out;
      Test::array_array v_return = hello->inout_array_array(v_in, v_out,
          v_inout);
      if (!eqArray("out", v_out, v_org) || !eqArray("inout", v_inout, v_in)
          || !eqArray("return", v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_array_array unexpected value."
              << std::endl;
          ++result_;
        }

      hello->at_array_array(v_in);
      v_return = hello->at_array_array();
      if (!eqArray("return", v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->at_array_array unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test array of sequence type.
    {
      TAOX11_TEST_DEBUG << "Test IDL array of sequence type." << std::endl;
      Test::sequence_array v_in = { { Test::sequenceType { { 1.0, 2.0, 3.0,
        4.0, 5.0 } }, Test::sequenceType { { 1.0, 2.0, 3.0, 4.0, 5.0 } },
        Test::sequenceType { { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::sequenceType {
          { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::sequenceType { { 1.0, 2.0, 3.0,
          4.0, 5.0 } } } };
      Test::sequence_array v_org = { { Test::sequenceType { { 99.99F, 2.0F, 3.0F,
        4.0F, 5.0F } }, Test::sequenceType { { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F } },
        Test::sequenceType { { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F } }, Test::sequenceType {
          { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F } }, Test::sequenceType { { 1.0F, 2.0F, 3.0F,
          4.0F, 5.0F } } } };
      Test::sequence_array v_inout = v_org;
      Test::sequence_array v_out;
      Test::sequence_array v_return = hello->inout_sequence_array(v_in, v_out,
          v_inout);
      if (!eqArray("out", v_out, v_org) || !eqArray("inout", v_inout, v_in)
          || !eqArray("return", v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_sequence_array unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test array of struct type.
    {
      TAOX11_TEST_DEBUG << "Test IDL array of struct type." << std::endl;
      Test::struct_array v_in =
          { { Test::structType { 'a', 1, L"b" }, Test::structType { 'a', 1,
            L"b" }, Test::structType { 'a', 1, L"b" }, Test::structType { 'a',
            1, L"b" }, Test::structType { 'a', 1, L"b" }, } };
      Test::struct_array v_org = { { Test::structType { 'A', 91, L"abc" },
        Test::structType { 'A', 91, L"abc" }, Test::structType { 'A', 91,
          L"abc" }, Test::structType { 'A', 91, L"abc" }, Test::structType {
          'A', 91, L"abc" }, } };
      Test::struct_array v_inout = v_org;
      Test::struct_array v_out;
      Test::struct_array v_return = hello->inout_struct_array(v_in, v_out,
          v_inout);
      if (!eq_structType("v_out", v_org[0], v_out[0]) || !eq_structType(
          "v_inout", v_in[0], v_inout[0]) || !eq_structType("v_return",
          v_in[0], v_return[0]))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_struct_array unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test array of union type.
    {
      TAOX11_TEST_DEBUG << "Test IDL array of union type." << std::endl;
      Test::unionType ut1;
      ut1.longData(1L);
      Test::unionType ut2;
      ut2.longData(12L);
      Test::unionType ut3;
      ut3.longData(123L);
      Test::unionType ut4;
      ut4.longData(1234L);
      Test::unionType ut5;
      ut5.longData(12345L);
      Test::union_array v_in = { { ut1, ut2, ut3, ut4, ut5 } };
      Test::union_array v_org = { { ut5, ut4, ut3, ut2, ut1 } };
      Test::union_array v_inout = v_org;
      Test::union_array v_out;
      Test::union_array v_return = hello->inout_union_array(v_in, v_out,
          v_inout);
      if (!eq_unionType("v_out", v_org[0], v_out[0]) || !eq_unionType(
          "v_inout", v_in[0], v_inout[0]) || !eq_unionType("v_return", v_in[0],
          v_return[0]))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_union_array unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // sequence

    // Test sequence of enum type.
    {
      TAOX11_TEST_DEBUG << "Test IDL sequence of enum type." << std::endl;
      Test::enum_sequence v_in = { { Test::enumType::a, Test::enumType::b,
        Test::enumType::c, Test::enumType::a, Test::enumType::b } };
      Test::enum_sequence v_org = { { Test::enumType::c, Test::enumType::b,
        Test::enumType::c, Test::enumType::a, Test::enumType::b } };
      Test::enum_sequence v_inout = v_org;
      Test::enum_sequence v_out;
      Test::enum_sequence v_return = hello->inout_enum_sequence(v_in, v_out,
          v_inout);
      if (!eqVector(v_out, v_org) || !eqVector(v_inout, v_in) || !eqVector(
          v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_enum_sequence unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test sequence of array type.
    {
      TAOX11_TEST_DEBUG << "Test IDL sequence of array type." << std::endl;

      Test::array_sequence v_in = { { Test::arrayType { { 1.0, 2.0, 3.0, 4.0,
        5.0 } }, Test::arrayType { { 1.0, 2.0, 3.0, 4.0, 5.0 } },
        Test::arrayType { { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::arrayType { {
          1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::arrayType { { 1.0, 2.0, 3.0, 4.0,
          5.0 } } } };
      Test::array_sequence v_org = { { Test::arrayType { { 99.99f, 2.0f, 3.0f,
        4.0f, 5.0f } }, Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f } },
        Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f } }, Test::arrayType { {
          1.0f, 2.0f, 3.0f, 4.0f, 5.0f } }, Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f,
          5.0f } } } };
      Test::array_sequence v_inout = v_org;
      Test::array_sequence v_out;
      Test::array_sequence v_return = hello->inout_array_sequence(v_in, v_out,
          v_inout);
      if (!eqVector(v_out, v_org) || !eqVector(v_inout, v_in) || !eqVector(
          v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_array_sequence unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test sequence of sequence type.
    {
      TAOX11_TEST_DEBUG << "Test IDL sequence of sequence type." << std::endl;
      Test::sequence_sequence v_in = { { Test::sequenceType { { 1.0, 2.0, 3.0,
        4.0, 5.0 } }, Test::sequenceType { { 1.0, 2.0, 3.0, 4.0, 5.0 } },
        Test::sequenceType { { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::sequenceType {
          { 1.0, 2.0, 3.0, 4.0, 5.0 } }, Test::sequenceType { { 1.0, 2.0, 3.0,
          4.0, 5.0 } } } };
      Test::sequence_sequence v_org = { { Test::sequenceType { { 99.99F, 2.0F,
        3.0F, 4.0F, 5.0F } }, Test::sequenceType { { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F } },
        Test::sequenceType { { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F } }, Test::sequenceType {
          { 1.0F, 2.0F, 3.0F, 4.0F, 5.0F } }, Test::sequenceType { { 1.0F, 2.0F, 3.0F,
          4.0F, 5.0F } } } };
      Test::sequence_sequence v_inout = v_org;
      Test::sequence_sequence v_out;
      Test::sequence_sequence v_return = hello->inout_sequence_sequence(v_in,
          v_out, v_inout);
      if (!eqVector(v_out, v_org) || !eqVector(v_inout, v_in) || !eqVector(
          v_return, v_in))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_sequence_sequence unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test sequence of struct type.
    {
      TAOX11_TEST_DEBUG << "Test IDL sequence of struct type." << std::endl;
      Test::struct_sequence v_in =
          { { Test::structType { 'a', 1, L"b" }, Test::structType { 'a', 1,
            L"b" }, Test::structType { 'a', 1, L"b" }, Test::structType { 'a',
            1, L"b" }, Test::structType { 'a', 1, L"b" }, } };
      Test::struct_sequence v_org = { { Test::structType { 'A', 91, L"abc" },
        Test::structType { 'A', 91, L"abc" }, Test::structType { 'A', 91,
          L"abc" }, Test::structType { 'A', 91, L"abc" }, Test::structType {
          'A', 91, L"abc" }, } };
      Test::struct_sequence v_inout = v_org;
      Test::struct_sequence v_out;
      Test::struct_sequence v_return = hello->inout_struct_sequence(v_in,
          v_out, v_inout);
      if (!eq_structType("v_out", v_org[0], v_out[0]) || !eq_structType(
          "v_inout", v_in[0], v_inout[0]) || !eq_structType("v_return",
          v_in[0], v_return[0]))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_struct_sequence unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test sequence of union type.
    {
      TAOX11_TEST_DEBUG << "Test IDL sequence of union type." << std::endl;
      Test::unionType ut1;
      ut1.longData(1L);
      Test::unionType ut2;
      ut2.longData(12L);
      Test::unionType ut3;
      ut3.longData(123L);
      Test::unionType ut4;
      ut4.longData(1234L);
      Test::unionType ut5;
      ut5.longData(12345L);
      Test::union_sequence v_in = { { ut1, ut2, ut3, ut4, ut5 } };
      Test::union_sequence v_org = { { ut5, ut4, ut3, ut2, ut1 } };
      Test::union_sequence v_inout = v_org;
      Test::union_sequence v_out;
      Test::union_sequence v_return = hello->inout_union_sequence(v_in, v_out,
          v_inout);
      if (!eq_unionType("v_out", v_org[0], v_out[0]) || !eq_unionType(
          "v_inout", v_in[0], v_inout[0]) || !eq_unionType("v_return", v_in[0],
          v_return[0]))
        {
          TAOX11_TEST_ERROR << "ERROR: hello->inout_union_sequence unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test struct of union type.
    {
      TAOX11_TEST_DEBUG << "Test IDL struct of union type." << std::endl;
      Test::unionType ut_in;
      ut_in.longData(12345678);
      Test::union_struct v_in;
      v_in.unio(ut_in);
      Test::unionType ut_org;
      ut_org.longData(87654321);
      Test::union_struct v_org;
      v_org.unio(ut_org);
      Test::union_struct v_inout = v_org;
      Test::union_struct v_out;
      Test::union_struct v_return = hello->inout_union_struct(v_in, v_out,
          v_inout);
      if (!eq_unionType("v_out", v_org.unio(), v_out.unio()) || !eq_unionType(
          "v_inout", v_in.unio(), v_inout.unio()) || !eq_unionType("v_return",
          v_in.unio(), v_return.unio()))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_union_struct unexpected value for union."
              << std::endl;
          ++result_;
        }
    }

    // Test struct of all complex types.
    {
      TAOX11_TEST_DEBUG << "Test IDL struct of all complex types." << std::endl;
      Test::unionType ut_in;
      ut_in.longData(12345678);
      Test::all_struct v_in;
      v_in.unio(ut_in);
      Test::unionType ut_org;
      ut_org.longData(87654321);
      Test::all_struct v_org;
      v_org.unio(ut_org);
      Test::all_struct v_inout = v_org;
      Test::all_struct v_out;
      Test::all_struct v_return = hello->inout_all_struct(v_in, v_out, v_inout);
      if (!eq_all_struct(v_org, v_out) || !eq_all_struct(v_in, v_inout)
          || !eq_all_struct(v_in, v_return))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_all_struct unexpected value."
              << std::endl;
          ++result_;
        }
    }

    // Test union of union type.
    {
      TAOX11_TEST_DEBUG << "Test IDL union of union type." << std::endl;
      Test::unionType ut1;
      ut1.longData(1L);
      Test::unionType ut2;
      ut2.longData(12L);
      Test::union_union v_in;
      v_in.unio(ut1);
      Test::union_union v_org;
      v_org.unio(ut2);
      Test::union_union v_inout = v_org;
      Test::union_union v_out;
      Test::union_union v_return = hello->inout_union_union(v_in, v_out,
          v_inout);
      if (!eq_unionType("v_out", v_org.unio(), v_out.unio()) || !eq_unionType(
          "v_inout", v_in.unio(), v_inout.unio()) || !eq_unionType("v_return",
          v_in.unio(), v_return.unio()))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_union_union unexpected value for union."
              << std::endl;
          ++result_;
        }
    }

    // Test union of all complex types.
    {
      TAOX11_TEST_DEBUG << "Test IDL union of all complex types." << std::endl;
      Test::all_union v_in;
      Test::all_union v_org;
      Test::all_union v_out;

      v_in.arr(Test::arrayType { { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f } });
      v_org.arr(Test::arrayType { { 91.09f, 92.09f, 93.09f, 94.09f, 95.09f } });
      Test::all_union v_inout = v_org;
      Test::all_union v_return = hello->inout_all_union(v_in, v_out, v_inout);
      if (!eqArray("out", v_org.arr(), v_out.arr()) || !eqArray("inout",
          v_in.arr(), v_inout.arr()) || !eqArray("return", v_in.arr(),
          v_return.arr()))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_all_union unexpected value for struct."
              << std::endl;
          ++result_;
        }

      v_in.enu(Test::enumType::a);
      v_org.enu(Test::enumType::b);
      v_inout = v_org;
      v_return = hello->inout_all_union(v_in, v_out, v_inout);
      if (!TestEqual("v_out", v_org.enu(), v_out.enu()) || !TestEqual(
          "v_inout", v_in.enu(), v_inout.enu()) || !TestEqual("v_return",
          v_in.enu(), v_return.enu()))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_all_union unexpected value for struct."
              << std::endl;
          ++result_;
        }

      v_in.struc(Test::structType { 'a', 1, L"b" });
      v_org.struc(Test::structType { 'Y', 1, L"Z" });
      v_inout = v_org;
      v_return = hello->inout_all_union(v_in, v_out, v_inout);
      if (!eq_structType("v_out", v_org.struc(), v_out.struc())
          || !eq_structType("v_inout", v_in.struc(), v_inout.struc())
          || !eq_structType("v_return", v_in.struc(), v_return.struc()))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_all_union unexpected value for struct."
              << std::endl;
          ++result_;
        }

      Test::unionType ut1;
      ut1.longData(1L);
      Test::unionType ut2;
      ut2.longData(12L);
      v_in.unio(ut1);
      v_org.unio(ut2);
      v_inout = v_org;
      v_return = hello->inout_all_union(v_in, v_out, v_inout);
      if (!eq_unionType("v_out", v_org.unio(), v_out.unio()) || !eq_unionType(
          "v_inout", v_in.unio(), v_inout.unio()) || !eq_unionType("v_return",
          v_in.unio(), v_return.unio()))
        {
          TAOX11_TEST_ERROR
              << "ERROR: hello->inout_all_union unexpected value for union."
              << std::endl;
          ++result_;
        }
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
