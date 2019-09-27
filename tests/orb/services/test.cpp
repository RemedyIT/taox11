/**
 * @file    test.cpp
 * @author  Marijke Hengstmengel
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
/**
 * Test whether list_initial_services returns an ObjectURL or an ObjectId as
 * it should be when an ORBInitRef is set.
 * Test whether get_service_information returns a NO_IMPLEMENT because
 * it isn't implemented in TAO_CORBA
 *
 * launch test like this:
 *  <execname> -ORBInitRef MyObjectId=file://myobj.ior
 *
 */

#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/object.h"
#include "testlib/taox11_testlog.h"

int main( int argc, char* argv[] )
{
  int retcode = 1;

  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
      return 1;
    }

    if (_orb->id () != "")
    {
      TAOX11_TEST_ERROR << "ERROR: ORBid not empty." << std::endl;
      return 1;
    }

    std::string orbname = "SecondORB";
    IDL::traits<CORBA::ORB>::ref_type _orb2 = CORBA::ORB_init (argc, argv, orbname);
    if (_orb2 == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv, " << orbname << ") returned null ORB." << std::endl;
      return 1;
    }

    if (_orb2->id () != orbname)
    {
      TAOX11_TEST_ERROR << "ERROR: ORBid not " << orbname << std::endl;
      return 1;
    }

    CORBA::ORB::ObjectIdList listsrv = _orb->list_initial_services();
    for (CORBA::ORB::ObjectId an_oid : listsrv)
      {
        if (an_oid =="MyObjectId")
          {
            TAOX11_TEST_DEBUG << "OK found " <<  an_oid << " with list_initial_services. " <<  std::endl;
            retcode = 0;
          }
      }
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    CORBA::ServiceType serv_type = 0;
    CORBA::ServiceInformation serv_info;

    try {
      _orb->get_service_information(serv_type, serv_info);

      // Not implemented in TAO so if no exception -exit- with error
      return 1;
    }
    catch (const CORBA::NO_IMPLEMENT &e )
       {
         TAOX11_TEST_DEBUG << "expected exception caught: " << e << std::endl;

       }
#endif
    _orb2->destroy();
    _orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception in test : "
      << ex << std::endl;
    return 1;
  }

  return retcode;
}
