/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"
#include "tao/x11/orb.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb) :
  orb_(orb)
{
}

// array

// array of enums
Test::enum_array Hello::inout_enum_array(const Test::enum_array& v_in,
    Test::enum_array& v_out, Test::enum_array& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
Test::enum_array mv_enum_array;
Test::enum_array Hello::at_enum_array()
{
  return mv_enum_array;
}
void Hello::at_enum_array(const Test::enum_array& _v)
{
  mv_enum_array = _v;
}
// array of arrays
Test::array_array Hello::inout_array_array(const Test::array_array& v_in,
    Test::array_array& v_out, Test::array_array& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
Test::array_array mv_array_array;
Test::array_array Hello::at_array_array()
{
  return mv_array_array;
}
void Hello::at_array_array(const Test::array_array& _v)
{
  mv_array_array = _v;
}
// array of sequences
Test::sequence_array Hello::inout_sequence_array(
    const Test::sequence_array& v_in, Test::sequence_array& v_out,
    Test::sequence_array& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
// array of structs
Test::struct_array Hello::inout_struct_array(const Test::struct_array& v_in,
    Test::struct_array& v_out, Test::struct_array& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
// array of unions
Test::union_array Hello::inout_union_array(const Test::union_array& v_in,
    Test::union_array& v_out, Test::union_array& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}

// sequence

// sequence of enums
Test::enum_sequence Hello::inout_enum_sequence(const Test::enum_sequence& v_in,
    Test::enum_sequence& v_out, Test::enum_sequence& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
// sequence of arrays
Test::array_sequence Hello::inout_array_sequence(
    const Test::array_sequence& v_in, Test::array_sequence& v_out,
    Test::array_sequence& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
// sequence of sequences
Test::sequence_sequence Hello::inout_sequence_sequence(
    const Test::sequence_sequence& v_in, Test::sequence_sequence& v_out,
    Test::sequence_sequence& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
// sequence of structs
Test::struct_sequence Hello::inout_struct_sequence(
    const Test::struct_sequence& v_in, Test::struct_sequence& v_out,
    Test::struct_sequence& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
// sequence of unions
Test::union_sequence Hello::inout_union_sequence(
    const Test::union_sequence& v_in, Test::union_sequence& v_out,
    Test::union_sequence& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}

// struct of complex types

Test::union_struct Hello::inout_union_struct(const Test::union_struct& v_in,
    Test::union_struct& v_out, Test::union_struct& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
Test::all_struct Hello::inout_all_struct(const Test::all_struct& v_in,
    Test::all_struct& v_out, Test::all_struct& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}

// union of complex types

Test::union_union Hello::inout_union_union(const Test::union_union& v_in,
    Test::union_union& v_out, Test::union_union& v_inout)
{
  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}
Test::all_union Hello::inout_all_union(const Test::all_union& v_in,
    Test::all_union& v_out, Test::all_union& v_inout)
{
  // Compile test for ostringstream write
  std::ostringstream ss;
  ss << IDL::traits<Test::all_union>::write (v_in) << std::endl;

  v_out = v_inout;
  v_inout = v_in;
  return v_in;
}

// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
