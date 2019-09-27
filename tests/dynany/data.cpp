/**
 * @file    data.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   Implementation file for the class containing test constants
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "data.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/portable_server/portableserver.h"
#include "testlib/taox11_testlog.h"

Data::Data (IDL::traits< CORBA::ORB>::ref_type orb) :
    orb_ (orb)
{
  try
  {
    // Getting the RootPOA so we can generate object references.
    IDL::traits< CORBA::Object>::ref_type obj =
      this->orb_->resolve_initial_references ("RootPOA");


    if (obj == nullptr)
    {
      TAOX11_TEST_ERROR << "Unable to get root poa reference."
                        << std::endl;
    }

    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

    if (!root_poa)
    {
      TAOX11_TEST_ERROR <<
          "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
          << std::endl;
    }
    // _interface_repository_id() of both m_objref, will in this testcase
    // always return "IDL:omg.org/CORBA/Object.1.o"
    this->m_objref2 = root_poa->create_reference ("foo");
    this->m_objref1 = root_poa->create_reference ("bar");

   }
  catch (const std::exception& e)
  {
     TAOX11_TEST_ERROR << "ERROR: exception caught in Data: " << e.what () << std::endl;
  }
}
