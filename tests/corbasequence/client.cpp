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
#include <vector>

#include "testdata.h"

int main(int argc, char* argv[])
{
  int result = 0;
  try {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object("file://test.ior");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

    IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

    if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object." << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    // Char-sequence
    {
      TAOX11_TEST_DEBUG << "Test Char-sequence type get and set." << std::endl;
      CORBA::CharSeq seq = hello->get_CharSeq(CharVec);

      TAOX11_TEST_DEBUG << "  ostream test CharSeq : " << seq << std::endl;

      if (!eqv (seq, CharOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_CharSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Char-sequence type out parameter." << std::endl;
      CORBA::CharSeq outSeq;
      hello->out_CharSeq (outSeq);
      if (!eqv(outSeq, CharOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_CharSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Char-sequence type inout parameter." << std::endl;
      outSeq = CharVec;
      hello->inout_CharSeq (outSeq);
      if (!eqv (outSeq, CharOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_CharSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // WChar-sequence
    {
      TAOX11_TEST_DEBUG << "Test WChar-sequence type get and set." << std::endl;
      CORBA::WCharSeq seq = hello->get_WCharSeq(WCharVec);

      TAOX11_TEST_DEBUG << "  ostream  test WCharSeq : " << seq << std::endl;

      if (!eqv (seq, WCharOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_WCharSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test WChar-sequence type out parameter." << std::endl;
      CORBA::WCharSeq outSeq;
      hello->out_WCharSeq (outSeq);
      if (!eqv (outSeq, WCharOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_WCharSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test WChar-sequence type inout parameter." << std::endl;
      outSeq = WCharVec;
      hello->inout_WCharSeq (outSeq);
      if (!eqv (outSeq, WCharOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_WCharSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // String-sequence
    {
      TAOX11_TEST_DEBUG << "Test String-sequence type get and set." << std::endl;
      CORBA::StringSeq seq = hello->get_StringSeq(StringVec);

      TAOX11_TEST_DEBUG << "  ostream test StringSeq : " << seq << std::endl;

      if (!eqv (seq, StringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_StringSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test String-sequence type out parameter." << std::endl;
      CORBA::StringSeq outSeq;
      hello->out_StringSeq (outSeq);
      if (!eqv(outSeq, StringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_StringSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test String-sequence type inout parameter." << std::endl;
      outSeq = StringVec;
      hello->inout_StringSeq (outSeq);
      if (!eqv (outSeq, StringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_StringSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // WString-sequence
    {
      TAOX11_TEST_DEBUG << "Test WString-sequence type get and set." << std::endl;
      CORBA::WStringSeq seq = hello->get_WStringSeq(WStringVec);

      TAOX11_TEST_DEBUG << "  ostream  test WStringSeq : " << seq << std::endl;

      if (!eqv (seq, WStringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_WStringSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test WString-sequence type out parameter." << std::endl;
      CORBA::WStringSeq outSeq;
      hello->out_WStringSeq (outSeq);
      if (!eqv (outSeq, WStringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_WStringSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test WString-sequence type inout parameter." << std::endl;
      outSeq = WStringVec;
      hello->inout_WStringSeq (outSeq);
      if (!eqv (outSeq, WStringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_WStringSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // Octet-sequence
    {
      TAOX11_TEST_DEBUG << "Test Octet-sequence type get and set." << std::endl;
      CORBA::OctetSeq seq = hello->get_OctetSeq (OctetVec);

      TAOX11_TEST_DEBUG << "  ostream test OctetSeq : " << seq << std::endl;

      if (!eqv (seq, OctetOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_OctetSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Octet-sequence type out parameter." << std::endl;
      CORBA::OctetSeq outSeq;
      hello->out_OctetSeq (outSeq);
      if (!eqv (outSeq, OctetOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_OctetSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Octet-sequence type inout parameter." << std::endl;
      outSeq = OctetVec;
      hello->inout_OctetSeq (outSeq);
      if (!eqv (outSeq, OctetOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_OctetSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // Boolean-sequence
    {
      TAOX11_TEST_DEBUG << "Test Boolean-sequence type get and set." << std::endl;
      CORBA::BooleanSeq seq = hello->get_BooleanSeq (BooleanVec);

      TAOX11_TEST_DEBUG << "  ostream test BooleanSeq : " << seq << std::endl;

      if (!eqv (seq, BooleanOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_BooleanSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Boolean-sequence type out parameter." << std::endl;
      CORBA::BooleanSeq outSeq;
      hello->out_BooleanSeq (outSeq);
      if (!eqv (outSeq, BooleanOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_BooleanSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Boolean-sequence type inout parameter." << std::endl;
      outSeq = BooleanVec;
      hello->inout_BooleanSeq (outSeq);
      if (!eqv (outSeq, BooleanOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_BooleanSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // Short-sequence
    {
      TAOX11_TEST_DEBUG << "Test Short-sequence type get and set." << std::endl;
      CORBA::ShortSeq seq = hello->get_ShortSeq (ShortVec);

      TAOX11_TEST_DEBUG << "  ostream test ShortSeq " << seq << std::endl;

      if (!eqv (seq, ShortOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_ShortSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Short-sequence type out parameter." << std::endl;
      CORBA::ShortSeq outSeq;
      hello->out_ShortSeq(outSeq);
      if (!eqv (outSeq, ShortOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_ShortSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Short-sequence type inout parameter." << std::endl;
      outSeq = ShortVec;
      hello->inout_ShortSeq (outSeq);
      if (!eqv (outSeq, ShortOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_ShortSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // UShort-sequence
    {
      TAOX11_TEST_DEBUG << "Test UShort-sequence type get and set." << std::endl;
      CORBA::UShortSeq seq = hello->get_UShortSeq (UShortVec);

      TAOX11_TEST_DEBUG << " ostream  test UShortSeq " << seq << std::endl;

      if (!eqv (seq, UShortOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_UShortSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test UShort-sequence type out parameter." << std::endl;
      CORBA::UShortSeq outSeq;
      hello->out_UShortSeq (outSeq);
      if (!eqv (outSeq, UShortOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_UShortSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test UShort-sequence type inout parameter." << std::endl;
      outSeq = UShortVec;
      hello->inout_UShortSeq (outSeq);
      if (!eqv (outSeq, UShortOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_UShortSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // Long-sequence
    {
      TAOX11_TEST_DEBUG << "Test Long-sequence type get and set." << std::endl;
      CORBA::LongSeq seq = hello->get_LongSeq (LongVec);

      TAOX11_TEST_DEBUG << "  ostream test LongSeq : " << seq << std::endl;

      if (!eqv (seq, LongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_LongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Long-sequence type out parameter." << std::endl;
      CORBA::LongSeq outSeq;
      hello->out_LongSeq (outSeq);
      if (!eqv (outSeq, LongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_LongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Long-sequence type inout parameter." << std::endl;
      outSeq = LongVec;
      hello->inout_LongSeq (outSeq);
      if (!eqv (outSeq, LongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_LongSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // ULong-sequence
    {
      TAOX11_TEST_DEBUG << "Test ULong-sequence type get and set." << std::endl;
      CORBA::ULongSeq seq = hello->get_ULongSeq (ULongVec);

      TAOX11_TEST_DEBUG << "  ostream test ULongSeq : " << seq << std::endl;

      if (!eqv (seq, ULongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_ULongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test ULong-sequence type out parameter." << std::endl;
      CORBA::ULongSeq outSeq;
      hello->out_ULongSeq (outSeq);
      if (!eqv (outSeq, ULongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_ULongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test ULong-sequence type inout parameter." << std::endl;
      outSeq = ULongVec;
      hello->inout_ULongSeq (outSeq);
      if (!eqv (outSeq, ULongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_ULongSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // LongLong-sequence
    {
      TAOX11_TEST_DEBUG << "Test LongLong-sequence type get and set." << std::endl;
      CORBA::LongLongSeq seq = hello->get_LongLongSeq (LongLongVec);

      TAOX11_TEST_DEBUG << "  ostream test LongLong : " << seq << std::endl;

      if (!eqv (seq, LongLongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_LongLongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test LongLong-sequence type out parameter." << std::endl;
      CORBA::LongLongSeq outSeq;
      hello->out_LongLongSeq (outSeq);
      if (!eqv (outSeq, LongLongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_LongLongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test LongLong-sequence type inout parameter." << std::endl;
      outSeq = LongLongVec;
      hello->inout_LongLongSeq (outSeq);
      if (!eqv (outSeq, LongLongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_LongLongSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // ULongLong-sequence
    {
      TAOX11_TEST_DEBUG << "Test ULongLong-sequence type get and set." << std::endl;
      CORBA::ULongLongSeq seq = hello->get_ULongLongSeq (ULongLongVec);

      TAOX11_TEST_DEBUG << "  ostream test ULongLongSeq : " << seq << std::endl;

      if (!eqv (seq, ULongLongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_ULongLongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test ULongLong-sequence type out parameter." << std::endl;
      CORBA::ULongLongSeq outSeq;
      hello->out_ULongLongSeq (outSeq);
      if (!eqv (outSeq, ULongLongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_ULongLongSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test ULongLong-sequence type inout parameter." << std::endl;
      outSeq = ULongLongVec;
      hello->inout_ULongLongSeq (outSeq);
      if (!eqv (outSeq, ULongLongOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_ULongLongSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // Float-sequence
    {
      TAOX11_TEST_DEBUG << "Test Float-sequence type get and set." << std::endl;
      CORBA::FloatSeq seq = hello->get_FloatSeq (FloatVec);

      TAOX11_TEST_DEBUG << "  ostream test FloatSeq : " << seq << std::endl;

      if (!eqv (seq, FloatOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_FloatSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Float-sequence type out parameter." << std::endl;
      CORBA::FloatSeq outSeq;
      hello->out_FloatSeq (outSeq);
      if (!eqv (outSeq, FloatOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_FloatSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Float-sequence type inout parameter." << std::endl;
      outSeq = FloatVec;
      hello->inout_FloatSeq (outSeq);
      if (!eqv (outSeq, FloatOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_FloatSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // Double-sequence
    {
      TAOX11_TEST_DEBUG << "Test Double-sequence type get and set." << std::endl;
      CORBA::DoubleSeq seq = hello->get_DoubleSeq (DoubleVec);

      TAOX11_TEST_DEBUG << "  ostream test DoubleSeq : " << seq << std::endl;

      if (!eqv (seq, DoubleOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_DoubleSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Double-sequence type out parameter." << std::endl;
      CORBA::DoubleSeq outSeq;
      hello->out_DoubleSeq (outSeq);
      if (!eqv (outSeq, DoubleOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_DoubleSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test Double-sequence type inout parameter." << std::endl;
      outSeq = DoubleVec;
      hello->inout_DoubleSeq (outSeq);
      if (!eqv (outSeq, DoubleOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_DoubleSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // LongDouble-sequence
    {
      TAOX11_TEST_DEBUG << "Test LongDouble-sequence type get and set." << std::endl;
      CORBA::LongDoubleSeq seq = hello->get_LongDoubleSeq (LongDoubleVec);

      TAOX11_TEST_DEBUG << "  ostream test LongDouble : " << seq << std::endl;

      if (!eqv (seq, LongDoubleOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_LongDoubleSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test LongDouble-sequence type out parameter." << std::endl;
      CORBA::LongDoubleSeq outSeq;
      hello->out_LongDoubleSeq (outSeq);
      if (!eqv (outSeq, LongDoubleOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_LongDoubleSeq() received unexpected value."
          << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "Test LongDouble-sequence type inout parameter." << std::endl;
      outSeq = LongDoubleVec;
      hello->inout_LongDoubleSeq (outSeq);
      if (!eqv (outSeq, LongDoubleOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_LongDoubleSeq (out) received unexpected value."
          << std::endl;
        ++result;
      }
    }

    // End

    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown();
    TAOX11_TEST_DEBUG << std::endl;
    _orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result;
  }
  return result;
}
