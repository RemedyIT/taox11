/**
 * @file    client_request_interceptor2.h
 * @author  Martin Corino
 *
 * @brief   PICurrentTest server-side ClientRequestInterceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef CLIENT_REQUEST_INTERCEPTOR2_H
#define CLIENT_REQUEST_INTERCEPTOR2_H

#include "tao/x11/pi/pi.h"
#include "tao/x11/PortableInterceptorC.h"

/**
 * @class ClientRequestInterceptor2
 *
 * @brief Simple concrete client request interceptor.
 *
 * This is a test client request interceptor that verifies that is
 * used when verifying that the RSC->TSC->RSC copying semantics work
 * as expected.
 */
class ClientRequestInterceptor2
  : public IDL::traits<PortableInterceptor::ClientRequestInterceptor>::base_type
{
public:

  /// Constructor.
  ClientRequestInterceptor2 (PortableInterceptor::SlotId slot_id);

  /**
   * @name Methods Required by the Client Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all client request interceptors.
   */
  //@{
  /// Return the name of this ClientRequestInterceptor.
  virtual std::string name () override;

  virtual void destroy () override;

  virtual void send_request (
      IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  virtual void send_poll (
      IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  virtual void receive_reply (
      IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  virtual void receive_exception (
      IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  virtual void receive_other (
      IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;
  //@}

private:

  /// The PICurrent slot ID allocated to this application during ORB
  /// initialization.
  PortableInterceptor::SlotId slot_id_;

};

#endif  /* CLIENT_REQUEST_INTERCEPTOR2_H */
