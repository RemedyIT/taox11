// -*- C++ -*-
/**
 * @file test_impl.cpp
 * @author Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
//@@{__RIDL_REGEN_MARKER__} - HEADER_END : test_impl.cpp[Header]

#include "test_impl.h"
//@@{__RIDL_REGEN_MARKER__} - BEGIN : test_impl.cpp[User Includes]
#include "testlib/taox11_testlog.h"
//@@{__RIDL_REGEN_MARKER__} - END : test_impl.cpp[User Includes]

// generated from ImplSourceWriter#enter_module
namespace Test
{

  // generated from ImplSourceWriter#enter_interface

  // generated from c++11/templates/impl/src/interface_pre
  namespace _impl {

    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[Constructors]
    // your constructor(s)
    Foo::Foo ()
      : CORBA::servant_traits< ::Test::Foo>::base_type ()
    {
    }
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[Constructors]

    Foo::~Foo ()
    {
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[Dtor Body]
      // your destructor body
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[Dtor Body]
    }

    // generated from c++11/templates/impl/src/interface_post
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[User Public]
    // your public implementations
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[User Public]

    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[User Protected]
    // your protected implementations
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[User Protected]

    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[User Private]
    // your private implementations
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[User Private]

  } // namespace _impl


  // generated from ImplSourceWriter#enter_interface

  // generated from c++11/templates/impl/src/interface_pre
  namespace _impl {

    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[Constructors]
    Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb)
      : CORBA::servant_traits< ::Test::Hello>::base_type ()
      , orb_ (std::move(orb))
    {
    }
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[Constructors]

    Hello::~Hello ()
    {
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[Dtor Body]
      TAOX11_TEST_INFO << "Hello::~Hello()" << std::endl;
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[Dtor Body]
    }

  // generated from c++11/templates/impl/src/operation
  std::string
  Hello::get_string ()
  {
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello::get_string[Body]
      TAOX11_TEST_INFO << "Hello::get_string()" << std::endl;
      return "Hello there!";
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello::get_string[Body]
  }

  // generated from c++11/templates/impl/src/operation
  void
  Hello::create_foo (
      IDL::traits<::Test::Foo>::ref_type& foo_inst)
  {
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello::create_foo[Body]
      // your code here
      X11_UNUSED_ARG (foo_inst);
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello::create_foo[Body]
  }

  // generated from c++11/templates/impl/src/operation
  ::Test::Foo::S
  Hello::update (
      const ::Test::Foo::S& s)
  {
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello::update[Body]
      // your code here
      X11_UNUSED_ARG (s);
      return ::Test::Foo::S ();
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello::update[Body]
  }

  // generated from c++11/templates/impl/src/operation
  void
  Hello::shutdown ()
  {
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello::shutdown[Body]
      this->orb_->shutdown (false);
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello::shutdown[Body]
  }

  // generated from c++11/templates/impl/src/attribute
  int32_t
  Hello::int_prop ()
  {
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello::int_prop[Getter Body]
      // your code here
      return 0;
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello::int_prop[Getter Body]
  }
  void
  Hello::int_prop (
      int32_t _v)
  {
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello::int_prop[Setter Body]
      // your code here
      X11_UNUSED_ARG (_v);
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello::int_prop[Setter Body]
  }

    // generated from c++11/templates/impl/src/interface_post
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[User Public]
    // your public implementations
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[User Public]

    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[User Protected]
    // your protected implementations
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[User Protected]

    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[User Private]
    // your private implementations
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[User Private]

  } // namespace _impl

} // namespace Test


//@@{__RIDL_REGEN_MARKER__} - BEGIN : test_impl.cpp[Footer]
// Your footer here.
// -*- END -*-
