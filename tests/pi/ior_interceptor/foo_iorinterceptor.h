/**
 * @file    foo_iorinterceptor.h
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" IOR test interceptor.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_IOR_INTERCEPTOR_H
#define FOO_IOR_INTERCEPTOR_H

#include "tao/x11/ior_interceptor/ior_interceptor.h"
#include "tao/x11/codecfactory/codecfactory.h"

/**
 * @class FOO_IORInterceptor
 *
 * @brief Simple concrete IOR interceptor.
 *
 * This is a test IOR interceptor that embeds a tagged component in an
 * IOR.  That tagged component will then be verified on the client
 * side to ensure that IOR interceptor support is functioning
 * properly.
 */
class FOO_IORInterceptor
  : public IDL::traits<PortableInterceptor::IORInterceptor_3_0>::base_type
{
public:

  /// Constructor
  FOO_IORInterceptor (IDL::traits<IOP::Codec>::ref_type codec);

  /**
   * @name Methods Required by the IOR Interceptor Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all IOR interceptors.
   */
  //@{
  /// Return the name of this IORInterceptor.
  virtual std::string name () override;

  /// Cleanup resources acquired by this IORInterceptor.
  virtual void destroy () override;

  /// Add the tagged components to the IOR.
  virtual void establish_components (
      IDL::traits<PortableInterceptor::IORInfo>::ref_type info) override;

  virtual void components_established (
      IDL::traits<PortableInterceptor::IORInfo>::ref_type info) override;

  virtual void adapter_manager_state_changed (
      const PortableInterceptor::AdapterManagerId& id,
      PortableInterceptor::AdapterState state) override;

  virtual void adapter_state_changed (
      const PortableInterceptor::ObjectReferenceTemplateSeq & templates,
      PortableInterceptor::AdapterState state) override;

  //@}

private:

  /// The CDR encapsulation coder/decoder used to encapsulate data
  /// into a CDR encapsulation.
  IDL::traits<IOP::Codec>::ref_type codec_;

};

#endif  /* FOO_IOR_INTERCEPTOR_H */
