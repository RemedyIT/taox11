/**
 * @file    server_orb_initializer.cpp
 * @author  Martin Corino
 *
 * @brief   PICurrentTest server-side ORB initializer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "server_orb_initializer.h"

#include "server_request_interceptor.h"
#include "client_request_interceptor2.h"
#include "tao/x11/pi/pi.h"

#include "testlib/taox11_testlog.h"

PortableInterceptor::SlotId slot_id = 2093843221;

void
ServerORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  TAOX11_TEST_INFO << "ServerORBInitializer::pre_init called" << std::endl;

  TAOX11_TEST_INFO << "Server ORB id = " << info->orb_id () << std::endl;
}

void
ServerORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  TAOX11_TEST_INFO << "ServerORBInitializer::post_init called" << std::endl;

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

  IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type server_interceptor =
      CORBA::make_reference<ServerRequestInterceptor> (
          ::slot_id,
          pi_current);

  info->add_server_request_interceptor (server_interceptor);

  IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type client_interceptor =
      CORBA::make_reference<ClientRequestInterceptor2> (
          ::slot_id);

  info->add_client_request_interceptor (client_interceptor);

//  // Disable collocation -- TAO-specific!!!
//  //
//  // Collocation optimizations must be disabled since TAO doesn't
//  // implement interceptor support for THRU_POA collocated
//  // invocations yet, and we need to force a client request
//  // interceptor to be invoked in this server.
//  TAO_VERSIONED_NAMESPACE_NAME::TAO_ORBInitInfo_var tao_info =
//      TAO_VERSIONED_NAMESPACE_NAME::TAO_ORBInitInfo::_narrow (info->get_proxy ()->get ());
//
//  tao_info->orb_core ()->optimize_collocation_objects (0);
}
