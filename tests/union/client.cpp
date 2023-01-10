/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client union
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"
#include "checks.h"

uint16_t
test_data_union (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t retval = 0;

  Test::Data data;
  data._default ();
  retval += check_union (data, Test::DataType::dtEmpty, "after default");
  data.pointData (Test::Point (12, 34));
  retval += check_union (data, Test::DataType::dtPoint, "before pass_union");

#if defined (ACE_HAS_CPP17)
  if (!std::is_swappable<Test::Data>())
  {
    TAOX11_TEST_ERROR << "ERROR: V is not swappable." << std::endl;
    return 1;
  }
  else
  {
    TAOX11_TEST_DEBUG << "Test::Data is swappable." << std::endl;
  }
#endif /* ACE_HAS_CPP17 */

  if (foo->pass_union (data))
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::pass_union: <"
      << data << ">" << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "ERROR: pass_union call failed." << std::endl;
    ++retval;
  }

  // Not a copy here but a move!
  Test::Data data2__ = foo->return_union ();
  retval += check_union (data2__, Test::DataType::dtLong, "after return_union");

  if (data2__._d () != Test::DataType::dtLong)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in data2__ found: "
      << "expected <" << Test::DataType::dtLong << "> - found <"
      << data2__._d () << ">" << std::endl;
    ++retval;
  }
  else if (data2__.longData () != 123456)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in data2__: "
      << "expected <123456> - found <" << data2__.longData () << ">"
      << std::endl;
    ++retval;
  }

  Test::Data data2 (std::move (data2__));
  retval += check_union (data2, Test::DataType::dtLong, "after move");

  if (data2._d () != Test::DataType::dtLong)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in data2 found: "
      << "expected <" << Test::DataType::dtLong << "> - found <"
      << data2._d () << ">" << std::endl;
    ++retval;
  }
  else if (data2.longData () != 123456)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in data2: "
      << "expected <123456> - found <" << data2.longData () << ">"
      << std::endl;
    ++retval;
  }
  else
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::return_union: <"
      << data2 << ">" << std::endl;
  }

  Test::Data data3;
  if (foo->get_union (data3))
  {
    retval += check_union (data3, Test::DataType::dtPoint, "after get_union");
    // Check data set by the server
    if (data3.pointData ().x () != 10 || data3.pointData ().y () != 56)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected value for pointData received: "
        "expected <Point (10,56)> - found <" << data3.pointData () << ">"
        << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::get_union: <"
        << data3 << ">" << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "ERROR: get_union call failed." << std::endl;
    ++retval;
  }
  // This is a copy (as intended).
  Test::Data data4 (data3);
  retval += check_union (data4, Test::DataType::dtPoint, "before update_union");

  if (data4.pointData ().x () != 10 || data4.pointData ().y () != 56)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected value for pointData found after copying: "
      "expected <Point (10,56)> - found <" << data3.pointData () << ">"
      << std::endl;
    ++retval;
  }

  // NOTE: just passed (and returned) by reference!
  if (foo->update_union (data4))
  {
    retval += check_union (data4, Test::DataType::dtTrack, "after update_union");
    // Check the data set by the server
    if (data4.trackData ().id () != 99 ||
      data4.trackData().p().x () != 23 ||
      data4.trackData().p().y () != 77)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected trackData received: "
        << "expected <99, Point(23,77)> - found <" << data4.trackData ()
        << ">" << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::update_union from <"
        << data3 << "> to <" << data4 << ">" << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "ERROR: update_union call failed." << std::endl;
    ++retval;
  }

  Test::Data data5;
  data5.longData (321);
  data5 = data3;
  retval += check_union (data5, Test::DataType::dtPoint, "after assigning");
  if (data5.pointData ().x () != 10 || data5.pointData ().y () != 56)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected value for pointData found after assigning: "
      "expected <Point (10,56)> - found <" << data5.pointData () << ">"
      << std::endl;
    ++retval;
  }
  return retval;
}

