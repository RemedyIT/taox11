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
#include "ace/Get_Opt.h"

int
main(int argc, ACE_TCHAR *argv[])
{
  // Compilation test
  Point2D p2d;
  TAOX11_TEST_DEBUG << "Point2D: " << p2d << std::endl;
  Point3D p3d;
  TAOX11_TEST_DEBUG << "Point3D: " << p3d << std::endl;
  Point4D p4d;
  TAOX11_TEST_DEBUG << "Point4D: " << p4d << std::endl;
  EmptyStruct es;
  TAOX11_TEST_DEBUG << "EmptyStruct: " << es << std::endl;
  DeriveEmpty de;
  TAOX11_TEST_DEBUG << "DeriveEmpty: " << de << std::endl;
  DeriveEmptyWithMember dewm;
  TAOX11_TEST_DEBUG << "DeriveEmptyWithMember: " << dewm << std::endl;
  EmptyFromDerived efd;
  TAOX11_TEST_DEBUG << "EmptyFromDerived: " << efd << std::endl;
  EmptyFromDerivedDerived efdd;
  TAOX11_TEST_DEBUG << "EmptyFromDerivedDerived: " << efdd << std::endl;
  ObjectStruct os;
  TAOX11_TEST_DEBUG << "ObjectStruct: " << os << std::endl;
  ObjectStructDerived osd;
  TAOX11_TEST_DEBUG << "ObjectStructDerived: " << osd << std::endl;

  int error_count = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
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

      IDL::traits<Foo>::ref_type foo = IDL::traits<Foo>::narrow (obj);

      if (!foo)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Foo>::narrow (obj) returned null object." << std::endl;
          return 1;
        }
      TAOX11_TEST_DEBUG << "narrowed Foo interface" << std::endl;

      Point2D sin {2, 5};
      Point2D sinout;
      Point2D sout;

      TAOX11_TEST_DEBUG << "Sending Point2D sin: " << sin << " sinout: " << sinout << std::endl;
      Point2D sret = foo->test_Point2D (sin, sinout, sout);
      TAOX11_TEST_DEBUG << "Received Point2D sret: " << sret << " sinout: " << sinout << " sout: " << sout << std::endl;

      Point3D sin5 {sin, 4};
      Point3D sinout5;
      Point3D sout5;

      TAOX11_TEST_DEBUG << "Sending Point3D sin: " << sin5 << " sinout: " << sinout5 << std::endl;
      Point3D sret5 = foo->test_Point3D (sin5, sinout5, sout5);
      TAOX11_TEST_DEBUG << "Received Point3D sret: " << sret5 << " sinout: " << sinout5 << " sout: " << sout5 << std::endl;

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;
      foo->shutdown ();
      _orb->destroy ();
    }
  catch (const CORBA::BAD_PARAM& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected CORBA::BAD_PARAM exception caught"
        << e << std::endl;
      ++error_count;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected exception caught: " << e << std::endl;
      ++error_count;
    }
  return error_count;
}
