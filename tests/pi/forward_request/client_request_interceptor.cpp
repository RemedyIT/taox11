/**
 * @file    client_request_interceptor.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the client request interceptor for the
 *          PortableInterceptor::ForwardRequest test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "client_request_interceptor.h"
#include "testC.h"
#include "tao/x11/PortableInterceptorC.h"

#include "testlib/taox11_testlog.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  std::string orb_id,
  std::string forward_str)
  : orb_id_ (std::move(orb_id)),
    forward_str_ (std::move(forward_str))
{
}

std::string
Client_Request_Interceptor::name ()
{
  return "Client_Request_Interceptor";
}

void
Client_Request_Interceptor::destroy ()
{
}

void
Client_Request_Interceptor::send_request (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri)
{
  ++this->request_count_;

  bool response_expected =
    ri->response_expected ();

  if (!response_expected)   // A one-way request.
    return;

  // Request 1 -- non-forwarded
  // Request 2 -- forwarded by this interception point.

  if (this->request_count_ == 2)
  {
    if (!this->orb_)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    this->orb_id_);
    }

    IDL::traits<CORBA::Object>::ref_type forward =
      this->orb_->string_to_object (this->forward_str_);

    std::string forward_str =
      this->orb_->object_to_string (forward);

    TAOX11_TEST_INFO << "CLIENT - Request " << this->request_count_ << " will be forwarded " \
                "to object 2" << std::endl <<
                "CLIENT - via send_request()." << std::endl;

    // Notice that this is not a permanent forward.
    throw PortableInterceptor::ForwardRequest (forward);
  }
}

void
Client_Request_Interceptor::send_poll (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
Client_Request_Interceptor::receive_reply (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
Client_Request_Interceptor::receive_exception (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
Client_Request_Interceptor::receive_other (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri)
{

  bool response_expected =
    ri->response_expected ();

  // Oneway
  if (!response_expected)
    return;

  PortableInterceptor::ReplyStatus reply_status =
    ri->reply_status ();

  if (reply_status == PortableInterceptor::TRANSPORT_RETRY)
    return;

  // If we get this far then we should have received a
  // LOCATION_FORWARD reply, either from another client request
  // interceptor (not this one) or from the server.

  // This will throw an exception if a location forward has not
  // occurred.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  IDL::traits<CORBA::Object>::ref_type forward = ri->forward_reference ();

  if (!forward)
    throw CORBA::INTERNAL ();

  TAOX11_TEST_INFO << "CLIENT - Received LOCATION_FORWARD reply." << std::endl;
}
