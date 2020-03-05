/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods for array of complex type.
  Test::enum_array inout_enum_array(const Test::enum_array& v_in,
      Test::enum_array& v_out, Test::enum_array& v_inout) override;
  Test::enum_array at_enum_array() override;
  void at_enum_array(const Test::enum_array& _v) override;
  Test::array_array inout_array_array(const Test::array_array& v_in,
      Test::array_array& v_out, Test::array_array& v_inout) override;
  Test::array_array at_array_array() override;
  void at_array_array(const Test::array_array& _v) override;
  Test::sequence_array inout_sequence_array(const Test::sequence_array& v_in,
      Test::sequence_array& v_out, Test::sequence_array& v_inout) override;
  Test::struct_array inout_struct_array(const Test::struct_array& v_in,
      Test::struct_array& v_out, Test::struct_array& v_inout) override;
  Test::union_array inout_union_array(const Test::union_array& v_in,
      Test::union_array& v_out, Test::union_array& v_inout) override;

  // = The skeleton methods for sequence of complex type.
  Test::enum_sequence inout_enum_sequence(const Test::enum_sequence& v_in,
      Test::enum_sequence& v_out, Test::enum_sequence& v_inout) override;
  Test::array_sequence inout_array_sequence(const Test::array_sequence& v_in,
      Test::array_sequence& v_out, Test::array_sequence& v_inout) override;
  Test::sequence_sequence inout_sequence_sequence(const Test::sequence_sequence& v_in,
      Test::sequence_sequence& v_out, Test::sequence_sequence& v_inout) override;
  Test::struct_sequence inout_struct_sequence(const Test::struct_sequence& v_in,
      Test::struct_sequence& v_out, Test::struct_sequence& v_inout) override;
  Test::union_sequence inout_union_sequence(const Test::union_sequence& v_in,
      Test::union_sequence& v_out, Test::union_sequence& v_inout) override;

  // = The skeleton methods for struct of complex type.
  Test::union_struct inout_union_struct(const Test::union_struct& v_in,
      Test::union_struct& v_out, Test::union_struct& v_inout) override;
  Test::all_struct inout_all_struct(const Test::all_struct& v_in,
      Test::all_struct& v_out, Test::all_struct& v_inout) override;

  // = The skeleton methods for union of complex type.
  Test::union_union inout_union_union(const Test::union_union& v_in,
      Test::union_union& v_out, Test::union_union& v_inout) override;
  Test::all_union inout_all_union(const Test::all_union& v_in,
      Test::all_union& v_out, Test::all_union& v_inout) override;

  void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
