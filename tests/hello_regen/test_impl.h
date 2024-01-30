// -*- C++ -*-
/**
 * @file test_impl.h
 * @author Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
//@@{__RIDL_REGEN_MARKER__} - HEADER_END : test_impl.h[Header]

#ifndef __RIDL_TEST_IMPL_H_INCLUDED__
#define __RIDL_TEST_IMPL_H_INCLUDED__

#include "testS.h"

#pragma once

//@@{__RIDL_REGEN_MARKER__} - BEGIN : test_impl.h[User Includes]
// your includes here
//@@{__RIDL_REGEN_MARKER__} - END : test_impl.h[User Includes]

// generated from ImplHeaderWriter#enter_module
namespace Test
{

  // generated from c++11/templates/impl/hdr/interface_pre
  namespace _impl
  {
    class Foo
      : public CORBA::servant_traits<::Test::Foo>::base_type
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[Base List]
    // your base classes
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[Base List]
    {
    public:
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[Constructors]
      /// Constructor(s)
      Foo ();
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[Constructors]

      /// Destructor
      ~Foo () override;

      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[User Protected]
      // your protected definitions
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[User Protected]


      // generated from c++11/templates/impl/hdr/interface_post
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[User Public]
      // your public definitions
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[User Public]

    private:
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Foo[User Private]
      // your private definitions
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Foo[User Private]

      Foo (const Foo&) = delete;
      Foo (Foo&&) = delete;
      Foo& operator= (const Foo&) = delete;
      Foo& operator= (Foo&&) = delete;
    };

  }; // _impl

  // generated from c++11/templates/impl/hdr/interface_pre
  namespace _impl
  {
    class Hello
      : public CORBA::servant_traits<::Test::Hello>::base_type
    //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[Base List]
    // your base classes
    //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[Base List]
    {
    public:
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[Constructors]
      /// Constructor(s)
      Hello (IDL::traits<CORBA::ORB>::ref_type orb);
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[Constructors]

      /// Destructor
      ~Hello () override;

      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[User Protected]
      // your protected definitions
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[User Protected]


      // generated from c++11/templates/impl/hdr/attribute
      int32_t
      int_prop () override;

      void int_prop (int32_t _v) override;

      // generated from c++11/templates/impl/hdr/operation
      /// @copydoc test.idl::Test::Hello::get_string
      std::string
      get_string () override;

      // generated from c++11/templates/impl/hdr/operation
      /// @copydoc test.idl::Test::Hello::create_foo
      void
      create_foo (
          IDL::traits<::Test::Foo>::ref_type& foo_inst) override;

      // generated from c++11/templates/impl/hdr/operation
      /// @copydoc test.idl::Test::Hello::update
      ::Test::Foo::S
      update (
          const ::Test::Foo::S& s) override;

      // generated from c++11/templates/impl/hdr/operation
      /// @copydoc test.idl::Test::Hello::shutdown
      void
      shutdown () override;

      // generated from c++11/templates/impl/hdr/interface_post
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[User Public]
      // your public definitions
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[User Public]

    private:
      //@@{__RIDL_REGEN_MARKER__} - BEGIN : Test::Hello[User Private]
      /// Use an ORB reference to convert strings to objects and shutdown
      /// the application.
      IDL::traits<CORBA::ORB>::ref_type orb_;
      //@@{__RIDL_REGEN_MARKER__} - END : Test::Hello[User Private]

      Hello (const Hello&) = delete;
      Hello (Hello&&) = delete;
      Hello& operator= (const Hello&) = delete;
      Hello& operator= (Hello&&) = delete;
    };

  }; // _impl
} // namespace Test


#endif /* __RIDL_TEST_IMPL_H_INCLUDED__ */
//@@{__RIDL_REGEN_MARKER__} - BEGIN : test_impl.h[Footer]
// Your footer here.
// -*- END -*-
