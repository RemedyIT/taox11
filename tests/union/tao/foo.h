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
  virtual bool pass_union (const Test::Data & s);
  virtual bool pass_default_union (const Test::DefaultData & dd);

  virtual Test::Data * return_union ();
  virtual Test::DefaultData return_default_union ();
  virtual Test::X_Union * return_x_union ();
  virtual Test::Y_Union * return_y_union ();
  virtual Test::Z_Union * return_z_union (const Test::Z_Union & z);

  virtual bool get_union (Test::Data_out s);
  virtual bool get_default_union (Test::DefaultData_out dd);

  virtual bool update_union (Test::Data & s);
  virtual bool update_default_union (Test::DefaultData & dd);

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Data s_;
  Test::DefaultData dd_;
};

#endif /* FOO_H */
