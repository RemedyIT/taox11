/**
 * @file    client_request_interceptor.h
 * @author  Martin Corino
 *
 * @brief   PICurrentTest client-side ClientRequestInterceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef CLIENT_REQUEST_INTERCEPTOR_H
#define CLIENT_REQUEST_INTERCEPTOR_H

#include "tao/x11/pi/pi.h"
#include "tao/x11/PortableInterceptorC.h"

/**
 * @class ClientRequestInterceptor
 *
 * @brief Simple concrete client request interceptor.
 *
 * This is a test client request interceptor that verifies that the
 * client side PICurrent semantics are correct.  In particular, it
 * verifies that the request-scope current (RSC) is not modified after
 * the contents of the thread-scope current (TSC) are copied to it.
 * This is particularly important for ORBs that initially perform a
 * logical copy for performance reasons.
 */
class ClientRequestInterceptor
  : public IDL::traits<PortableInterceptor::ClientRequestInterceptor>::base_type
{
public:
  /// Constructor.
  ClientRequestInterceptor (PortableInterceptor::SlotId slot_id,
                            IDL::traits<PortableInterceptor::Current>::ref_type pi_current);

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
  std::string name () override;

  void destroy () override;

  void send_request (IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  void send_poll (IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  void receive_reply (IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  void receive_exception (IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;

  void receive_other (IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri) override;
  //@}

private:
  /// The PICurrent slot ID allocated to this application during ORB
  /// initialization.
  PortableInterceptor::SlotId slot_id_;

  /// Reference to the PICurrent object.
  IDL::traits<PortableInterceptor::Current>::ref_type pi_current_;
};

#endif  /* CLIENT_REQUEST_INTERCEPTOR_H */
