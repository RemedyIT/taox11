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
  virtual Test::Hello_ptr get_hello ();

  virtual void get_hello_2 (Test::Hello_out hello_obj);

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
  virtual char * get_string ();

  virtual ::Test::Hello_Factory_ptr get_factory ();

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Hello_Factory_var hf_;
};

#endif /* FOO_H */
