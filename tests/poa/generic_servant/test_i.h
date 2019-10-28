/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testS.h"

class GENERIC_SERVANT_Export test_i
  : public virtual CORBA::servant_traits<test>::base_type
{
public:
  /// Constructor - takes a POA and a value parameter
  test_i (IDL::traits<CORBA::ORB>::ref_type orb,
          IDL::traits<PortableServer::POA>::ref_type poa);

  virtual ~test_i();
  /// Returns the Default POA of this servant
  IDL::traits<PortableServer::POA>::ref_type _default_POA ();

  /// A twoway operation.
  void method ();

  /// A oneway operation.
  void oneway_method ();

  /// A timed twoway operation.
  void timed_method (uint32_t timeout);

  /// A timed twoway operation.
  void timed_oneway_method (uint32_t timeout);

  /// Shutdown the ORB
  void shutdown ();

  /// Sleep for some time.
  void sleep (unsigned long timeout,
              const std::string operation);

protected:
  /// Our ORB.
  IDL::traits<CORBA::ORB>::ref_type orb_;

  /// Our POA.
  IDL::traits<PortableServer::POA>::ref_type poa_;
};
