/**
 * @file    foo.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods
  virtual bool pass_union (const Test::Data & s) override;
  virtual bool pass_default_union (const Test::DefaultData & dd) override;

  virtual Test::Data return_union () override;
  virtual Test::DefaultData return_default_union () override;
  virtual Test::X_Union return_x_union () override;
  virtual Test::Y_Union return_y_union () override;
  virtual Test::Z_Union return_z_union (const Test::Z_Union &z) override;

  virtual bool get_union (Test::Data & s) override;
  virtual bool get_default_union (Test::DefaultData & dd) override;

  virtual bool update_union (Test::Data & s) override;
  virtual bool update_default_union (Test::DefaultData & dd) override;

  virtual void shutdown () override;

  uint16_t get_error_count ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  Test::Data s_;
  Test::DefaultData dd_;

  uint16_t errors_;

  Foo (const Foo&) = delete;
  Foo (Foo&&) = delete;
  Foo& operator= (const Foo&) = delete;
  Foo& operator= (Foo&&) = delete;
};

#endif /* FOO_H */
