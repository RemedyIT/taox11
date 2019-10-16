/**
 * @file    foo.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

class Bar
  : public virtual CORBA::servant_traits<Test::Bar>::base_type
{
public:
  /// Constructor
  Bar () = default;

  virtual void do_something () override;

  virtual void a_bar (int32_t) override;

  virtual int32_t a_bar () override;
};

class Foo
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<Test::Bar>::ref_type bar);

  // = The skeleton methods
  virtual void do_it () override;

  virtual int32_t a_number () override;
  virtual int32_t a_number_and_message () override;


  virtual std::string a_string () override;
  virtual void a_string(const std::string& _v) override;

  virtual void do_reason () override;

  virtual void do_throw_system () override;

  virtual void do_throw_object () override;

  virtual void do_throw_struct () override;

  virtual void do_throw_structs () override;

  virtual void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<Test::Bar>::ref_type bar_;
};

#endif /* FOO_H */
