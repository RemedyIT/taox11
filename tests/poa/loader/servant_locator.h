/**
 * @file    servant_locator.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SERVANT_LOCATOR_H
#define SERVANT_LOCATOR_H

#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/ServantLocatorC.h"

#include "tao/x11/orb.h"
#include "ace/DLL.h"

// This class is used by a POA with USE_SERVANT_MANAGER and
// NON_RETAIN policy.
//
// This class defines the Servant Locator interface of the Servant
// Manager. It is invoked when the POA has an USE_SERVANT_MANAGER
// policy and a servant_retention policy of NON_RETAIN type.
class ServantLocator :
  public virtual IDL::traits<PortableServer::ServantLocator>::base_type
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

  /// Constructor.
  ServantLocator (IDL::traits<CORBA::ORB>::ref_type orb,
                  const std::string& dllname,
                  const std::string& factory_function);

  virtual ~ServantLocator();

  /**
   * This method is invoked by a POA whenever it receives a request
   * for test object that is not currently active.  When the POA is
   * created using the NON_RETAIN policy the Active Object Map is not
   * maintained, in other words, an association between the ObjectId
   * and the servant is not maintained. Hence every client request the
   * servant has to be loaded. Note the operation argument. This
   * argument specifies the operation to be invoked on the
   * servant. The cookie helps in marking the servant. This marking is
   * useful while destroying the servant.
   */
  CORBA::servant_reference<PortableServer::Servant>
  preinvoke (const PortableServer::ObjectId &oid,
            IDL::traits<PortableServer::POA>::ref_type adapter,
            const CORBA::Identifier& operation,
            PortableServer::ServantLocator::Cookie &the_cookie) override;

  /**
   * This method is invoked whenever a test servant completes a
   * request. As the Servant Loactor interface is used when the POA
   * doesnt maintain the Active Object Map, its necessary to get rid
   * of the servant after the client request has been processed. The
   * appropriate servant is destroyed by verifying the cookie.Again
   * this method is invoked per client request.
   */
  void postinvoke (const PortableServer::ObjectId &oid,
                   IDL::traits<PortableServer::POA>::ref_type adapter,
                   const CORBA::Identifier& operation,
                   PortableServer::ServantLocator::Cookie the_cookie,
                   CORBA::servant_reference<PortableServer::Servant> the_servant) override;

private:
  /// The name of the dll containing the servant.
  std::string dllname_;

  /// The symbol which on getting invoked will give us the servant
  /// pointer.
  std::string create_symbol_;

  /// The ACE_DLL object which performs the task of loading the dll
  /// and accessing it.
  ACE_DLL dll_;

  /// The function pointer of factory_function type.
  SERVANT_FACTORY servant_supplier_;

  /// A reference to the ORB.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  int result_;
};

#endif /* SERVANT_LOCATOR_H */
