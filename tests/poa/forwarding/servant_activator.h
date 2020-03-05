/**
 * @file    servant_activator.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA forwarding test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/x11/orb.h"
#include "tao/x11/portable_server/ServantActivatorC.h"

class ServantActivator
  : public IDL::traits<PortableServer::ServantActivator>::base_type
{
public:
  ServantActivator (IDL::traits<CORBA::ORB>::ref_type orb,
                    IDL::traits<CORBA::Object>::ref_type forward_to);

  ~ServantActivator();

  CORBA::servant_reference<PortableServer::Servant> incarnate (
    const PortableServer::ObjectId& oid,
    IDL::traits<PortableServer::POA>::ref_type adapter) override;

  void etherealize (
    const PortableServer::ObjectId& oid,
    IDL::traits<PortableServer::POA>::ref_type adapter,
    CORBA::servant_reference<PortableServer::Servant> serv,
    bool cleanup_in_progress,
    bool remaining_activations) override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;

  /// Location to forward to
  IDL::traits<CORBA::Object>::ref_type forward_to_;
};

#endif /* SERVANT_ACTIVATOR_H */
