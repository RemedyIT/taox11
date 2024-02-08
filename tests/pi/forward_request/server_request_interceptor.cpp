/**
 * @file    server_request_interceptor.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the server request interceptor for the
 *          PortableInterceptor::ForwardRequest test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "server_request_interceptor.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/orb_constants.h"

#include "testlib/taox11_testlog.h"

void
Server_Request_Interceptor::forward_references (
  IDL::traits<CORBA::Object>::ref_type obj1,
  IDL::traits<CORBA::Object>::ref_type obj2)
{
  if (!obj1 || !obj2)
    throw CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);

  this->obj_[0] = std::move(obj1);
  this->obj_[1] = std::move(obj2);
}

std::string
Server_Request_Interceptor::name ()
{
  return "Server_Request_Interceptor";
}

void
Server_Request_Interceptor::destroy ()
{
  TAOX11_TEST_INFO << "Server_Request_Interceptor::destroy" << std::endl;

  this->obj_[0] = {};
  this->obj_[1] = {};
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  ++this->request_count_;

  bool const response_expected = ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by client request interceptor
  // Request 3 -- forwarded by this interception point

  if (this->request_count_ == 3)
  {
    // The client request interceptor should have already forwarded
    // the request to obj_[1], so we re-forward the request back to
    // obj_[0].

    TAOX11_TEST_INFO << "SERVER - Request " << this->request_count_ << " will be forwarded "
                "to object 1" << std::endl <<        // "object 1" as in "obj_[0]"
                "SERVER - via receive_request_service_contexts()" << std::endl;

    throw PortableInterceptor::ForwardRequest (this->obj_[0]);
  }
}

void
Server_Request_Interceptor::receive_request (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  bool const response_expected =
    ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  if (ri->server_id() != "testserver")
  {
    TAOX11_TEST_ERROR << "SERVER - Request " << (this->request_count_-1) << " invalid server id <"
                      << ri->server_id() << ">" << std::endl;

  }

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by client request interceptor
  // Request 3 -- forwarded by receive_request_service_contexts()
  // Request 4 -- non-forwarded (give client chance to print result)
  // Request 5 -- forwarded by this interception point
  // Request 6 -- non-forwarded (request 5 gets forwarded here)
  // Request 7 -- throw exception to initiate forwarding from sent_exception

  if (this->request_count_ == 5)
  {
    // This interceptor should have already forwarded the request to
    // obj_[0] so re-forward it to obj_[1].  This will be the last
    // location forward.

    TAOX11_TEST_INFO << "SERVER - Request " << (this->request_count_-1) << " will be forwarded "
                "to object 2" << std::endl <<        // "object 1" as in "obj_[0]"
                "SERVER - via receive_request()" << std::endl;
    // "request_count_ - 1" is used above since there was a location
    // forward.

    throw PortableInterceptor::ForwardRequest (this->obj_[1]);
  }

  if (this->request_count_ == 7)
  {
    // Throw an exception to force the invocation of send_exception.
    TAOX11_TEST_INFO << "SERVER - OBJ_NOT_EXIST exception thrown for request "
                << (this->request_count_-2) << std::endl <<
                "SERVER - via receive_request()" << std::endl;

    throw CORBA::OBJECT_NOT_EXIST (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);
  }
}

void
Server_Request_Interceptor::send_reply (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type)
{
}

void
Server_Request_Interceptor::send_exception (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  CORBA::Any exception = ri->sending_exception ();
  IDL::traits<CORBA::TypeCode>::ref_type tc = exception.type ();
  std::string const id = tc->id ();

  CORBA::OBJECT_NOT_EXIST nonexist_exception;

  if (id == nonexist_exception._rep_id ())
  {
    TAOX11_TEST_INFO << "SERVER - OBJ_NOT_EXIST exception caught for request " <<
                (this->request_count_ - 2) << std::endl <<
                "SERVER - will be forwarded to object 1 via send_exception()" << std::endl;

    throw PortableInterceptor::ForwardRequest (this->obj_[0]);
  }
}

void
Server_Request_Interceptor::send_other (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  bool const response_expected =
    ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  // If we get this far then we should have received a
  // LOCATION_FORWARD reply.

  // This will throw an exception if a location forward has not
  // occurred.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  IDL::traits<CORBA::Object>::ref_type forward = ri->forward_reference ();

  if (!forward)
    throw CORBA::INTERNAL ();
}
