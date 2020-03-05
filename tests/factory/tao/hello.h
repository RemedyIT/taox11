#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello_Factory
  : public virtual POA_Test::Hello_Factory
{
public:
  /// Constructor
  Hello_Factory (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  // = The skeleton methods
  Test::Hello_ptr get_hello () override;

  void get_hello_2 (Test::Hello_out hello_obj) override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
};


/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb, Test::Hello_Factory_ptr hf);

  // = The skeleton methods
  char * get_string () override;

  ::Test::Hello_Factory_ptr get_factory () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Hello_Factory_var hf_;
};

#endif /* FOO_H */
