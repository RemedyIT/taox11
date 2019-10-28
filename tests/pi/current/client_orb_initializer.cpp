/**
 * @file    client_orb_initializer.cpp
 * @author  Martin Corino
 *
 * @brief   PICurrentTest server-side ORB initializer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "client_orb_initializer.h"

#include "client_request_interceptor.h"

#include "tao/x11/pi/pi.h"

#include "testlib/taox11_testlog.h"

PortableInterceptor::SlotId slot_id = 2093843211;

void
ClientORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  TAOX11_TEST_INFO << "ClientORBInitializer::pre_init called" << std::endl;

  TAOX11_TEST_INFO << "Client ORB id = " << info->orb_id () << std::endl;
}

void
ClientORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  TAOX11_TEST_INFO << "ClientORBInitializer::post_init called" << std::endl;


  IDL::traits<CORBA::Object>::ref_type obj =
      info->resolve_initial_references ("PICurrent");

  IDL::traits<PortableInterceptor::Current>::ref_type pi_current =
    IDL::traits<PortableInterceptor::Current>::narrow (obj);

  if (!pi_current)
  {
    TAOX11_TEST_ERROR << "Unable to resolve PICurrent reference." << std::endl;
    throw CORBA::INTERNAL ();
  }

  ::slot_id = info->allocate_slot_id ();

  IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type interceptor =
      CORBA::make_reference<ClientRequestInterceptor> (
          ::slot_id,
          pi_current);

  info->add_client_request_interceptor (interceptor);
}
