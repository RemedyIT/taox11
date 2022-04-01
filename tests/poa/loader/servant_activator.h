/**
 * @file    servant_activator.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/orb.h"

#include "tao/x11/portable_server/ServantActivatorC.h"

#include "ace/DLL.h"

/**
 * Servant Activator for the test servant.
 *
 * This class associates an unassociated servant with an object in
 * the POA Active Object Map.
 */
class ServantActivator :
  public virtual IDL::traits<PortableServer::ServantActivator>::base_type
{
public:
  /**
   * This typedef is used to typecast the void* obtained when finding
   * a symbol in the dll. Invoking the function pointer obtained would
   * get a servant.
   */
  typedef void
           (*SERVANT_FACTORY) (const PortableServer::ObjectId &oid,
                               IDL::traits<PortableServer::POA>::ref_type poa,
                               IDL::traits<CORBA::ORB>::ref_type orb,
                               CORBA::servant_reference<PortableServer::Servant>& svt);

  /// Initialization.
  ServantActivator (IDL::traits<CORBA::ORB>::ref_type orb,
                    const std::string& dllname,
                    const std::string& factory_function);

  ~ServantActivator() override;

  /**
   * This method is invoked by a POA with USE_SERVANT_MANAGER and
   * RETAIN policies, whenever it receives a request for a
   * test object that is not currently active. When an servant
   * pointer corresponding to objectId is not found in the Active
   * Object Map, the POA hands over the job of obtaining the servant
   * to the Servant Manager. Depending upon whether the POA is created
   * with RETAIN or NON_RETAIN as the servant_retention policy, the
   * Servant Activator or the Servant Locator interface is invoked
   * respectively.
   */
  CORBA::servant_reference<PortableServer::Servant>
    incarnate (const PortableServer::ObjectId &oid,
              IDL::traits<PortableServer::POA>::ref_type poa) override;

  /**
   * This method is invoked whenever a test object is
   * deactivated. This occurs when the POA is destroyed or the Object
   * is deactivated. When the POA is getting destroyed, it needs to
   * deactivate every object in the Active Object Map and on that call
   * the ServantActivator invokes this method which will destroy the
   * servant associated with the object.
   */
  void etherealize (const PortableServer::ObjectId &oid,
                    IDL::traits<PortableServer::POA>::ref_type adapter,
                    CORBA::servant_reference<PortableServer::Servant> servant,
                    bool cleanup_in_progress,
                    bool remaining_activations) override;

private:
  /// The ACE_DLL object which performs the task of loading the dll
  /// and accessing it.
  ACE_DLL dll_;

  /// The function pointer of factory_function type.
  SERVANT_FACTORY servant_supplier_;

  /// A reference to the ORB.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int result_;
};

#endif /* SERVANT_ACTIVATOR_H */