uint16_t
test_data_default_union (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t retval = 0;

  Test::DefaultData data;
  retval += check_default_union (data, -1, "after default");
  data.shortData (12);
  retval += check_default_union (data, 2, "before pass_default_union");

  if (foo->pass_default_union (data))
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::pass_default_union: <"
      << data << ">" << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "ERROR: pass_default_union call failed." << std::endl;
    ++retval;
  }

  // Not a copy here but a move!
  Test::DefaultData data2__ = foo->return_default_union ();
  if (data2__._d () != 2)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in data2__ found: "
      << "expected <2> - found <" << data2__._d () << ">" << std::endl;
    ++retval;
  }
  else if (data2__.shortData () != 12)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in data2__: "
      << "expected <12> - found <" << data2__.shortData () << ">"
      << std::endl;
    ++retval;
  }
  retval += check_default_union (data2__, 2, "after return_default_union");

  Test::DefaultData data2 (std::move (data2__));
  retval += check_default_union (data2, 2, "after move");
  if (data2._d () != 2)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in data2 found: "
      << "expected <2> - found <" << data2._d () << ">" << std::endl;
    ++retval;
  }
  else if (data2.shortData () != 12)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in data2: "
      << "expected <12> - found <" << data2.shortData () << ">"
      << std::endl;
    ++retval;
  }
  else
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::return_default_union: <"
      << data2 << ">" << std::endl;
  }

  Test::DefaultData data3;
  if (foo->get_default_union (data3))
  {
    retval += check_default_union (data3, 1, "after get_default_union");
    // Check data set by the server
    if (data3.longData () != 10)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected value for shortData received: "
        "expected <10> - found <" << data3.longData () << ">"
        << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::get_default_union: <"
        << data3 << ">" << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "ERROR: get_default_union call failed." << std::endl;
    ++retval;
  }

  // This is a copy (as intended).
  Test::DefaultData data4 (data3);
  retval += check_default_union (data4, 1, "before update_default_union");
  if (data4.longData () != 10)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected value for shortData found in "
        << "data4: expected <10> - found <" << data4.longData () << ">"
        << std::endl;
      ++retval;
    }
  data4.defData (123456);
  retval += check_default_union (data4, -1, "after moving data4 to def");
  if (data4.defData () != 123456)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected value for defData found in "
        << "data4: expected <123456> - found <" << data4.shortData () << ">"
        << std::endl;
      ++retval;
    }
  // NOTE: just passed (and returned) by reference!
  if (foo->update_default_union (data4))
  {
    retval += check_default_union (data4, 1, "after update_default_union");
    // Check the data set by the server
    if (data4.longData () != 99)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected trackData received: "
        << "expected <99> - found <" << data4.longData ()
        << ">" << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "successfully called Foo::update_default_union from <"
        << data3 << "> to <" << data4 << ">" << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "ERROR: update_default_union call failed." << std::endl;
    ++retval;
  }

  return retval;
}

uint16_t
test_data_x (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t retval = 0;

  Test::X_Union data = foo->return_x_union ();
  if (data._d () != 1)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in X_Union data found: "
      << "expected <1> - found <" << data._d () << ">" << std::endl;
    ++retval;
  }
  else if (data.x_long () != 10)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in X_Union data: "
      << "expected <10> - found <" << data.x_long () << ">"
      << std::endl;
    ++retval;
  }
  else
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::test_data_x: <"
      << data << ">" << std::endl;
  }
  return retval;
}

uint16_t
test_data_y (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t retval = 0;

  Test::Y_Union data = foo->return_y_union ();
  if (data._d () != 1)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in Y_Union data found: "
      << "expected <1> - found <" << data._d () << ">" << std::endl;
    ++retval;
  }
  else if (data.y_test () != "hello")
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in Y_Union data: "
      << "expected <hello> - found <" << data.y_test () << ">"
      << std::endl;
    ++retval;
  }
  else
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::test_data_y: <"
      << data << ">" << std::endl;
  }
  return retval;
}

uint16_t
test_data_z (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t retval = 0;

  Test::Z_Union data;
  data.z_string ("Hi");
  Test::Z_Union ret = foo->return_z_union (data);
  if (ret._d () != 3)
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected discriminator in Z_Union data found: "
      << "expected <3> - found <" << ret._d () << ">" << std::endl;
    ++retval;
  }
  else if (ret.z_string () != "Bye")
  {
    TAOX11_TEST_ERROR << "ERROR: Unexpected data found in Z_Union data: "
      << "expected <\"Bye\"> - found <" << ret.z_string () << ">"
      << std::endl;
    ++retval;
  }
  else
  {
    TAOX11_TEST_DEBUG << "successfully called Foo::test_data_z: <"
      << data << "> - <" << ret << ">." << std::endl;
  }

  // Check that passing an invalid discriminator will result in an exception
  Test::Z_Union data2;
  try
  {
    data2.z_string("Hello", 4);
    TAOX11_TEST_ERROR << "ERROR: Setting Z_Union::z_string with discriminator 4 should result in an exception"
      << std::endl;
    ++retval;
  }
  catch (const CORBA::BAD_PARAM&)
  {
    TAOX11_TEST_DEBUG << "Setting Z_Union::z_string with discriminator 4 resulted in correct exception" << std::endl;
  }
  return retval;
}

