/**
 * @file    foo.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

class Bar : public virtual CORBA::servant_traits<Test::Bar>::base_type
{
public:
  /// Constructor
  Bar () = default;

  void do_something () override;

  void a_bar (int32_t) override;
  int32_t a_bar () override;
};

class Foo
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<Test::Bar>::ref_type bar);

  // = The skeleton methods
  void do_it () override;

  int32_t a_number () override;
  int32_t a_number_and_message () override;

  std::string a_string () override;
  void a_string(const std::string& _v) override;

  void do_reason () override;

  void do_throw_system () override;

  void do_throw_object () override;

  void do_throw_struct () override;

  void do_throw_structs () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<Test::Bar>::ref_type bar_;
};

#endif /* FOO_H */
