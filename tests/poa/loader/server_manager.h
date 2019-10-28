/**
 * @file    server_manager.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include "servant_activator.h"
#include "servant_locator.h"

/**
 * @class Server_i
 *
 * @brief This class provides the server application with helper methods
 *
 * The various methods required by the server application for
 * utilizing the ServantActivator and ServantLocator interfaces of
 * the Servant Manager are implemented by this class.  These
 * include initialisation procedures of the ServantActivator and
 * ServantLocator, and creation of POAs with emphasis on the
 * servant retention policy.
 */
class Server_i
{
public:
  // = Initialization and termination methods.
  /// Initialisation.
  Server_i ();

  /// Destruction.
  ~Server_i ();

  /// Initialisation of the ORB and poa.
  int init (int argc, char **argv);

  /**
   * This method creates a POA from the root_poa with emphasis being
   * on the servant_retention_policy which decides the use of the
   * ServantActivator or ServantLocator interfaces. The
   * servent_retention_policy value is 1 for the RETAIN policy and 0
   * for the NONRETAIN policy.
   */
  IDL::traits<PortableServer::POA>::ref_type
    create_poa (const std::string& name,
                bool servant_retention_policy);

  /// A ServantActivator object is created and initialised.
  int create_activator (IDL::traits<PortableServer::POA>::ref_type first_poa);

  /// A ServantActivator object is created and initialised.
  int create_locator (IDL::traits<PortableServer::POA>::ref_type second_poa);

  /// The server is executed.
  int run ();

private:
  /// Parses the input arguments.
  bool parse_args (int argc, char **argv);

  /// The IORs are written to a file for future use.
  int write_iors_to_file (const std::string& first_ior,
                          const std::string& second_ior);

  /// Default ior file.
  std::string ior_output_file_;

  /// The orb pointer.
  IDL::traits<CORBA::ORB>::ref_type orb_;

  /// The poa policicies.
  CORBA::PolicyList policies_;

  /// The root_poa which is used for cretaing different child poas.
  IDL::traits<PortableServer::POA>::ref_type root_poa_;

  /// The poa_manager object.
  IDL::traits<PortableServer::POAManager>::ref_type poa_manager_;

  /// The object pointer used by the ServantActivator.
  IDL::traits<CORBA::Object>::ref_type first_test_;

  /// The object pointer used by the Servant Locator.
  IDL::traits<CORBA::Object>::ref_type second_test_;
};

#endif /* SERVER_MANAGER_H */
