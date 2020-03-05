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
  CORBA::Boolean pass_struct (const Test::Simple & s) override;

  Test::Simple * return_struct () override;

  CORBA::Boolean get_struct (Test::Simple_out s) override;

  CORBA::Boolean update_struct (Test::Simple & s) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Simple s_;
};

#endif /* FOO_H */
