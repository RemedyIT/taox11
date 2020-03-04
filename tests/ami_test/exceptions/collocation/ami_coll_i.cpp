/**
 * @file    ami_coll_i.cpp
 * @author  Marcel Smit
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "ami_coll_i.h"
#include "testlib/taox11_testlog.h"

#define EXCEPTION_MESSAGE_METHOD "Start doing something"
#define EXCEPTION_MESSAGE_ATTRIBUTE "Attribute 'something'"

BarHandler::BarHandler ()
{

}

BarHandler::~BarHandler ()
{
}

void
BarHandler::do_something ()
{
  TAOX11_TEST_ERROR << "ERROR : BarHandler::do_something - Bad invocation." << std::endl;
}

void
BarHandler::do_something_excep (
  >::ref_type >::ref_type excep_holder)
{
  TAOX11_TEST_INFO << "Callback method <do_something_excep> called: "
              << "Testing proper exception handling."<< std::endl;
  try
  {
    excep_holder->raise_exception ();
  }
  catch (const Test::StructEx &ex)
  {
    TAOX11_TEST_DEBUG << "BarHandler::do_something_excep - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.struct_i ().id () != 12345)
    {
      TAOX11_TEST_ERROR << "ERROR : BarHandler::do_something_excep - "
        << "Correct exception caught but 'id' seems to contain a wrong value. "
        << "expected <12345> - found <" << ex.struct_i ().id () << ">"
        << std::endl;
    }
    if (ex.struct_i ().message () != EXCEPTION_MESSAGE_METHOD)
      {
        TAOX11_TEST_ERROR << "ERROR : BarHandler::do_something_excep - "
          << "Correct exception caught but it seems to contain a wrong message. "
          << "expected <" << EXCEPTION_MESSAGE_METHOD <<  "> - found <"
          << ex.struct_i ().message () << ">" << std::endl;
      }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR : BarHandler::do_something_excep - "
      << "Unexpected std exception caught: " << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "ERROR : BarHandler::do_something_excep - "
      << "Unexpected unknown exception caught" << std::endl;
    throw;
  }
}

void
BarHandler::get_something (int32_t )
{
  TAOX11_TEST_ERROR << "ERROR : BarHandler::get_something - Bad invocation."
    << std::endl;
}

void
BarHandler::get_something_excep (
  >::ref_type >::ref_type excep_holder)
{
  TAOX11_TEST_INFO << "Callback method <get_something_excep> called: "
              << "Testing proper exception handling."<< std::endl;
  try
  {
    excep_holder->raise_exception ();
  }
  catch (const Test::SomeException &ex)
  {
    TAOX11_TEST_DEBUG << "BarHandler::get_something_excep - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.id () != 10)
    {
      TAOX11_TEST_ERROR << "ERROR : BarHandler::get_something_excep - "
        << "Correct exception caught while getting attribute 'something' "
        << "but 'id' seems to contain a wrong value. "
        << "expected <10> - found <" << ex.id () << ">"
        << std::endl;
    }
    if (ex.message () != EXCEPTION_MESSAGE_ATTRIBUTE)
    {
      TAOX11_TEST_ERROR << "ERROR : BarHandler::get_something_excep - "
        << "Correct exception caught while getting attribute 'something' "
        << "but it seems to contain a wrong message. "
        << "expected <" << EXCEPTION_MESSAGE_ATTRIBUTE <<  "> - found <"
        << ex.message () << ">" << std::endl;
    }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR : BarHandler::get_something_excep - "
      << "Unexpected std exception caught while getting attribute 'something' : "
      << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "ERROR : BarHandler::get_something_excep - "
      << "Unexpected unknown exception caught while setting attribute 'something'"
      << std::endl;
    throw;
  }
}

void
BarHandler::set_something ()
{
  TAOX11_TEST_ERROR << "ERROR : BarHandler::set_something - Bad invocation."
    << std::endl;
}

void
BarHandler::set_something_excep (
  >::ref_type >::ref_type excep_holder)
{
  TAOX11_TEST_INFO << "Callback method <set_something_excep> called: "
              << "Testing proper exception handling."<< std::endl;
  try
  {
    excep_holder->raise_exception ();
  }
  catch (const Test::SomeException &ex)
  {
    TAOX11_TEST_DEBUG << "BarHandler::set_something_excep - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.id () != 0)
    {
      TAOX11_TEST_ERROR << "ERROR : BarHandler::set_something_excep - "
        << "Correct exception caught while setting attribute 'something' "
        << "but 'id' seems to contain a wrong value. "
        << "expected <0> - found <" << ex.id () << ">"
        << std::endl;
    }
    if (ex.message () != EXCEPTION_MESSAGE_ATTRIBUTE)
    {
      TAOX11_TEST_ERROR << "ERROR : BarHandler::set_something_excep - "
        << "Correct exception caught while setting attribute 'something' "
        << "but it seems to contain a wrong message. "
        << "expected <" << EXCEPTION_MESSAGE_ATTRIBUTE <<  "> - found <"
        << ex.message () << ">" << std::endl;
    }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR : BarHandler::set_something_excep - "
      << "Unexpected std exception caught while setting attribute 'something' : "
      << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "ERROR : BarHandler::set_something_excep - "
      << "Unexpected unknown exception caught while setting attribute 'something'"
      << std::endl;
    throw;
  }
}


Bar::Bar ()
{
}

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
  TAOX11_TEST_DEBUG << "Bar::something (getter) - Throw a SomeException exception, "
    << "initialized with <10> and '" << EXCEPTION_MESSAGE_ATTRIBUTE << "'"
    << std::endl;
  throw Test::SomeException (10, EXCEPTION_MESSAGE_ATTRIBUTE);
}

void
Bar::something (int32_t something)
{
  TAOX11_TEST_DEBUG << "Bar::something (setter) - Throw a SomeException exception, "
    << "initialized with <" << something << "> and '" << EXCEPTION_MESSAGE_ATTRIBUTE
    << "'" << std::endl;
  throw Test::SomeException (something, EXCEPTION_MESSAGE_ATTRIBUTE);
}


Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<Test::Bar>::ref_type bar,
  CORBA::amic_traits<Test::Bar>::replyhandler_ref_type bar_handler,
  CORBA::amic_traits<Test::Bar>::ref_type ami_bar_client)
  : orb_ (orb)
  , bar_ (bar)
  , bar_handler_ (bar_handler)
  , ami_bar_client_ (ami_bar_client)
{
}

void
Foo::test_collocated_exception_method ()
{
  // Test collocated exception for methods
  try
  {
    this->bar_->do_something (EXCEPTION_MESSAGE_METHOD);
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_method - "
      << "No exception caught" << std::endl;
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
  TAOX11_TEST_DEBUG << "Foo::test_collocated_exception_attribute - "
    << "Testing the getter of 'something'" << std::endl;
  try
  {
    int32_t tmp = this->bar_->something ();
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "No exception caught while getting attribute 'something'. 'something' "
      << "returned " << tmp << std::endl;
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
  TAOX11_TEST_DEBUG << "Foo::test_collocated_exception_attribute - "
    << "Testing the setter of 'something'" << std::endl;
  try
  {
    this->bar_->something (0);
    TAOX11_TEST_ERROR << "ERROR : Foo::test_collocated_exception_attribute - "
      << "No exception caught while setting attribute 'something'" << std::endl;
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
Foo::test_asynch_collocated_exception_method ()
{
  // Test collocated exception for methods first
  this->ami_bar_client_->sendc_do_something (this->bar_handler_, EXCEPTION_MESSAGE_METHOD);
}

void
Foo::test_asynch_collocated_exception_attribute ()
{
  // Testing the getraises colocated exception
  TAOX11_TEST_DEBUG << "Foo::test_asynch_collocated_exception_attribute - "
    << "Testing the setter of 'something'" << std::endl;
  this->ami_bar_client_->sendc_get_something (this->bar_handler_);

  // Now testing the setraises.
  TAOX11_TEST_DEBUG << "Foo::test_asynch_collocated_exception_attribute - "
    << "Testing the getter of 'something'" << std::endl;
  this->ami_bar_client_->sendc_set_something (this->bar_handler_, 0);
}

void
Foo::do_throw_struct ()
{
  // Asynchronous invocations
  TAOX11_TEST_DEBUG << "Foo::do_throw_struct - "
    << "Calling 'do_something' asynchronously on bar" << std::endl;
  this->test_asynch_collocated_exception_method ();

  TAOX11_TEST_DEBUG << "Foo::do_throw_struct - "
    << "Manipulating attribute 'something' asynchronously on bar" << std::endl;
  this->test_asynch_collocated_exception_attribute ();

  // Synchronous invocations
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
