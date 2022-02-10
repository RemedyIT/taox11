/**
 * @file    client.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"

bool
test_struct_typecode (IDL::traits<Test::Foo>::ref_type foo)
{
  bool testFailed = false;
  Test::Simple simple (32, 12345, "test", 3.45, true, 'A', Test::EEnum::B);

  CORBA::Any any_struct;
  any_struct <<= simple;

  if (!foo->pass_data(any_struct))
  {
    TAOX11_TEST_ERROR << "ERROR: Foo::pass_data call for struct failed." << std::endl;
    testFailed = true;
  }
  if (!testFailed)
  {

    TAOX11_TEST_DEBUG << "successfully called Foo::pass_data for struct (" << simple << ")." << std::endl;

    Test::Simple simple2;
    CORBA::Any any_struct2 = foo->return_data(true);
    if (!(any_struct2 >>= simple2))
      {
        TAOX11_TEST_ERROR << "Error: Extraction or simple2 out of any failed " << std::endl;
        testFailed = true;
      }

    if ((simple2.o() == 32) && (simple2.l()==12345) && (simple2.s()=="test") &&
         (simple2.d() == 3.45) && (simple2.b()) && (simple2.c()=='A') && (simple2.e () == Test::EEnum::B))
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::return_data for struct (" << simple2 << ")."<< std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "Error Foo::return_data() for struct :" << simple2 << std::endl;
      testFailed = true;
    }
  }
  if (!testFailed)
  {
    Test::Simple simple3;
    CORBA::Any any_struct3;
    if (!foo->get_data(any_struct3, true))  //out
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::get_data call for struct failed." << std::endl;
      testFailed = true;
    }
    any_struct3>>= simple3;
    if ((simple3.o() == 32) && (simple3.l()==12345) && (simple3.s()=="test") &&
        (simple3.d() == 3.45) && (simple3.b()) && (simple3.c()=='A') && (simple3.e () == Test::EEnum::B))
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::get_data() for struct(" << simple3 << ")."<< std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "Error Foo::get_data call for struct failed :" << simple3 << std::endl;
      testFailed = true;
    }
    // This is a copy (as intended).
    Test::Simple simple4 (simple3);
    CORBA::Any any_struct4;
    any_struct4<<=std::move(simple4);
    // test non copying (move) Any operator for struct.
    if ((simple4.s().length()!=0) )
    {
      TAOX11_TEST_ERROR << "Error non-copying (move) any operator:" << simple4 << std::endl;
      testFailed = true;
    }
    else
    {
      TAOX11_TEST_DEBUG << "successfully used non-copying (move) any operator. (" << simple4 << ')' << std::endl;
    }

     //simple4
    if (!foo->update_struct(any_struct4)) //inout
    {
       TAOX11_TEST_ERROR << "Error: Foo::update_struct call failed." << std::endl;
       testFailed = true;
    }
    any_struct4>>=simple4;

    if ((simple4.o() == 32) && (simple4.l()==24690) && (simple4.s()=="test") &&
          (simple4.d() == 6.90) && (!simple4.b()) && (simple4.c()=='B') && (simple4.e () == Test::EEnum::C))
    {
      TAOX11_TEST_DEBUG << "successfully used non-copying any operator. (" << simple4 << ')' << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "Error Foo::update_struct():" << simple4 << std::endl;
      testFailed = true;
    }

    //test detecting wrong Any-type
    CORBA::Any wrongtype;
    if (any_struct4>>=wrongtype)
    {
      TAOX11_TEST_ERROR << "Error Foo::not detected wrong type" << std::endl;
      testFailed = true;
    }
    char t = 'x';
    wrongtype<<= t;
    if (foo->pass_data(wrongtype))
    {
      TAOX11_TEST_ERROR << "Error Foo::not detected passing wrong type" << std::endl;
      testFailed = true;
    }
  }
  return testFailed;
}

bool
test_union_typecode (IDL::traits<Test::Foo>::ref_type foo)
{
  bool testFailed = false;
  Test::Point point (10, 20);

  CORBA::Any any_union;
  Test::Data du;
  du._default ();
  uint32_t l = 1200;
  du.LongData(l);
  any_union <<= du;

  if (!foo->pass_data(any_union))
  {
    TAOX11_TEST_ERROR << "ERROR: Foo::pass_data call for union failed." << std::endl;
    testFailed = true;
  }
  if (!testFailed)
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::pass_data call for union." << std::endl;

    Test::Data du2;
    CORBA::Any any_union2 = foo->return_data(false);
    any_union2>>=du2;

    if (du2.LongData()== 1200)
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::return_data call for union."<< std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "Error Foo::return_data call for union" << std::endl;
      testFailed = true;
    }
  }
  if (!testFailed)
  {
    Test::Data du3;
    CORBA::Any any_union3;
    if (!foo->get_data(any_union3, false))  //out

    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::get_data call for union failed." << std::endl;
      testFailed = true;
    }
    if (!testFailed)
    {
      any_union3>>= du3;
      if (du3.LongData() == 1200)
      {
        TAOX11_TEST_DEBUG << "successfully called Foo::get_data call for union."<< std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "Error Foo::get_data call for union failed."  << std::endl;
        testFailed = true;
      }

      Test::Data du4 (du3);
      CORBA::Any any_union4;
      any_union4<<=std::move(du4);

      if (!foo->update_union(any_union4)) //inout
      {
        TAOX11_TEST_ERROR << "Error: Foo::update_union call failed." << std::endl;
        testFailed = true;
      }
      Test::Data du5;
      any_union4>>=du5;
      Test::Point pt(du5.pointData());

      if (pt.x() != 20)
      {
        TAOX11_TEST_ERROR << "Error Foo::update_union call returns wrong value ";  TAOX11_TEST_DEBUG << std::endl;
        testFailed = true;
      }
      else
      {
        TAOX11_TEST_DEBUG << "successfully called Foo::update_union call."<< std::endl;
      }
    }
  }

  // Test non-copying Any operator
  Test::Data du6;
  Test::Data du7;
  du6.stringData("test");
  CORBA::Any any_union6;
  any_union6<<=std::move(du6);
  any_union6>>=du7;
  if (du7._d() != Test::DataType::dtString)
  {
    TAOX11_TEST_ERROR << "Error non-copying (move) any operator." << std::endl;
  }
  if (du6.stringData() == "")
  {
      TAOX11_TEST_DEBUG << "successfully used non-copying (move) any operator. source= (" << du6.stringData();
      TAOX11_TEST_DEBUG << ") destination= ( " << du6.stringData() << ")" <<std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Error non-copying (move) any operator for unions " <<std::endl;
    testFailed = true;
  }

  return testFailed;
}




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

      bool testFailed =  test_struct_typecode (foo);

      testFailed |= test_union_typecode (foo);

      if(testFailed)
        {
          TAOX11_TEST_ERROR << "ERROR in test with struct and union typecode." << std::endl;
          return 1;
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
