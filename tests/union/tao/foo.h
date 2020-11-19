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
  bool pass_union (const Test::Data & s) override;
  bool pass_default_union (const Test::DefaultData & dd) override;

  Test::Data * return_union () override;
  Test::DefaultData return_default_union () override;
  Test::X_Union * return_x_union () override;
  Test::Y_Union * return_y_union () override;
  Test::Z_Union * return_z_union (const Test::Z_Union & z) override;

  bool get_union (Test::Data_out s) override;
  bool get_default_union (Test::DefaultData_out dd) override;

  bool update_union (Test::Data & s);
  bool update_default_union (Test::DefaultData & dd) override;

  bool send_unionmessage (const Test::UnionMessage & msg) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::Data s_;
  Test::DefaultData dd_;
};

#endif /* FOO_H */
