/**
 * @file    test_i.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testS.h"
#include "tao/x11/portable_server/ServantActivatorC.h"

class test_i :
  public CORBA::servant_traits<test>::base_type
{
public:
  test_i (IDL::traits<CORBA::ORB>::ref_type orb,
          IDL::traits<PortableServer::POA>::ref_type poa,
          IDL::traits<PortableServer::ServantActivator>::weak_ref_type activator,
          int32_t value);
  virtual ~test_i ();

  int32_t doit () override;

  // Setup forwarding
  void forward () override;

  void shutdown () override;

  /// Returns the default POA for this servant.
  IDL::traits<PortableServer::POA>::ref_type
    _default_POA () override;

protected:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
  IDL::traits<PortableServer::ServantActivator>::weak_ref_type activator_;
  int32_t value_;
};

