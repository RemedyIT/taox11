/**
 * @file    receiver_factory.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef THROUGHPUT_RECEIVER_FACTORY_H
#define THROUGHPUT_RECEIVER_FACTORY_H

#include "testS.h"

/// Implement the Test::Receiver_Factory interface
class Receiver_Factory :
    public virtual CORBA::servant_traits<Test::Receiver_Factory>::base_type
{
public:
  /// Constructor
  Receiver_Factory (IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  virtual IDL::traits<Test::Receiver>::ref_type create_receiver () override;

  virtual void shutdown () override;

private:
  /// Keep a reference to the ORB in order to shutdown the app
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* THROUGHPUT_RECEIVER_FACTORY_H */
