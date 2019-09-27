/**
 * @file    foo_client_request_interceptor.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" client request test
 *          interceptor.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef FOO_CLIENT_REQUEST_INTERCEPTOR_H
#define FOO_CLIENT_REQUEST_INTERCEPTOR_H

#include "tao/x11/pi/pi.h"

/**
 * @class FOO_ClientRequestInterceptor
 *
 * @brief Simple concrete client request interceptor.
 *
 * This is a test client request interceptor that extracts a tagged
 * component that should have been embedded in the server IOR by an
 * IOR interceptor.  It displays the contents of that tagged
 * component.
 */
class FOO_ClientRequestInterceptor
  : public IDL::traits<PortableInterceptor::ClientRequestInterceptor>::base_type
{
public:

  /// Constructor
  FOO_ClientRequestInterceptor (IDL::traits<IOP::Codec>::ref_type codec);

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

  /// The CDR encapsulation coder/decoder used to retrieve data
  /// from the CDR encapsulation in the tagged component.
  IDL::traits<IOP::Codec>::ref_type codec_;

};

#endif  /* FOO_CLIENT_REQUEST_INTERCEPTOR_H */
