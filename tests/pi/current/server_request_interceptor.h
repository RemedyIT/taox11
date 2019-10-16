/**
 * @file    server_request_interceptor.h
 * @author  Martin Corino
 *
 * @brief   PICurrentTest server-side ServerRequestInterceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SERVER_REQUEST_INTERCEPTOR_H
#define SERVER_REQUEST_INTERCEPTOR_H

#include "tao/x11/pi_server/pi_server.h"
#include "tao/x11/PortableInterceptorC.h"

/**
 * @class ServerRequestInterceptor
 *
 * @brief Simple concrete server request interceptor.
 *
 * This server request interceptor
 */
class ServerRequestInterceptor
  : public IDL::traits<PortableInterceptor::ServerRequestInterceptor>::base_type
{
public:

  /// Constructor.
  ServerRequestInterceptor (PortableInterceptor::SlotId id,
                            IDL::traits<PortableInterceptor::Current>::ref_type pi_current);

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

private:

  /// The PICurrent slot ID allocated to this application during ORB
  /// initialization.
  PortableInterceptor::SlotId slot_id_;

  /// Reference to the PICurrent object.
  IDL::traits<PortableInterceptor::Current>::ref_type pi_current_;

};

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
