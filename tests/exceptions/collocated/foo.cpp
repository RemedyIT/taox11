/**
 * @file    foo.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"
#include "testlib/taox11_testlog.h"

const std::string EXCEPTION_MESSAGE_METHOD ("Start doing something");
const std::string EXCEPTION_MESSAGE_ATTRIBUTE ("Attribute 'something'");

void
Bar::do_something (const std::string &message)
{
  TAOX11_TEST_DEBUG << "Bar::do_something - Throw a StructEx exception containing a "
    << "MessageStruct, initialized with 12345 and '" << message << "'"
    << std::endl;
  throw Test::StructEx (Test::MessageStruct (12345, message));
}

int32_t
Bar::something ()
{
  TAOX11_TEST_DEBUG << "Bar::something - Throw a SomeException exception, initialized "
    << "with <10> and '" << EXCEPTION_MESSAGE_ATTRIBUTE << "'"
    << std::endl;
  throw Test::SomeException (10, EXCEPTION_MESSAGE_ATTRIBUTE);
}

void
Bar::something (int32_t something)
{
  TAOX11_TEST_DEBUG << "Bar::something - Throw a SomeException exception, initialized "
    << "with <" << something << "> and '" << EXCEPTION_MESSAGE_ATTRIBUTE << "'"
    << std::endl;
  throw Test::SomeException (something, EXCEPTION_MESSAGE_ATTRIBUTE);
}


Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<Test::Bar>::ref_type bar)
  : orb_ (std::move(orb))
  , bar_ (std::move(bar))
{
}

void
Foo::test_collocated_exception_method ()
{
  // Test collocated exception for methods first
  TAOX11_TEST_DEBUG << "Foo::test_collocated_exception_method - "
    << "Calling 'do_something' on bar" << std::endl;
  // Test collocated exception for methods
  try
  {
    this->bar_->do_something (EXCEPTION_MESSAGE_METHOD);
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_method - "
      << "No exception caught" << std::endl;
    throw Test::StructEx (Test::MessageStruct (0, "No exception"));
  }
  catch (const Test::StructEx &ex)
  {
    TAOX11_TEST_DEBUG << "Foo::test_collocated_exception_method - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.struct_i ().id () != 12345)
      {
        TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_method - "
          << "Correct exception caught but 'id' seems to contain a wrong value. "
          << "expected <12345> - found <" << ex.struct_i ().id () << ">"
          << std::endl;
      }
    if (ex.struct_i ().message () != EXCEPTION_MESSAGE_METHOD)
      {
        TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_method - "
          << "Correct exception caught but it seems to contain a wrong message. "
          << "expected <" << EXCEPTION_MESSAGE_METHOD <<  "> - found <"
          << ex.struct_i ().message () << ">" << std::endl;
      }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_method - "
      << "Unexpected std exception caught: " << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_method - "
      << "Unexpected unknown exception caught" << std::endl;
    throw;
  }
}

void
Foo::test_collocated_exception_attribute ()
{
  // Testing the getraises colocated exception
  try
  {
    int32_t const tmp = this->bar_->something ();
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "No exception caught while getting attribute 'something'. 'something' "
      << "returned " << tmp << std::endl;
    throw Test::StructEx (Test::MessageStruct (0, "No exception"));
  }
  catch (const Test::SomeException &ex)
  {
    TAOX11_TEST_DEBUG << "Foo::test_collocated_exception_attribute - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.id () != 10)
      {
        TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
          << "Correct exception caught while getting attribute 'something' "
          << "but 'id' seems to contain a wrong value. "
          << "expected <10> - found <" << ex.id () << ">"
          << std::endl;
      }
    if (ex.message () != EXCEPTION_MESSAGE_ATTRIBUTE)
      {
        TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
          << "Correct exception caught while getting attribute 'something' "
          << "but it seems to contain a wrong message. "
          << "expected <" << EXCEPTION_MESSAGE_ATTRIBUTE <<  "> - found <"
          << ex.message () << ">" << std::endl;
      }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "Unexpected std exception caught while getting attribute 'something' : "
      << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "Unexpected unknown exception caught while setting attribute 'something'"
      << std::endl;
    throw;
  }

  // Now testing the setraises.
  try
  {
    this->bar_->something (0);
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "No exception caught while setting attribute 'something'" << std::endl;
    throw Test::StructEx (Test::MessageStruct (0, "No exception"));
  }
  catch (const Test::SomeException &ex)
  {
    TAOX11_TEST_DEBUG << "Foo::test_collocated_exception_attribute - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.id () != 0)
      {
        TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
          << "Correct exception caught while setting attribute 'something' "
          << "but 'id' seems to contain a wrong value. "
          << "expected <0> - found <" << ex.id () << ">"
          << std::endl;
      }
    if (ex.message () != EXCEPTION_MESSAGE_ATTRIBUTE)
      {
        TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
          << "Correct exception caught while setting attribute 'something' "
          << "but it seems to contain a wrong message. "
          << "expected <" << EXCEPTION_MESSAGE_ATTRIBUTE <<  "> - found <"
          << ex.message () << ">" << std::endl;
      }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "Unexpected std exception caught while setting attribute 'something' : "
      << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "Unexpected unknown exception caught while setting attribute 'something'"
      << std::endl;
    throw;
  }
}

void
Foo::do_throw_struct ()
{
  TAOX11_TEST_DEBUG << "Foo::do_throw_struct - "
    << "Calling 'do_something' on bar" << std::endl;
  this->test_collocated_exception_method ();

  TAOX11_TEST_DEBUG << "Foo::do_throw_struct - "
    << "Manipulating attribute 'something' on bar" << std::endl;
  this->test_collocated_exception_attribute ();

  throw Test::StructEx (Test::MessageStruct (888, "Almost right"));
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
