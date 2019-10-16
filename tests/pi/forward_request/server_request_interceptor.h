/**
 * @file    server_request_interceptor.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the server request interceptor for the
 *          PortableInterceptor::ForwardRequest test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SERVER_REQUEST_INTERCEPTOR_H
#define SERVER_REQUEST_INTERCEPTOR_H

#include "sri_testC.h"
#include "tao/x11/pi_server/pi_server.h"

/**
 * @class Server_Request_Interceptor
 *
 * @brief Simple concrete server request interceptor.
 *
 * This server request interceptor forwards an initial client request
 * to the primary server since the client request interceptor should
 * have attempted to forward a request to the secondary server.  That
 * request will then be forwarded back to the primary server, at which
 * point the request will be handled.
 *
 * The PortableInterceptor::ForwardRequest code for the
 * receive_request_service_contexts() interception point (which is not
 * in the servant skeleton) is slightly different from the
 * corresponding code in the server skeleton.  Hence,
 * PortableInterceptor::ForwardRequest support is tested for both
 * cases by throwing that exception in the
 * receive_request_service_contexts() and the receive_request()
 * interception points.
 *
 * See the README file for test details.
 */
class Server_Request_Interceptor final
  : public IDL::traits<ForwardRequestTest::ServerRequestInterceptor>::base_type
{
public:
  /// Constructor.
  Server_Request_Interceptor () = default;

  /// Set the references to which requests will be forwarded.
  virtual void forward_references (IDL::traits<CORBA::Object>::ref_type obj1,
                                   IDL::traits<CORBA::Object>::ref_type obj2) override;

  /**
   * @name Methods Required by the Server Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all server request interceptors.
   */
  //@{
  /// Return the name of this ServerRequestinterceptor.
  virtual std::string name () override;

  virtual void destroy () override;

  virtual void receive_request_service_contexts (
      IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri) override;

  virtual void receive_request (
      IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri) override;

  virtual void send_reply (
      IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri) override;

  virtual void send_exception (
      IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri) override;

  virtual void send_other (
      IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri) override;
  //@}

protected:

  /// Destructor.
  virtual ~Server_Request_Interceptor () = default;

private:

  /// The number of requests intercepted by this interceptor.
  uint32_t request_count_ {};

  /// References to the two objects used in this test.
  IDL::traits<CORBA::Object>::ref_type obj_[2];

};

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
