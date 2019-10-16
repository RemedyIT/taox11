/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "factoryC.h"
#include "factoryc_impl.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

#include "tao/x11/basic_traits.h"
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/object.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

bool parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:") );
  int c;

  while ( (c = get_opts() ) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -k <ior>" << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

/// Test that no factory is registred.
bool no_factory (IDL::traits<OBV_Test::IFactory>::ref_type test)
{
  bool succeed = false;
  try
    {
      // Calling this without a factory registered should give a marshal
      // exception with minor code 1
      IDL::traits<OBV_Test::BasValue>::ref_type base_value =
        test->get_base_value();
    }
  catch (const CORBA::MARSHAL& ex)
    {
      if ( (ex.minor() & 0xFFFU) == 1)
        {
          succeed = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
    {
      TAOX11_TEST_ERROR << "ERROR, no_factory failed" << std::endl;
    }
  return succeed;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  uint16_t errors = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == false)
        return 1;

      // Obtain reference to the object
      IDL::traits<CORBA::Object>::ref_type tmp = orb->string_to_object (ior);

      IDL::traits<OBV_Test::IFactory>::ref_type fact =
        IDL::traits<OBV_Test::IFactory>::narrow (tmp);

      if (!fact)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<OBV_Test::IFactory>::narrow returned null reference."
            << std::endl;
          return 1;
        }

      // Check if we get the correct exception with minor code because no
      // factory has been set.
      if (!no_factory (fact))
        return 1;

      // Create factories.
      IDL::traits<OBV_Test::BasValue>::factory_ref_type base_factory =
        CORBA::make_reference<OBV_Test::BasValue_init>();

      const std::string id1 = base_factory->_obv_repository_id();
      TAOX11_TEST_DEBUG << "Created factory with id: " << id1 << std::endl;

      CORBA::ValueFactory vt_factory =
        orb->register_value_factory (base_factory->_obv_repository_id(),
          base_factory);
      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Registered factory with id: " << id1 << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Factory with id " << id1 << " already exists."
                        << std::endl;
          ++errors;
        }

      IDL::traits<OBV_Test::Value1>::factory_ref_type value1_factory =
        CORBA::make_reference<OBV_Test::Value1_init>();

      vt_factory = orb->register_value_factory (
        value1_factory->_obv_repository_id(),
        value1_factory);

      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Registered factory with id: "
                        << value1_factory->_obv_repository_id()
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Factory for Value1 already exists"
                        << std::endl;
          ++errors;
        }

      IDL::traits<OBV_Test::Value2>::factory_ref_type v2_fact1 =
        CORBA::make_reference<Value2_init_impl>();

      vt_factory = orb->register_value_factory (v2_fact1->_obv_repository_id(), v2_fact1);

      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Registered factory with id: "
            << v2_fact1->_obv_repository_id() << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Factory for Value2 already exists"
                        << std::endl;
          ++errors;
        }

      IDL::traits<OBV_Test::Value2>::factory_ref_type v2_fact2 =
        CORBA::make_reference<Value2_init_impl>();

      std::string const id = v2_fact2->_obv_repository_id();
      TAOX11_TEST_DEBUG << "Created a factory with id: " << id << std::endl;

      CORBA::ValueFactory v2_lookup;
      v2_lookup = orb->lookup_value_factory (id);
      if (v2_lookup)
        {
          TAOX11_TEST_DEBUG << "lookup_value_factory found same id: " << id << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "Id should already exist for factory of same type."
            << std::endl;
          ++errors;
        }
      // Registering with register_value_factory with already a factory
      // registered should replace the factory already set.
      vt_factory = orb->register_value_factory (id, v2_fact2);
      if (vt_factory)
        {
          TAOX11_TEST_DEBUG << "Rebind factory with id " << id
                        << "to v2_fact2."
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "Id should already exist for factory of same type."
            << std::endl;
          ++errors;
        }
      v2_lookup = orb->lookup_value_factory (id);
      if (!v2_lookup)
        {
          TAOX11_TEST_ERROR << "register_value_factory failed." << std::endl;
          ++errors;
        }
      orb->unregister_value_factory (id);
      TAOX11_TEST_DEBUG << "Unregistered factory with id: " << id << std::endl;
      v2_lookup = orb->lookup_value_factory (id);
      if (v2_lookup)
        {
          TAOX11_TEST_ERROR << "ERROR: " << id << " still exists" << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_DEBUG << "Succesfully looked up unregistered value factory "
                        << "with id " << id
                        << std::endl;
        }
      orb->unregister_value_factory (id);
      TAOX11_TEST_DEBUG << "Tried unregister factory with id: " << id << " again."
                    << std::endl;

      vt_factory = orb->register_value_factory (id, v2_fact2);
      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Registered factory with id: " << id << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to register factory with id " << id
                        << std::endl;
          ++errors;
        }

      vt_factory = orb->register_value_factory ("test1", v2_fact1);
      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Succesfully bound v2_fact1 to \"test1\""
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to bound v2_fact1 to \"test1\""
                        << std::endl;
          ++errors;
        }

      vt_factory = orb->register_value_factory ("test2", v2_fact1);
      if (!vt_factory)
        {
          TAOX11_TEST_DEBUG << "Succesfully bound v2_fact1 to \"test2\""
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to bound v2_fact1 to \"test2\""
                        << std::endl;
          ++errors;
        }

      v2_lookup = orb->lookup_value_factory (id);
      if (v2_lookup)
        {
          TAOX11_TEST_DEBUG << "lookup_value_factory found "
                        << v2_lookup->_obv_repository_id()
                        << " for id: " << id
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to lookup "
                        << v2_lookup->_obv_repository_id()
                        << " for id: " << id
                        << std::endl;
          ++errors;
        }

      v2_lookup = orb->lookup_value_factory ("test1" );
      if (v2_lookup)
        {
          TAOX11_TEST_DEBUG << "lookup_value_factory found "
                        << v2_lookup->_obv_repository_id()
                        << " for id: " << "test1"
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to lookup "
                        << v2_lookup->_obv_repository_id()
                        << " for id: " << "test1"
                        << std::endl;
          ++errors;
        }

      v2_lookup = orb->lookup_value_factory ("test2" );
      if (v2_lookup)
        {
          TAOX11_TEST_DEBUG << "lookup_value_factory found "
                        << v2_lookup->_obv_repository_id()
                        << " for id: " << "test2"
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to lookup "
                        << v2_lookup->_obv_repository_id()
                        << " for id: " << "test2"
                        << std::endl;
          ++errors;
        }

      // Now perform the test. Return values and marshalling are not checked.
      IDL::traits<OBV_Test::BasValue>::ref_type basvalue = fact->get_base_value();
      IDL::traits<OBV_Test::Value1>::ref_type value1 = fact->get_value1();
      IDL::traits<OBV_Test::Value2>::ref_type value2 = fact->get_value2();

      // Test factories.
      value2 = v2_fact2->create_default (1);
      if (value2->id () != 1)
        {
          TAOX11_TEST_ERROR << "ERROR: Incorrect id for Value2 found:"
                        << "found <" << value2->id () << "> - expected <1>."
                        << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_DEBUG << "Correct id for Value2 found <"
                        << value2->id () << ">" << std::endl;
        }

      OBV_Test::BasValue::BV_Data data;
      data.value (2);

      value2 = v2_fact2->create (3, data);

      if (value2->id () != 3)
        {
          TAOX11_TEST_ERROR << "ERROR: Incorrect id for Value2 found:"
                        << "found <" << value2->id () << "> - expected <3>."
                        << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_DEBUG << "Correct id for Value2 found <"
                        << value2->id () << ">" << std::endl;
        }
      if (value2->data ().value() != 2)
        {
          TAOX11_TEST_ERROR << "ERROR: Incorrect data.value for Value2 found:"
                        << "found <" << value2->data ().value ()
                        << "> - expected <2>." << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_DEBUG << "Correct data.value for Value2 found <"
                        << value2->data ().value () << ">"
                        << std::endl;
        }

      TAOX11_TEST_INFO << std::endl
                  << "client - test finished. Found <"
                  << errors << "> error(s)."
                  << std::endl << std::endl;

      fact->shutdown();

      orb->destroy();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }

  return errors;
}
