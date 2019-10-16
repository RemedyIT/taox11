/**
 * @file    locala.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef LOCALA_H_
#define LOCALA_H_

#include "testS.h"

class iLocal_A_impl: public IDL::traits<Test::iLocal_A>::base_type
{
public:
  iLocal_A_impl () = default;
  virtual ~iLocal_A_impl ();
  virtual std::string do_local () override;
  virtual IDL::traits<Test::iLocal>::ref_type loc () override;
  virtual void loc (IDL::traits<Test::iLocal>::ref_type _v) override;
  virtual std::string do_A(int32_t& result) override;
};

class iLocal_B_impl: public IDL::traits<Test::iLocal_B>::base_type
{
public:
  iLocal_B_impl () = default;
  virtual ~iLocal_B_impl ();
  virtual std::string do_localB () override;
  virtual std::string do_B(int32_t& result) override;
};

class Foo_impl: public IDL::traits<Test::Foo>::base_type
{
public:
  Foo_impl (IDL::traits<Test::FooFactory>::ref_type);
  virtual ~Foo_impl ();
  virtual std::string get_foo () override;
  virtual IDL::traits<Test::FooFactory>::ref_type get_factory () override;
private:
  IDL::traits<Test::FooFactory>::ref_type factory_;
};

class Foo_Factory : public IDL::traits<Test::FooFactory>::base_type
{
public:
  Foo_Factory () = default;
  virtual ~Foo_Factory ();
  virtual std::string factory_name () override;
  virtual IDL::traits<Test::Foo>::ref_type get_foo () override;
};

#endif /* LOCALA_H_ */
