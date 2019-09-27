/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"

#include "testlib/taox11_testlog.h"
#include <limits>

int result_ = 0;

template<class T>
void
TestEqual(std::string name, T exp, T a) {
  if (a != exp) {
    TAOX11_TEST_ERROR << "ERROR: value for " << name << " expected " << exp
      << " , received " << a << std::endl;
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

int
main(int argc, char* argv[]) {
  try {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr) {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
        << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object("file://test.ior");

    if (!obj) {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello) {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    // struct with special basic types
    {
      TAOX11_TEST_DEBUG << "Test struct with special basic type." << std::endl;
      TAOX11_TEST_DEBUG << " get of struct" << std::endl;
      Test::simpleStruct s1 = hello->get_struct();
      TestEqual("s1.c", s1.c(), '4');
      s1.c('1');
      Test::simpleStruct s2;
      s2.c('2');
      Test::simpleStruct s3;
      s3.c('3');
      s1.wc(L'1');
      s2.wc(L'2');
      s3.wc(L'3');
      s1.b(true);
      s2.b(true);
      s3.b(false);
      s1.o(1);
      s2.o(2);
      s3.o(3);
      TAOX11_TEST_DEBUG << " get, set, in and in/out." << std::endl;
      Test::simpleStruct s4 = hello->inout_simpleStruct(s1, s2, s3);
      TestEqual("s2.c", s2.c(), '1');
      TestEqual("s3.c", s3.c(), '1');
      TestEqual("s4.c", s4.c(), '3');
      TestEqual("s2.b", s2.b(), true);
      TestEqual("s3.b", s3.b(), true);
      TestEqual("s4.b", s4.b(), false);
      TestEqual("s2.o", s2.o(), (uint8_t) 1);
      TestEqual("s3.o", s3.o(), (uint8_t) 1);
      TestEqual("s4.o", s4.o(), (uint8_t) 3);
      TestEqual("s2.wc", s2.wc(), L'1');
      TestEqual("s3.wc", s3.wc(), L'1');
      TestEqual("s4.wc", s4.wc(), L'3');
    }

    // struct with basic types
    {
      TAOX11_TEST_DEBUG << "Test struct with basic type." << std::endl;
      Test::basicStruct s1;
      Test::basicStruct s2;
      Test::basicStruct s3;
      s1.si(-1);
      s2.si(-2);
      s3.si(-3);
      s1.usi(1);
      s2.usi(2);
      s3.usi(3);
      s1.li(-11);
      s2.li(-22);
      s3.li(-33);
      s1.uli(11);
      s2.uli(22);
      s3.uli(33);
      s1.lli(-111);
      s2.lli(-222);
      s3.lli(-333);
      s1.ulli(111);
      s2.ulli(222);
      s3.ulli(333);
      s1.f(-1.0F);
      s2.f(-2.0F);
      s3.f(-3.0F);
      s1.df(-11.0);
      s2.df(-22.0);
      s3.df(-33.0);
      s1.ldf(-111.0L);
      s2.ldf(-222.0L);
      s3.ldf(-333.0L);
      s1.s("1");
      s2.s("2");
      s3.s("3");
      s1.ws(L"11");
      s2.ws(L"22");
      s3.ws(L"33");
      TAOX11_TEST_DEBUG << " get, set, in and in/out." << std::endl;
      Test::basicStruct s4 = hello->inout_basicStruct(s1, s2, s3);
      TestEqual("s2.si", s2.si(), (int16_t) -1);
      TestEqual("s3.si", s3.si(), (int16_t) -1);
      TestEqual("s4.si", s4.si(), (int16_t) -3);
      TestEqual("s2.usi", s2.usi(), (uint16_t) 1);
      TestEqual("s3.usi", s3.usi(), (uint16_t) 1);
      TestEqual("s4.usi", s4.usi(), (uint16_t) 3);
      TestEqual("s2.li", s2.li(), (int32_t) -11);
      TestEqual("s3.li", s3.li(), (int32_t) -11);
      TestEqual("s4.li", s4.li(), (int32_t) -33);
      TestEqual("s2.uli", s2.uli(), (uint32_t) 11);
      TestEqual("s3.uli", s3.uli(), (uint32_t) 11);
      TestEqual("s4.uli", s4.uli(), (uint32_t) 33);
      TestEqual("s2.lli", s2.lli(), (int64_t) -111);
      TestEqual("s3.lli", s3.lli(), (int64_t) -111);
      TestEqual("s4.lli", s4.lli(), (int64_t) -333);
      TestEqual("s2.ulli", s2.ulli(), (uint64_t) 111);
      TestEqual("s3.ulli", s3.ulli(), (uint64_t) 111);
      TestEqual("s4.ulli", s4.ulli(), (uint64_t) 333);
      TestEqual("s2.f", s2.f(), -1.0F);
      TestEqual("s3.f", s3.f(), -1.0F);
      TestEqual("s4.f", s4.f(), -3.0F);
      TestEqual("s2.df", s2.df(), -11.0);
      TestEqual("s3.df", s3.df(), -11.0);
      TestEqual("s4.df", s4.df(), -33.0);
      TestEqual("s2.ldf", s2.ldf(), -111.0L);
      TestEqual("s3.ldf", s3.ldf(), -111.0L);
      TestEqual("s4.ldf", s4.ldf(), -333.0L);
      TestEqual("s2.s", s2.s(), std::string("1"));
      TestEqual("s3.s", s3.s(), std::string("1"));
      TestEqual("s4.s", s4.s(), std::string("3"));
      TestEqual("s2.ws", s2.ws(), std::wstring(L"11"));
      TestEqual("s3.ws", s3.ws(), std::wstring(L"11"));
      TestEqual("s4.ws", s4.ws(), std::wstring(L"33"));
    }

    // enumStruct
    {
      TAOX11_TEST_DEBUG << "Test struct with enum, array and struct." << std::endl;
      Test::enumStruct s1;
      s1.num(Test::numeral::First);
      s1.bs().s("Pirmas");
      s1.arr()[0][0] = 11;
      Test::enumStruct s2;
      s2.num(Test::numeral::Second);
      s2.bs().s("Antras");
      s2.arr()[0][0] = 22;
      Test::enumStruct s3;
      s3.num(Test::numeral::Third);
      s3.bs().s("Trecias");
      s3.arr()[0][0] = 33;
      TAOX11_TEST_DEBUG << " get, set, in and in/out." << std::endl;
      Test::enumStruct s4 = hello->inout_enumStruct(s1, s2, s3);
      TestEqual("s2", Test::numeral::First, s2.num());
      TestEqual("s3", Test::numeral::First, s3.num());
      TestEqual("s4", Test::numeral::Third, s4.num());
      TestEqual("s2.s", s2.bs().s(), std::string("Pirmas"));
      TestEqual("s3.s", s3.bs().s(), std::string("Pirmas"));
      TestEqual("s4.s", s4.bs().s(), std::string("Trecias"));
      TestEqual("s2.arr", s2.arr()[0][0], (uint8_t)11);
      TestEqual("s3.arr", s3.arr()[0][0], (uint8_t)11);
      TestEqual("s4.arr", s4.arr()[0][0], (uint8_t)33);
    }

    // sequenceStruct
    {
      TAOX11_TEST_DEBUG << "Test struct with sequence." << std::endl;
      Test::sequenceStruct s1;

      s1.short1(1);
      Test::sequenceStruct s2;
      s2.short1(2);
      Test::sequenceStruct s3;
      s3.short1(3);
      Test::seqStructseq seq;
      seq.push_back(s2);
      s3.children(seq);
      TAOX11_TEST_DEBUG << " get, set, in and in/out." << std::endl;
      Test::sequenceStruct s4 = hello->inout_sequenceStruct(s1, s2, s3);
      TestEqual("s2", (short) 10, s2.short1());
      TestEqual("s3", (short) 1, s3.short1());
      TestEqual("s4", (short) 30, s4.short1());
      Test::sequenceStruct s4child = s4.children().at(0);
      TestEqual("s4 child", (short) 2, s4child.short1());
      try
      {
        Test::sequenceStruct s3child = s3.children().at(0);
        TAOX11_TEST_ERROR << "ERROR: exception expected on s3." << std::endl;
        ++result_;
      }
      catch (const std::out_of_range&)
      {
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
    ++result_;
  }
  return result_;
}
