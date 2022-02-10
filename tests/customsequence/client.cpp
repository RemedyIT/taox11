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
#include <vector>

#include "testdata.h"

using namespace Test;

int main(int argc, char* argv[])
{
  int result_ = 0;
  try {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr) {
      TAOX11_TEST_ERROR
          << "ERROR: Hello::CORBA::ORB_init (argc, argv) returned null ORB."
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

    // char-sequence
    {
      TAOX11_TEST_DEBUG << "Test char-sequence type get and set." << std::endl;
      Hello::mycharSeq seq = hello->get_charSeq(charVec);

      TAOX11_TEST_DEBUG << " ostream  test char seq " << seq << std::endl;

      if (!(eqv(seq, charOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->get_charSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test char-sequence type out parameter." << std::endl;
      Hello::mycharSeq outSeq;
      hello->out_charSeq(outSeq);
      if (!(eqv(outSeq, charOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->out_charSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test char-sequence type inout parameter." << std::endl;
      outSeq = charVec;
      hello->inout_charSeq(outSeq);
      if (!(eqv(outSeq, charOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_charSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // Wchar-sequence
    {
      TAOX11_TEST_DEBUG << "Test Wchar-sequence type get and set." << std::endl;
      Hello::mywchar_tSeq seq = hello->get_wchar_tSeq(wchar_tVec);

      TAOX11_TEST_DEBUG << " ostream  test wchar seq " << seq << std::endl;

      if (!(eqv(seq, wchar_tOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_wchar_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Wchar-sequence type out parameter." << std::endl;
      Hello::mywchar_tSeq outSeq;
      hello->out_wchar_tSeq(outSeq);
      if (!(eqv(outSeq, wchar_tOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_wchar_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Wchar-sequence type inout parameter." << std::endl;
      outSeq = wchar_tVec;
      hello->inout_wchar_tSeq(outSeq);
      if (!(eqv(outSeq, wchar_tOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_wchar_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // Octet-sequence
    {
      TAOX11_TEST_DEBUG << "Test Octet-sequence type get and set." << std::endl;
      Hello::myuint8_tSeq seq = hello->get_uint8_tSeq(OctetVec);

      std::ostringstream strb;
      TAOX11_TEST_DEBUG << " ostream  test octet seq " << seq << std::endl;

      if (!(eqv(seq, OctetOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_uint8_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Octet-sequence type out parameter." << std::endl;
      Hello::myuint8_tSeq outSeq;
      hello->out_uint8_tSeq(outSeq);
      if (!(eqv(outSeq, OctetOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_uint8_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Octet-sequence type inout parameter." << std::endl;
      outSeq = OctetVec;
      hello->inout_uint8_tSeq(outSeq);
      if (!(eqv(outSeq, OctetOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_uint8_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // bool-sequence
    {
      TAOX11_TEST_DEBUG << "Test bool-sequence type get and set." << std::endl;
      Hello::myboolSeq seq = hello->get_boolSeq(boolVec);

      TAOX11_TEST_DEBUG << " ostream  test bool seq " << seq << std::endl;


      if (!(eqv(seq, boolOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->get_boolSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test bool-sequence type out parameter." << std::endl;
      Hello::myboolSeq outSeq;
      hello->out_boolSeq(outSeq);
      if (!(eqv(outSeq, boolOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->out_boolSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test bool-sequence type inout parameter." << std::endl;
      outSeq = boolVec;
      hello->inout_boolSeq(outSeq);
      if (!(eqv(outSeq, boolOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_boolSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // Short-sequence
    {
      TAOX11_TEST_DEBUG << "Test Short-sequence type get and set." << std::endl;
      Hello::myint16_tSeq seq = hello->get_int16_tSeq(ShortVec);
      if (!(eqv(seq, ShortOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_int16_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Short-sequence type out parameter." << std::endl;
      Hello::myint16_tSeq outSeq;
      hello->out_int16_tSeq(outSeq);
      if (!(eqv(outSeq, ShortOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_int16_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Short-sequence type inout parameter." << std::endl;
      outSeq = ShortVec;
      hello->inout_int16_tSeq(outSeq);
      if (!(eqv(outSeq, ShortOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_int16_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // UShort-sequence
    {
      TAOX11_TEST_DEBUG << "Test UShort-sequence type get and set." << std::endl;
      Hello::myuint16_tSeq seq = hello->get_uint16_tSeq(UShortVec);
      if (!(eqv(seq, UShortOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_uint16_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test UShort-sequence type out parameter." << std::endl;
      Hello::myuint16_tSeq outSeq;
      hello->out_uint16_tSeq(outSeq);
      if (!(eqv(outSeq, UShortOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_uint16_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test UShort-sequence type inout parameter." << std::endl;
      outSeq = UShortVec;
      hello->inout_uint16_tSeq(outSeq);
      if (!(eqv(outSeq, UShortOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_uint16_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // Long-sequence
    {
      TAOX11_TEST_DEBUG << "Test Long-sequence type get and set." << std::endl;
      Hello::myint32_tSeq seq = hello->get_int32_tSeq(LongVec);

      TAOX11_TEST_DEBUG << " ostream  test long seq " << seq << std::endl;


      if (!(eqv(seq, LongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_int32_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Long-sequence type out parameter." << std::endl;
      Hello::myint32_tSeq outSeq;
      hello->out_int32_tSeq(outSeq);
      if (!(eqv(outSeq, LongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_int32_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test Long-sequence type inout parameter." << std::endl;
      outSeq = LongVec;
      hello->inout_int32_tSeq(outSeq);
      if (!(eqv(outSeq, LongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_int32_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // ULong-sequence
    {
      TAOX11_TEST_DEBUG << "Test ULong-sequence type get and set." << std::endl;
      Hello::myuint32_tSeq seq = hello->get_uint32_tSeq(ULongVec);
      if (!(eqv(seq, ULongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_uint32_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test ULong-sequence type out parameter." << std::endl;
      Hello::myuint32_tSeq outSeq;
      hello->out_uint32_tSeq(outSeq);
      if (!(eqv(outSeq, ULongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_uint32_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test ULong-sequence type inout parameter." << std::endl;
      outSeq = ULongVec;
      hello->inout_uint32_tSeq(outSeq);
      if (!(eqv(outSeq, ULongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_uint32_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // LongLong-sequence
    {
      TAOX11_TEST_DEBUG << "Test LongLong-sequence type get and set." << std::endl;
      Test::myint64_tSeq seq = hello->get_int64_tSeq(LongLongVec);

      TAOX11_TEST_DEBUG << " ostream  test longlong seq " << seq << std::endl;

      if (!(eqv(seq, LongLongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_int64_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test LongLong-sequence type out parameter." << std::endl;
      Test::myint64_tSeq outSeq;
      hello->out_int64_tSeq(outSeq);
      if (!(eqv(outSeq, LongLongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_int64_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test LongLong-sequence type inout parameter." << std::endl;
      outSeq = LongLongVec;
      hello->inout_int64_tSeq(outSeq);
      if (!(eqv(outSeq, LongLongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_int64_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // ULongLong-sequence
    {
      TAOX11_TEST_DEBUG << "Test ULongLong-sequence type get and set." << std::endl;
      Test::myuint64_tSeq seq = hello->get_uint64_tSeq(ULongLongVec);
      if (!(eqv(seq, ULongLongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->get_uint64_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test ULongLong-sequence type out parameter." << std::endl;
      Test::myuint64_tSeq outSeq;
      hello->out_uint64_tSeq(outSeq);
      if (!(eqv(outSeq, ULongLongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: hello->out_uint64_tSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test ULongLong-sequence type inout parameter." << std::endl;
      outSeq = ULongLongVec;
      hello->inout_uint64_tSeq(outSeq);
      if (!(eqv(outSeq, ULongLongOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_uint64_tSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // float-sequence
    {
      TAOX11_TEST_DEBUG << "Test float-sequence type get and set." << std::endl;
      Test::myfloatSeq seq = hello->get_floatSeq(floatVec);
      if (!(eqv(seq, floatOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->get_floatSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test float-sequence type out parameter." << std::endl;
      Test::myfloatSeq outSeq;
      hello->out_floatSeq(outSeq);
      if (!(eqv(outSeq, floatOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->out_floatSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test float-sequence type inout parameter." << std::endl;
      outSeq = floatVec;
      hello->inout_floatSeq(outSeq);
      if (!(eqv(outSeq, floatOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_floatSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // double-sequence
    {
      TAOX11_TEST_DEBUG << "Test double-sequence type get and set." << std::endl;



      Test::mydoubleSeq seq = hello->get_doubleSeq(doubleVec);

      TAOX11_TEST_DEBUG << " ostream  test double seq " << seq << std::endl;

      if (!(eqv(seq, doubleOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->get_doubleSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test double-sequence type out parameter." << std::endl;
      Test::mydoubleSeq outSeq;
      hello->out_doubleSeq(outSeq);
      if (!(eqv(outSeq, doubleOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->out_doubleSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test double-sequence type inout parameter." << std::endl;
      outSeq = doubleVec;
      hello->inout_doubleSeq(outSeq);
      if (!(eqv(outSeq, doubleOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_doubleSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    // long double-sequence
    {
      TAOX11_TEST_DEBUG << "Test long double-sequence type get and set." << std::endl;
      Test::myldoubleSeq seq = hello->get_ldoubleSeq(ldoubleVec);
      if (!(eqv(seq, ldoubleOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->get_ldoubleSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test long double-sequence type out parameter." << std::endl;
      Test::myldoubleSeq outSeq;
      hello->out_ldoubleSeq(outSeq);
      if (!(eqv(outSeq, ldoubleOutVec))) {
        TAOX11_TEST_ERROR << "ERROR: hello->out_ldoubleSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      TAOX11_TEST_DEBUG << "Test long double-sequence type inout parameter." << std::endl;
      outSeq = ldoubleVec;
      hello->inout_ldoubleSeq(outSeq);
      if (!(eqv(outSeq, ldoubleOutVec))) {
        TAOX11_TEST_DEBUG
            << "ERROR: Hello::inout_ldoubleSeq (out) received unexpected value."
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
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
  }
  return result_;
}

