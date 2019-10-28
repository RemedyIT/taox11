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
#include "testdata.h"

int main(int argc, char* argv[])
{
  int result_ = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

      if (_orb == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object(
          "file://test.ior");

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: string_to_object(<ior>) returned null reference."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

      if (!hello)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

      // Test test data generator code
      Test::charArray3 a3;
      fill(a3, 'A');
      TAOX11_TEST_DEBUG << "Test output of testdata:";
      TAOX11_TEST_DEBUG << a3[0][0];

      TAOX11_TEST_DEBUG << std::endl << "Test 1, 2 and 3 dimensional arrays:"
          << std::endl;

      // bool
        {
          // Test array of IDL bool
          TAOX11_TEST_DEBUG << "Test array of IDL bool." << std::endl;
          Test::boolArray obool = hello->get_boolArray(_bool1);
          if (!eqArray(_bool2, obool))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_boolArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::boolArray iobool = _bool1;
          Test::boolArray rbool = hello->inout_boolArray(_bool1, obool, iobool);
          if (!eqArray(_bool1, rbool) || !eqArray(_bool1, obool)
              || !eqArray(_bool2, iobool))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_boolArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL bool
          Test::boolArray2 boolcon2a;
          fill(boolcon2a, false);
          Test::boolArray2 boolcon2b;
          fill(boolcon2b, true);
          Test::boolArray2 oboolArray2 = hello->get_boolArray2(boolcon2a);
          if (!eqArray(boolcon2b, oboolArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_boolArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::boolArray2 ioboolArray2 = boolcon2a;
          Test::boolArray2 rboolArray2 = hello->inout_boolArray2(boolcon2a,
              oboolArray2, ioboolArray2);
          if (!eqArray(boolcon2a, rboolArray2)
              || !eqArray(boolcon2a, oboolArray2)
              || !eqArray(boolcon2b, ioboolArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_boolArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL bool
          Test::boolArray3 boolcon3a;
          fill(boolcon3a, false);
          Test::boolArray3 boolcon3b;
          fill(boolcon3b, true);
          Test::boolArray3 oboolArray3 = hello->get_boolArray3(boolcon3a);
          if (!eqArray(boolcon3b, oboolArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_boolArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::boolArray3 ioboolArray3 = boolcon3a;
          Test::boolArray3 rboolArray3 = hello->inout_boolArray3(boolcon3a,
              oboolArray3, ioboolArray3);
          if (!eqArray(boolcon3a, rboolArray3)
              || !eqArray(boolcon3a, oboolArray3)
              || !eqArray(boolcon3b, ioboolArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_boolArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // char
        {
          // Test array of IDL char
          TAOX11_TEST_DEBUG << "Test array of IDL char." << std::endl;
          Test::charArray ochar = hello->get_charArray(_char1);
          if (!eqArray(_char2, ochar))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_charArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::charArray iochar = _char1;
          Test::charArray rchar = hello->inout_charArray(_char1, ochar, iochar);
          if (!eqArray(_char1, rchar) || !eqArray(_char1, ochar)
              || !eqArray(_char2, iochar))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_charArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL char
          Test::charArray2 con2a;
          fill(con2a, 'A');
          Test::charArray2 con2b;
          fill(con2b, '1');
          Test::charArray2 ocharArray2 = hello->get_charArray2(con2a);
          if (!eqArray(con2b, ocharArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_charArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::charArray2 iocharArray2 = con2a;
          Test::charArray2 rcharArray2 = hello->inout_charArray2(con2a,
              ocharArray2, iocharArray2);
          if (!eqArray(con2a, rcharArray2) || !eqArray(con2a, ocharArray2)
              || !eqArray(con2b, iocharArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_charArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL char
          Test::charArray3 con3a;
          fill(con3a, 'A');
          Test::charArray3 con3b;
          fill(con3b, '1');
          Test::charArray3 ocharArray3 = hello->get_charArray3(con3a);
          if (!eqArray(con3b, ocharArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_charArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::charArray3 iocharArray3 = con3a;
          Test::charArray3 rcharArray3 = hello->inout_charArray3(con3a,
              ocharArray3, iocharArray3);
          if (!eqArray(con3a, rcharArray3) || !eqArray(con3a, ocharArray3)
              || !eqArray(con3b, iocharArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_charArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // octet
        {
          // Test array of IDL octet
          TAOX11_TEST_DEBUG << "Test array of IDL octet." << std::endl;
          Test::octetArray ooctet = hello->get_octetArray(_octet1);
          if (!eqArray(_octet2, ooctet))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_octetArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::octetArray iooctet = _octet1;
          Test::octetArray roctet = hello->inout_octetArray(_octet1, ooctet,
              iooctet);
          if (!eqArray(_octet1, roctet) || !eqArray(_octet1, ooctet)
              || !eqArray(_octet2, iooctet))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_octetArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL octet
          Test::octetArray2 octetcon2a;
          fill(octetcon2a, (uint8_t) 1);
          Test::octetArray2 octetcon2b;
          fill(octetcon2b, (uint8_t) 10);
          Test::octetArray2 ooctetArray2 = hello->get_octetArray2(octetcon2a);
          if (!eqArray(octetcon2b, ooctetArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_octetArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::octetArray2 iooctetArray2 = octetcon2a;
          Test::octetArray2 roctetArray2 = hello->inout_octetArray2(octetcon2a,
              ooctetArray2, iooctetArray2);
          if (!eqArray(octetcon2a, roctetArray2)
              || !eqArray(octetcon2a, ooctetArray2)
              || !eqArray(octetcon2b, iooctetArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_octetArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL octet
          Test::octetArray3 octetcon3a;
          fill(octetcon3a, (uint8_t) 1);
          Test::octetArray3 octetcon3b;
          fill(octetcon3b, (uint8_t) 10);
          Test::octetArray3 ooctetArray3 = hello->get_octetArray3(octetcon3a);
          if (!eqArray(octetcon3b, ooctetArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_octetArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::octetArray3 iooctetArray3 = octetcon3a;
          Test::octetArray3 roctetArray3 = hello->inout_octetArray3(octetcon3a,
              ooctetArray3, iooctetArray3);
          if (!eqArray(octetcon3a, roctetArray3)
              || !eqArray(octetcon3a, ooctetArray3)
              || !eqArray(octetcon3b, iooctetArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_octetArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // wchar
        {
          // Test array of IDL wchar
          TAOX11_TEST_DEBUG << "Test array of IDL wchar." << std::endl;
          Test::wcharArray owchar = hello->get_wcharArray(_wchar1);
          if (!eqArray(_wchar2, owchar))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_wcharArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::wcharArray iowchar = _wchar1;
          Test::wcharArray rwchar = hello->inout_wcharArray(_wchar1, owchar,
              iowchar);
          if (!eqArray(_wchar1, rwchar) || !eqArray(_wchar1, owchar)
              || !eqArray(_wchar2, iowchar))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_wcharArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL wchar
          Test::wcharArray2 con2a;
          fill(con2a, L'A');
          Test::wcharArray2 con2b;
          fill(con2b, L'a');
          Test::wcharArray2 owcharArray2 = hello->get_wcharArray2(con2a);
          if (!eqArray(con2b, owcharArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_wcharArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::wcharArray2 iowcharArray2 = con2a;
          Test::wcharArray2 rwcharArray2 = hello->inout_wcharArray2(con2a,
              owcharArray2, iowcharArray2);
          if (!eqArray(con2a, rwcharArray2) || !eqArray(con2a, owcharArray2)
              || !eqArray(con2b, iowcharArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_wcharArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL wchar
          Test::wcharArray3 con3a;
          fill(con3a, L'A');
          Test::wcharArray3 con3b;
          fill(con3b, L'a');
          Test::wcharArray3 owcharArray3 = hello->get_wcharArray3(con3a);
          if (!eqArray(con3b, owcharArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_wcharArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::wcharArray3 iowcharArray3 = con3a;
          Test::wcharArray3 rwcharArray3 = hello->inout_wcharArray3(con3a,
              owcharArray3, iowcharArray3);
          if (!eqArray(con3a, rwcharArray3) || !eqArray(con3a, owcharArray3)
              || !eqArray(con3b, iowcharArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_wcharArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // short
        {
          // Test array of IDL short
          TAOX11_TEST_DEBUG << "Test array of IDL short." << std::endl;
          Test::shortArray oshort = hello->get_shortArray(_short1);
          if (!eqArray(_short2, oshort))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_shortArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::shortArray ioshort = _short1;
          Test::shortArray rshort = hello->inout_shortArray(_short1, oshort,
              ioshort);
          if (!eqArray(_short1, rshort) || !eqArray(_short1, oshort)
              || !eqArray(_short2, ioshort))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_shortArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL short
          Test::shortArray2 shortcon2a;
          fill(shortcon2a, (short) 1);
          Test::shortArray2 shortcon2b;
          fill(shortcon2b, (short) 1000);
          Test::shortArray2 oshortArray2 = hello->get_shortArray2(shortcon2a);
          if (!eqArray(shortcon2b, oshortArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_shortArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::shortArray2 ioshortArray2 = shortcon2a;
          Test::shortArray2 rshortArray2 = hello->inout_shortArray2(shortcon2a,
              oshortArray2, ioshortArray2);
          if (!eqArray(shortcon2a, rshortArray2)
              || !eqArray(shortcon2a, oshortArray2)
              || !eqArray(shortcon2b, ioshortArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_shortArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL short
          Test::shortArray3 shortcon3a;
          fill(shortcon3a, (short) 1);
          Test::shortArray3 shortcon3b;
          fill(shortcon3b, (short) 1000);
          Test::shortArray3 oshortArray3 = hello->get_shortArray3(shortcon3a);
          if (!eqArray(shortcon3b, oshortArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_shortArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::shortArray3 ioshortArray3 = shortcon3a;
          Test::shortArray3 rshortArray3 = hello->inout_shortArray3(shortcon3a,
              oshortArray3, ioshortArray3);
          if (!eqArray(shortcon3a, rshortArray3)
              || !eqArray(shortcon3a, oshortArray3)
              || !eqArray(shortcon3b, ioshortArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_shortArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // ushort
        {
          // Test array of IDL unsigned short
          TAOX11_TEST_DEBUG << "Test array of IDL unsigned short." << std::endl;
          Test::ushortArray oushort = hello->get_ushortArray(_ushort1);
          if (!eqArray(_ushort2, oushort))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ushortArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ushortArray ioushort = _ushort1;
          Test::ushortArray rushort = hello->inout_ushortArray(_ushort1,
              oushort, ioushort);
          if (!eqArray(_ushort1, rushort) || !eqArray(_ushort1, oushort)
              || !eqArray(_ushort2, ioushort))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ushortArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL unsigned short
          Test::ushortArray2 ushortcon2a;
          fill(ushortcon2a, (uint16_t) 1);
          Test::ushortArray2 ushortcon2b;
          fill(ushortcon2b, (uint16_t) 1000);
          Test::ushortArray2 oushortArray2 = hello->get_ushortArray2(
              ushortcon2a);
          if (!eqArray(ushortcon2b, oushortArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ushortArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ushortArray2 ioushortArray2 = ushortcon2a;
          Test::ushortArray2 rushortArray2 = hello->inout_ushortArray2(
              ushortcon2a, oushortArray2, ioushortArray2);
          if (!eqArray(ushortcon2a, rushortArray2)
              || !eqArray(ushortcon2a, oushortArray2)
              || !eqArray(ushortcon2b, ioushortArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ushortArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL unsigned short
          Test::ushortArray3 ushortcon3a;
          fill(ushortcon3a, (uint16_t) 1);
          Test::ushortArray3 ushortcon3b;
          fill(ushortcon3b, (uint16_t) 1000);
          Test::ushortArray3 oushortArray3 = hello->get_ushortArray3(
              ushortcon3a);
          if (!eqArray(ushortcon3b, oushortArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ushortArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ushortArray3 ioushortArray3 = ushortcon3a;
          Test::ushortArray3 rushortArray3 = hello->inout_ushortArray3(
              ushortcon3a, oushortArray3, ioushortArray3);
          if (!eqArray(ushortcon3a, rushortArray3)
              || !eqArray(ushortcon3a, oushortArray3)
              || !eqArray(ushortcon3b, ioushortArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ushortArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // long
        {
          // Test array of IDL long
          TAOX11_TEST_DEBUG << "Test array of IDL long." << std::endl;
          Test::longArray olong = hello->get_longArray(_long1);
          if (!eqArray(_long2, olong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_longArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::longArray iolong = _long1;
          Test::longArray rlong = hello->inout_longArray(_long1, olong, iolong);
          if (!eqArray(_long1, rlong) || !eqArray(_long1, olong)
              || !eqArray(_long2, iolong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_longArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL long
          Test::longArray2 longcon2a;
          fill(longcon2a, 1);
          Test::longArray2 longcon2b;
          fill(longcon2b, 1000);
          Test::longArray2 olongArray2 = hello->get_longArray2(longcon2a);
          if (!eqArray(longcon2b, olongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_longArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::longArray2 iolongArray2 = longcon2a;
          Test::longArray2 rlongArray2 = hello->inout_longArray2(longcon2a,
              olongArray2, iolongArray2);
          if (!eqArray(longcon2a, rlongArray2)
              || !eqArray(longcon2a, olongArray2)
              || !eqArray(longcon2b, iolongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_longArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL long
          Test::longArray3 longcon3a;
          fill(longcon3a, 1);
          Test::longArray3 longcon3b;
          fill(longcon3b, 1000);
          Test::longArray3 olongArray3 = hello->get_longArray3(longcon3a);
          if (!eqArray(longcon3b, olongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_longArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::longArray3 iolongArray3 = longcon3a;
          Test::longArray3 rlongArray3 = hello->inout_longArray3(longcon3a,
              olongArray3, iolongArray3);
          if (!eqArray(longcon3a, rlongArray3)
              || !eqArray(longcon3a, olongArray3)
              || !eqArray(longcon3b, iolongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_longArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // ulong
        {
          // Test array of IDL unsigned long
          TAOX11_TEST_DEBUG << "Test array of IDL unsigned long." << std::endl;
          Test::ulongArray oulong = hello->get_ulongArray(_ulong1);
          if (!eqArray(_ulong2, oulong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ulongArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ulongArray ioulong = _ulong1;
          Test::ulongArray rulong = hello->inout_ulongArray(_ulong1, oulong,
              ioulong);
          if (!eqArray(_ulong1, rulong) || !eqArray(_ulong1, oulong)
              || !eqArray(_ulong2, ioulong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ulongArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL unsigned long
          Test::ulongArray2 ulongcon2a;
          fill(ulongcon2a, 1u);
          Test::ulongArray2 ulongcon2b;
          fill(ulongcon2b, 1000u);
          Test::ulongArray2 oulongArray2 = hello->get_ulongArray2(ulongcon2a);
          if (!eqArray(ulongcon2b, oulongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ulongArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ulongArray2 ioulongArray2 = ulongcon2a;
          Test::ulongArray2 rulongArray2 = hello->inout_ulongArray2(ulongcon2a,
              oulongArray2, ioulongArray2);
          if (!eqArray(ulongcon2a, rulongArray2)
              || !eqArray(ulongcon2a, oulongArray2)
              || !eqArray(ulongcon2b, ioulongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ulongArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL unsigned long
          Test::ulongArray3 ulongcon3a;
          fill(ulongcon3a, 1u);
          Test::ulongArray3 ulongcon3b;
          fill(ulongcon3b, 1000u);
          Test::ulongArray3 oulongArray3 = hello->get_ulongArray3(ulongcon3a);
          if (!eqArray(ulongcon3b, oulongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ulongArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ulongArray3 ioulongArray3 = ulongcon3a;
          Test::ulongArray3 rulongArray3 = hello->inout_ulongArray3(ulongcon3a,
              oulongArray3, ioulongArray3);
          if (!eqArray(ulongcon3a, rulongArray3)
              || !eqArray(ulongcon3a, oulongArray3)
              || !eqArray(ulongcon3b, ioulongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ulongArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // llong
        {
          // Test array of IDL long long
          TAOX11_TEST_DEBUG << "Test array of IDL long long." << std::endl;
          Test::llongArray ollong = hello->get_llongArray(_llong1);
          if (!eqArray(_llong2, ollong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_llongArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::llongArray iollong = _llong1;
          Test::llongArray rllong = hello->inout_llongArray(_llong1, ollong,
              iollong);
          if (!eqArray(_llong1, rllong) || !eqArray(_llong1, ollong)
              || !eqArray(_llong2, iollong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_llongArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL long long
          Test::llongArray2 llongcon2a;
          fill(llongcon2a, int64_t(1));
          Test::llongArray2 llongcon2b;
          fill(llongcon2b, int64_t(1000));
          Test::llongArray2 ollongArray2 = hello->get_llongArray2(llongcon2a);
          if (!eqArray(llongcon2b, ollongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_llongArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::llongArray2 iollongArray2 = llongcon2a;
          Test::llongArray2 rllongArray2 = hello->inout_llongArray2(llongcon2a,
              ollongArray2, iollongArray2);
          if (!eqArray(llongcon2a, rllongArray2)
              || !eqArray(llongcon2a, ollongArray2)
              || !eqArray(llongcon2b, iollongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_llongArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL IDL long long
          Test::llongArray3 llongcon3a;
          fill(llongcon3a, int64_t(1));
          Test::llongArray3 llongcon3b;
          fill(llongcon3b, int64_t(1000));
          Test::llongArray3 ollongArray3 = hello->get_llongArray3(llongcon3a);
          if (!eqArray(llongcon3b, ollongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_llongArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::llongArray3 iollongArray3 = llongcon3a;
          Test::llongArray3 rllongArray3 = hello->inout_llongArray3(llongcon3a,
              ollongArray3, iollongArray3);
          if (!eqArray(llongcon3a, rllongArray3)
              || !eqArray(llongcon3a, ollongArray3)
              || !eqArray(llongcon3b, iollongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_llongArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // unsigned long long
        {
          // Test array of IDL unsigned long long
          TAOX11_TEST_DEBUG << "Test array of IDL unsigned long long." << std::endl;
          Test::ullongArray oullong = hello->get_ullongArray(_ullong1);
          if (!eqArray(_ullong2, oullong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ullongArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ullongArray ioullong = _ullong1;
          Test::ullongArray rullong = hello->inout_ullongArray(_ullong1,
              oullong, ioullong);
          if (!eqArray(_ullong1, rullong) || !eqArray(_ullong1, oullong)
              || !eqArray(_ullong2, ioullong))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ullongArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL unsigned long long
          Test::ullongArray2 ullongcon2a;
          fill(ullongcon2a, uint64_t(1));
          Test::ullongArray2 ullongcon2b;
          fill(ullongcon2b, uint64_t(1000));
          Test::ullongArray2 oullongArray2 = hello->get_ullongArray2(
              ullongcon2a);
          if (!eqArray(ullongcon2b, oullongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ullongArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ullongArray2 ioullongArray2 = ullongcon2a;
          Test::ullongArray2 rullongArray2 = hello->inout_ullongArray2(
              ullongcon2a, oullongArray2, ioullongArray2);
          if (!eqArray(ullongcon2a, rullongArray2)
              || !eqArray(ullongcon2a, oullongArray2)
              || !eqArray(ullongcon2b, ioullongArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ullongArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL unsigned long long
          Test::ullongArray3 ullongcon3a;
          fill(ullongcon3a, uint64_t(1));
          Test::ullongArray3 ullongcon3b;
          fill(ullongcon3b, uint64_t(1000));
          Test::ullongArray3 oullongArray3 = hello->get_ullongArray3(
              ullongcon3a);
          if (!eqArray(ullongcon3b, oullongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ullongArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::ullongArray3 ioullongArray3 = ullongcon3a;
          Test::ullongArray3 rullongArray3 = hello->inout_ullongArray3(
              ullongcon3a, oullongArray3, ioullongArray3);
          if (!eqArray(ullongcon3a, rullongArray3)
              || !eqArray(ullongcon3a, oullongArray3)
              || !eqArray(ullongcon3b, ioullongArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ullongArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // float
        {
          // Test array of IDL float
          TAOX11_TEST_DEBUG << "Test array of IDL float." << std::endl;
          Test::floatArray ofloat = hello->get_floatArray(_float1);
          if (!eqArray(_float2, ofloat))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_floatArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::floatArray iofloat = _float1;
          Test::floatArray rfloat = hello->inout_floatArray(_float1, ofloat,
              iofloat);
          if (!eqArray(_float1, rfloat) || !eqArray(_float1, ofloat)
              || !eqArray(_float2, iofloat))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_floatArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL float
          Test::floatArray2 floatcon2a;
          fill(floatcon2a, (float) 1);
          Test::floatArray2 floatcon2b;
          fill(floatcon2b, (float) 1000);
          Test::floatArray2 ofloatArray2 = hello->get_floatArray2(floatcon2a);
          if (!eqArray(floatcon2b, ofloatArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_floatArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::floatArray2 iofloatArray2 = floatcon2a;
          Test::floatArray2 rfloatArray2 = hello->inout_floatArray2(floatcon2a,
              ofloatArray2, iofloatArray2);
          if (!eqArray(floatcon2a, rfloatArray2)
              || !eqArray(floatcon2a, ofloatArray2)
              || !eqArray(floatcon2b, iofloatArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_floatArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL float
          Test::floatArray3 floatcon3a;
          fill(floatcon3a, (float) 1);
          Test::floatArray3 floatcon3b;
          fill(floatcon3b, (float) 1000);
          Test::floatArray3 ofloatArray3 = hello->get_floatArray3(floatcon3a);
          if (!eqArray(floatcon3b, ofloatArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_floatArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::floatArray3 iofloatArray3 = floatcon3a;
          Test::floatArray3 rfloatArray3 = hello->inout_floatArray3(floatcon3a,
              ofloatArray3, iofloatArray3);
          if (!eqArray(floatcon3a, rfloatArray3)
              || !eqArray(floatcon3a, ofloatArray3)
              || !eqArray(floatcon3b, iofloatArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_floatArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // double
        {
          // Test array of IDL double
          TAOX11_TEST_DEBUG << "Test array of IDL double." << std::endl;
          Test::doubleArray odouble = hello->get_doubleArray(_double1);
          if (!eqArray(_double2, odouble))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_doubleArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::doubleArray iodouble = _double1;
          Test::doubleArray rdouble = hello->inout_doubleArray(_double1,
              odouble, iodouble);
          if (!eqArray(_double1, rdouble) || !eqArray(_double1, odouble)
              || !eqArray(_double2, iodouble))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_doubleArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL double
          Test::doubleArray2 doublecon2a;
          fill(doublecon2a, (double) 1);
          Test::doubleArray2 doublecon2b;
          fill(doublecon2b, (double) 1000);
          Test::doubleArray2 odoubleArray2 = hello->get_doubleArray2(
              doublecon2a);
          if (!eqArray(doublecon2b, odoubleArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_doubleArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::doubleArray2 iodoubleArray2 = doublecon2a;
          Test::doubleArray2 rdoubleArray2 = hello->inout_doubleArray2(
              doublecon2a, odoubleArray2, iodoubleArray2);
          if (!eqArray(doublecon2a, rdoubleArray2)
              || !eqArray(doublecon2a, odoubleArray2)
              || !eqArray(doublecon2b, iodoubleArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_doubleArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL double
          Test::doubleArray3 doublecon3a;
          fill(doublecon3a, (double) 1);
          Test::doubleArray3 doublecon3b;
          fill(doublecon3b, (double) 1000);
          Test::doubleArray3 odoubleArray3 = hello->get_doubleArray3(
              doublecon3a);
          if (!eqArray(doublecon3b, odoubleArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_doubleArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::doubleArray3 iodoubleArray3 = doublecon3a;
          Test::doubleArray3 rdoubleArray3 = hello->inout_doubleArray3(
              doublecon3a, odoubleArray3, iodoubleArray3);
          if (!eqArray(doublecon3a, rdoubleArray3)
              || !eqArray(doublecon3a, odoubleArray3)
              || !eqArray(doublecon3b, iodoubleArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_doubleArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // long double
        {
          // Test array of IDL long double
          TAOX11_TEST_DEBUG << "Test array of IDL long double." << std::endl;
          Test::Hello::ldoubleArray oldouble = hello->get_ldoubleArray(
              _ldouble1);
          if (!eqArray(_ldouble2, oldouble))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ldoubleArray unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::Hello::ldoubleArray ioldouble = _ldouble1;
          Test::Hello::ldoubleArray rldouble = hello->inout_ldoubleArray(
              _ldouble1, oldouble, ioldouble);
          if (!eqArray(_ldouble1, rldouble) || !eqArray(_ldouble1, oldouble)
              || !eqArray(_ldouble2, ioldouble))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ldoubleArray unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 2 dimensional array of IDL long double
          Test::Hello::ldoubleArray2 ldoublecon2a;
          fill(ldoublecon2a, (long double) 1);
          Test::Hello::ldoubleArray2 ldoublecon2b;
          fill(ldoublecon2b, (long double) 1000);
          Test::Hello::ldoubleArray2 oldoubleArray2 = hello->get_ldoubleArray2(
              ldoublecon2a);
          if (!eqArray(ldoublecon2b, oldoubleArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ldoubleArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::Hello::ldoubleArray2 ioldoubleArray2 = ldoublecon2a;
          Test::Hello::ldoubleArray2 rldoubleArray2 =
              hello->inout_ldoubleArray2(ldoublecon2a, oldoubleArray2,
                  ioldoubleArray2);
          if (!eqArray(ldoublecon2a, rldoubleArray2)
              || !eqArray(ldoublecon2a, oldoubleArray2)
              || !eqArray(ldoublecon2b, ioldoubleArray2))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ldoubleArray2 unexpected return value."
                << std::endl;
              ++result_;
            }
          // Test 3 dimensional array of IDL long double
          Test::Hello::ldoubleArray3 ldoublecon3a;
          fill(ldoublecon3a, (long double) 1);
          Test::Hello::ldoubleArray3 ldoublecon3b;
          fill(ldoublecon3b, (long double) 1000);
          Test::Hello::ldoubleArray3 oldoubleArray3 = hello->get_ldoubleArray3(
              ldoublecon3a);
          if (!eqArray(ldoublecon3b, oldoubleArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::get_ldoubleArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
          Test::Hello::ldoubleArray3 ioldoubleArray3 = ldoublecon3a;
          Test::Hello::ldoubleArray3 rldoubleArray3 =
              hello->inout_ldoubleArray3(ldoublecon3a, oldoubleArray3,
                  ioldoubleArray3);
          if (!eqArray(ldoublecon3a, rldoubleArray3)
              || !eqArray(ldoublecon3a, oldoubleArray3)
              || !eqArray(ldoublecon3b, ioldoubleArray3))
            {
              TAOX11_TEST_ERROR
                << "ERROR: Hello::inout_ldoubleArray3 unexpected return value."
                << std::endl;
              ++result_;
            }
        }

      // End

      TAOX11_TEST_DEBUG << "shutting down...";

      hello->shutdown();
      _orb->destroy();
      TAOX11_TEST_DEBUG << std::endl;
    } catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }
  return result_;
}
