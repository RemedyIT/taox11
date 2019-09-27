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
#include "testdata.h"

int result_ = 0;

template<class T>
void TestEqual(T exp, T a)
{
  if (a != exp)
  {
    TAOX11_TEST_ERROR << "ERROR: value expected " << exp << " , received " << a
      << std::endl;
    ++result_;
  }
}

void get_basicType_union(Test::basicType_union u1, uint16_t disc)
{
  switch (disc)
  {
    case 0:
      u1.shortData();
      break;
    case 1:
      u1.ushortData();
      break;
    case 2:
      u1.longData();
      break;
    case 3:
      u1.ulongData();
      break;
    case 4:
      u1.llongData();
      break;
    case 5:
      u1.ullongData();
      break;
    case 6:
      u1.floatData();
      break;
    case 7:
      u1.doubleData();
      break;
    case 8:
      u1.ldoubleData();
      break;
    case 9:
      u1.stringData();
      break;
    case 10:
      u1.wstringData();
      break;
    default:
      u1.defaultData();
      break;
  };
}

void TestRetrieve(Test::basicType_union u1)
{
  for (uint16_t disc = 0; disc < 11; disc++)
  {
    if (u1._d() == disc)

    {
      TAOX11_TEST_DEBUG << "Test retrieving value for correct discriminator " << disc
        << std::endl;
      get_basicType_union(u1, disc);
    }
    else
    {
      TAOX11_TEST_DEBUG << "Test retrieving value for discriminator " << disc
        << " while current is " << u1._d();
      try
      {
        get_basicType_union(u1, disc);
        TAOX11_TEST_ERROR << ", ERROR: retrieve should result in CORBA::BAD_PARAM."
          << std::endl;
        ++result_;
      }
      catch (const CORBA::BAD_PARAM&)
      {
        TAOX11_TEST_DEBUG << ", retrieve results in CORBA::BAD_PARAM as expected."
          << std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR << ", ERROR: retrieve results in unexpected exception."
          << e << std::endl;
        ++result_;
      }
    }
  }
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

    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    // union
    // For union Z, calling the _default() modifier function causes the union's value to be
    //  composed solely of the discriminator value of false, since there is no explicit default member.
    Test::Z z;
    z._default(); // implicit default member selected
    bool disc = z._d(); // disc == false
    if (disc == true)
      {
        TAOX11_TEST_ERROR << "ERROR: union _d() returns true instead of false.";
        ++result_;
      }
    // union of special basic types.
    {
      TAOX11_TEST_DEBUG << "Test union of special basic types" << std::endl;
      Test::specialType_union preturn = hello->get_special_union();
      if (70 != preturn.octetData())
        {
          TAOX11_TEST_ERROR
            << "ERROR: get_special_union unexpected value for octetData. expected 70, found type "
            <<preturn._d() << " value " << preturn.octetData() << " ("
            << (int) (preturn.octetData()) << ")" << std::endl;
          ++result_;
        }

      Test::specialType_union pin;
      Test::specialType_union pout;
      Test::specialType_union pinout;
      pin.octetData(22);
      pinout.octetData(23);
      preturn = hello->inout_special_union(pin, pout, pinout);
      TestEqual("hello->inout_special_union pinout", (uint8_t) 22,
          pinout.octetData());
      if ((uint8_t) 22 != pout.octetData() || (uint8_t) 22
          != pinout.octetData() || (uint8_t) 23 != preturn.octetData())
        {
          TAOX11_TEST_ERROR
            << "ERROR: inout_special_union unexpected value for octetData. pinout "
            <<pinout.octetData() << ", out " << pout.octetData() << std::endl;
          ++result_;
        }
    }

    // union of basic types.
    for (uint16_t type = 0; type < 11; type++)
    {
      TAOX11_TEST_DEBUG << "Test union of basic types, type " << type << std::endl;
      // get_basic_union
      Test::basicType_union preturn = hello->get_basic_union();
      //v.wstringData(L"xyz");
      if (preturn.wstringData() != L"xyz")
      {
        TAOX11_TEST_ERROR_W << L"ERROR: get_basic_union unexpected value "
          << preturn.wstringData() << std::endl;
        ++result_;
      }

      // inout_basic_union
      Test::basicType_union pout;
      Test::basicType_union pin = create_basic_union(type, 5);
      Test::basicType_union pinout = create_basic_union(type, 6);
      preturn = hello->inout_basic_union(pin, pout, pinout);
      bool result = check_basic_union(type, 5, pout) && check_basic_union(type,
          5, pinout) && check_basic_union(type, 6, preturn);
      if (!result)
      {
        TAOX11_TEST_ERROR << "ERROR: get_basic_union unexpected value." << std::endl;
        ++result_;
      }
    }

    {
      // default value of union
      TAOX11_TEST_DEBUG << "Test default value of Test::basicType_union." << std::endl;
      Test::basicType_union bas;
      bas.defaultData(22.2F);
      if (bas.defaultData() != 22.2F)
        {
          ++result_;
          TAOX11_TEST_ERROR
            << "ERROR: Value for default basicType_union should be 22.2, found: "
            <<bas.defaultData() << std::endl;
        }
      // discriminator value for default can be anything BUT the defined labels
      if (bas._d() >= 0 && bas._d() <= 10)
        {
          ++result_;
          TAOX11_TEST_ERROR
            << "ERROR: Default for Test::basicType_union discriminator should be ???."
            <<bas._d() << std::endl;
        }
      // Test to get default value using _default() member.
      // Member _default() is not available because an explicit default has been defined in Test::basicType_union.

      // Test get value for invalid discriminator.
      bas.floatData(0.123F);
      TestRetrieve(bas);
      bas.longData(1234567890);
      TestRetrieve(bas);
      bas.stringData("Steve Jobs");
      TestRetrieve(bas);

      // Test setting of valid discriminator.
      TAOX11_TEST_DEBUG << "Test setting valid discriminator on Test::basicType_union."
        << std::endl;
      bas.longData(1234567890);
      bas._d(2); // OK

      // Test setting of invalid discriminator.
      TAOX11_TEST_DEBUG
        << "Test setting invalid discriminator on Test::basicType_union."
        <<std::endl;
      try
      {
        bas.longData(1234567890); // discriminator  is 2
        bas._d(1);
        ++result_;
        TAOX11_TEST_ERROR
          << "ERROR: Discriminator 1 is not valid for Test::basicType_union. Expected CORBA::BAD_PARAM exeption!"
          <<std::endl;
      }
      catch (const CORBA::BAD_PARAM&)
      {
        TAOX11_TEST_DEBUG
          << "Caught CORBA::BAD_PARAM as expected for invalid Test::basicType_union discriminator."
          <<std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR
          << "ERROR: Unexpected exeption for invalid Test::basicType_union discriminator. Expected CORBA::BAD_PARAM exeption!"
          << e << std::endl;
        ++result_;
      }

      // Test setting of non-existing/undefined discriminator.
      TAOX11_TEST_DEBUG
        << "Test setting undefined discriminator on Test::basicType_union."
        <<std::endl;
      try
      {
        bas.longData(1234567890); // discriminator  is 2
        bas._d(-1000);
        ++result_;
        TAOX11_TEST_ERROR
          << "ERROR: Discriminator -1000 is not valid (undefined) for Test::basicType_union. Expected CORBA::BAD_PARAM exeption!"
          <<std::endl;
      }
      catch (const CORBA::BAD_PARAM&)
      {
        TAOX11_TEST_DEBUG
          << "Caught CORBA::BAD_PARAM as expected for invalid (undefined) Test::basicType_union discriminator."
          <<std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR
          << "ERROR: Unexpected exeption for invalid (undefined) Test::basicType_union discriminator. Expected CORBA::BAD_PARAM exeption!"
          << e << std::endl;
        ++result_;
      }

      // default value of union
      TAOX11_TEST_DEBUG << "Test default value of Test::specialType_union."
        << std::endl;
      Test::specialType_union spe;
      spe._default();
      int discriminator = (int) spe._d();
      if (discriminator != 0)
      {
        ++result_;
        TAOX11_TEST_ERROR
          << "ERROR: Default discriminator for specialType_union should be 0, found "
          <<discriminator << std::endl;
      }

      // Test to get default value using _default() member.
      spe._default();

      // Test setting of valid discriminator.
      TAOX11_TEST_DEBUG
        << "Test setting valid discriminator on Test::specialType_union."
        <<std::endl;
      spe.boolData(true);
      spe._d('b'); // OK

      // Test setting of invalid discriminator.
      TAOX11_TEST_DEBUG
        << "Test setting invalid discriminator on Test::specialType_union."
        <<std::endl;
      try
      {
        spe.boolData(true);
        //spe._d('b'); // OK
        spe._d('!');
        ++result_;
        TAOX11_TEST_ERROR
          << "ERROR: Discriminator '!' is not valid (undefined) for Test::specialType_union. Expected CORBA::BAD_PARAM exeption!"
          <<std::endl;
      }
      catch (const CORBA::BAD_PARAM&)
      {
        TAOX11_TEST_DEBUG
          << "Caught CORBA::BAD_PARAM as expected for invalid (undefined) Test::specialType_union discriminator."
          <<std::endl;
      }
      catch (const CORBA::Exception& e)
      {
        TAOX11_TEST_ERROR
          << "ERROR: Unexpected exeption for invalid (undefined) Test::specialType_union discriminator. Expected CORBA::BAD_PARAM exeption! "
          << e << std::endl;
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
    ++result_;
  }
  return result_;
}
