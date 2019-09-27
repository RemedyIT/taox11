#ifndef FOO_H
#define FOO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Foo
  : public virtual POA_Test::Foo
{
public:
  /// Constructor
  Foo (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::Boolean pass_struct (const Test::Simple & s);

  virtual Test::Simple * return_struct ();

  virtual CORBA::Boolean get_struct (Test::Simple_out s);

  virtual CORBA::Boolean update_struct (Test::Simple & s);

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Simple s_;
};

#endif /* FOO_H */