uint16_t
test_union_message (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t retval = 0;

  Test::Data dt_d;
  Test::B03 b_d;

  // Set the last item
  b_d.b_03_5 ("XX");
  dt_d.globalData (Global (4));

  Test::UnionMessage msg_d { Test::Assignment::D, b_d, dt_d };

  TAOX11_TEST_DEBUG << "Sending <"
      << msg_d << ">" << std::endl;

  bool const ret = foo->send_unionmessage (msg_d);

  if (!ret)
  {
    TAOX11_TEST_ERROR << "ERROR: send_unionmessage returned false "
      << std::endl;
    ++retval;
  }

  return retval;
}

uint16_t
test_value_initialization ()
{
  TAOX11_TEST_DEBUG << "Test test_value_initialization" << std::endl;

  uint16_t retval {};

  TAOX11_TEST_DEBUG << "Test test_value_initialization Test::unionarrayarrayarrayoflong" << std::endl;

  Test::unionarrayarrayarrayoflong uaaal;
  int32_t default_int32_t {};
  for (const auto& aaal_member : uaaal.aaal()) {
    for (const auto& aal_member : aaal_member) {
      for (const auto& al_member : aal_member) {
        if (al_member != default_int32_t) {
          TAOX11_TEST_ERROR << "Array member of uaaal not value initialized, " << al_member << " instead of " << default_int32_t << std::endl;
          return 1;
        }
      }
    }
  }

  TAOX11_TEST_DEBUG << "Test test_value_initialization Test::unionarrayarrayoflong" << std::endl;

  Test::unionarrayarrayoflong uaal;
  for (const auto& aal_member : uaal.aal()) {
    for (const auto& al_member : aal_member) {
      if (al_member != default_int32_t) {
        TAOX11_TEST_ERROR << "Array member of uaal not value initialized, " << al_member << " instead of " << default_int32_t << std::endl;
        return 1;
      }
    }
  }

  TAOX11_TEST_DEBUG << "Test test_value_initialization Test::unionarrayoflong" << std::endl;

  Test::unionarrayoflong ual;
  for (const auto& al_member : ual.al()) {
    if (al_member != default_int32_t) {
      TAOX11_TEST_ERROR << "Array member of ual not value initialized, " << al_member << " instead of " << default_int32_t << std::endl;
      return 1;
    }
  }

  TAOX11_TEST_DEBUG << "Test test_value_initialization Test::unionarrayarrayofenum" << std::endl;

  Test:: enumType default_enumType {};
  Test::unionarrayarrayofenum uaae;
  for (const auto& aae_member : uaae.aae()) {
    for (const auto& ae_member : aae_member) {
      if (ae_member != default_enumType) {
        TAOX11_TEST_ERROR << "Array member of uaae not value initialized, " << ae_member << " instead of " << default_enumType << std::endl;
        return 1;
      }
    }
  }

  TAOX11_TEST_DEBUG << "Test test_value_initialization Test::unionarrayofenum" << std::endl;

  Test::unionarrayofenum uae;
  for (const auto& ae_member : uae.ae()) {
    if (ae_member != default_enumType) {
      TAOX11_TEST_ERROR << "Array member of uae not value initialized, " << ae_member << " instead of " << default_enumType << std::endl;
      return 1;
    }
  }

  return retval;
}

int main (int argc, char* argv[])
{
  uint16_t retval = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!orb)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object ("file://test.ior");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
        return 1;
      }

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      retval += test_data_union (foo);
      retval += test_data_default_union (foo);
      retval += test_data_x (foo);
      retval += test_data_y (foo);
      retval += test_data_z (foo);
      retval += test_union_message (foo);
      retval += test_value_initialization ();

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;

      foo->shutdown ();
      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }
  return retval;
}
