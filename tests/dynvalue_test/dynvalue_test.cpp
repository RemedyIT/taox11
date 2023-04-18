/**
 * @file    dynvalue_test.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   Main of DynValue test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "analyzer.h"
#include "dynvalue_testC.h"

int main (int argc, ACE_TCHAR *argv[])
{
  // Retrieve the underlying ORB
  IDL::traits<CORBA::ORB>::ref_type orb_ = CORBA::ORB_init (argc, argv, "local");
  if (!orb_)
  {
    TAOX11_TEST_ERROR << "Error: Could not init orb" << std::endl;
    return 1;
  }

  IDL::traits<CORBA::Object>::ref_type factory_obj =
     orb_->resolve_initial_references ("DynAnyFactory");

  if (!factory_obj)
  {
    TAOX11_TEST_ERROR << "Error: Nil factory_obj after resolve_initial_references"
                      << std::endl;
    return 1;
  }

  IDL::traits<DynamicAny::DynAnyFactory>::ref_type dany_fact_ =
     IDL::traits<DynamicAny::DynAnyFactory>::narrow (factory_obj);
  if (!dany_fact_)
  {
    TAOX11_TEST_ERROR << "Error: Nil dynamic any factory after narrow dany_fact_"
                      << std::endl;
    return 1;
  }
  DynAnyAnalyzer analyzer(dany_fact_);
  try
  {
    ////////////////////////////////////////////
    // DynValue_Test::BoxedLong
    ////////////////////////////////////////////
    TAOX11_TEST_DEBUG <<"A *=*=*=*= DynValue_Test::BoxedLong =*=*=*=*" << std::endl;
    int32_t const l1 = -157;

    TAOX11_TEST_DEBUG << "A1.Creating DynValue_Test::BoxedLong" << std::endl;
    IDL::traits<DynValue_Test::BoxedLong>::ref_type myBoxedLong =
        CORBA::make_reference <DynValue_Test::BoxedLong> (l1);

    TAOX11_TEST_DEBUG << "\tchecking value, long is " << myBoxedLong->_value () << std::endl;
    if (l1 != myBoxedLong->_value ())
    {
      TAOX11_TEST_ERROR << "ERROR: Failed, should have been "<< l1 << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "A2.Inserting into an any" << std::endl;
    CORBA::Any myAny;
    myAny <<= myBoxedLong;

    TAOX11_TEST_DEBUG << "Extracting from an any" << std::endl;

    IDL::traits<DynValue_Test::BoxedLong>::ref_type myBoxedLongExtracted {};
    if (!(myAny >>= myBoxedLongExtracted))
    {
      TAOX11_TEST_ERROR << "ERROR: Failed extraction" << std::endl;
      return 1;
    }
    if (!myBoxedLongExtracted)
    {
      TAOX11_TEST_ERROR << "ERROR: Failed null pointer extracted" << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "\tchecking value, long is " << myBoxedLongExtracted->_value () << std::endl;
    if (l1 != myBoxedLongExtracted->_value ())
    {
      TAOX11_TEST_ERROR << "Error Failed, should have been " << l1 << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A3.Creating dynamic any from any with tc " << myAny.type()->kind() << std::endl;
    IDL::traits<DynamicAny::DynAny>::ref_type dany_boxed = dany_fact_->create_dyn_any  (myAny);
    analyzer.analyze (dany_boxed);

    TAOX11_TEST_DEBUG << "A4.Testing dynamic any for not being null with is_null, " << std::endl;

    IDL::traits<DynamicAny::DynValueCommon>::ref_type dvc =
                       IDL::traits<DynamicAny::DynValueCommon>::narrow (dany_boxed);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << "ERROR : Failed, can not narrow dany_boxed to  DynamicAny::DynValueCommon" << std::endl;
      return 1;
    }
    if (dvc->is_null ())
    {
      TAOX11_TEST_ERROR << "ERROR: Failed, this dynamic any (dany_boxed) is null!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A5.Creating dynamic any from IDL generated typecode" << std::endl;
    IDL::traits<DynamicAny::DynAny>::ref_type dany  = dany_fact_->create_dyn_any_from_type_code (DynValue_Test::_tc_BoxedLong);
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "A6.Testing typecode generated dynamic any for being null, with is_null" << std::endl;
    dvc = IDL::traits<DynamicAny::DynValueCommon>::narrow (dany);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << "ERROR : Failed, can not narrow dany to  DynamicAny::DynValueCommon" << std::endl;
      return 1;
    }
    if (!(dvc->is_null ()))
    {
      TAOX11_TEST_ERROR << "ERROR : Failed, this dynamic any has a value!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A7.Testing for inequality with original dynamic any" << std::endl;
    if (dany->equal (dany_boxed))
    {
      TAOX11_TEST_ERROR << "ERROR : Failed, they test equal" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A8. Setting to value and test for inequality with original dynamic any" << std::endl;

    dvc->set_to_value ();
    if ((dvc->component_count() != 1) || (dvc->component_count() != 1))
    {
      TAOX11_TEST_ERROR << "Error, Fialed set_to_value" << std::endl;
      return 1;
    }

    analyzer.analyze (dany);
    if (dany->equal (dany_boxed))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test equal before setting long" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A9. Setting the long of the typecode generated dynamic any" << std::endl;
    dany->insert_long (l1);
    analyzer.analyze (dany);
    TAOX11_TEST_DEBUG << " Testing for equality with original dynamic any" << std::endl;
    if (!(dany->equal (dany_boxed)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal after setting long" << std::endl;
      return 1;
    }
    //dvc with dany
    TAOX11_TEST_DEBUG << "A10. Setting typecode dynamic any back to null" << std::endl;
    dvc->set_to_null ();
    analyzer.analyze (dany);
    if ((dvc->component_count() != 0) || (dvc->component_count() != 0))
    {
      TAOX11_TEST_ERROR << "Error, Fialed set_to_value" << std::endl;
      return 1;
    }

    //dvc with dany_boxed
    TAOX11_TEST_DEBUG << "A11. Setting the original dynamic any to null object" << std::endl;
    dvc= IDL::traits<DynamicAny::DynValueCommon>::narrow (dany_boxed);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << "ERROR: Failed, can not obtain DynamicAny::DynValueCommon *" << std::endl;
      return 1;
    }
    dvc->set_to_null ();
    analyzer.analyze (dany_boxed);

    //dany_boxed and dany are now both set to null
    TAOX11_TEST_DEBUG << "A12.Testing for equality with typecode dynamic any" << std::endl;
    if (!(dany->equal (dany_boxed)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A13. Converting from null dynamic any back to any" << std::endl;
    CORBA::Any myAny2 = dany_boxed->to_any ();

    TAOX11_TEST_DEBUG << "  Extracting from this any" << std::endl;

    //At this point myBoxedLongExtracted isn't a nullptr
    if (!(myAny2 >>= myBoxedLongExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed extraction" << std::endl;
      return 1;
    }
    //extraction succesfull, myBoxedLongExtracted  should now contain a nullptr
    if (myBoxedLongExtracted != nullptr)
    {
      TAOX11_TEST_DEBUG << "Error, Failed non-null pointer extracted" << std::endl;
      return 1;
    }

    //dvc with dany_boxed,
    TAOX11_TEST_DEBUG << "A14. Setting the dynamic any to VALUE object" << std::endl;
    bool received = false;
    try {
      dany_boxed->get_long();
    }
    catch (const DynamicAny::DynAny::InvalidValue &)
    {
      TAOX11_TEST_DEBUG << " Received expected exception, before setting to value object" << std::endl;
      received = true;
    }
    if(!received)
    {
       TAOX11_TEST_ERROR << " Error, Failed , starting value of dany_boxed is wrong" << std::endl;
       return 1;
    }
    else
    {
      dvc->set_to_value ();
      if (dany_boxed->get_long() != l1)
      {
        TAOX11_TEST_ERROR << " Error, Failed , set_to_value" << std::endl;
        return 1;
      }
    }
    analyzer.analyze (dany_boxed);

     //At this point myAny contains myBoxedLong -157, dany was set to null
    TAOX11_TEST_DEBUG << "A15. Resetting typecode dynamic any from original any" << std::endl;
    dany->from_any (myAny);
    analyzer.analyze (dany);
    analyzer.analyze (dany_boxed);

    TAOX11_TEST_DEBUG << "A16. Testing for equality with original dynamic any" << std::endl;
    if (!(dany->equal (dany_boxed)))
    {
      TAOX11_TEST_ERROR << " Error, Failed they test unequal!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A17. Converting from dynamic any back to any" << std::endl;
    myAny2 = dany_boxed->to_any ();

    TAOX11_TEST_DEBUG << "Extracting from this any" << std::endl;
    myBoxedLongExtracted=CORBA::make_reference <DynValue_Test::BoxedLong> (9);
    if (!(myAny >>= myBoxedLongExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed extraction" << std::endl;
      return 1;
    }
    if (!myBoxedLongExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed null pointer extracted" << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "checking value, long is " << myBoxedLongExtracted->_value () << std::endl;
    if (l1 != myBoxedLongExtracted->_value ())
    {
      TAOX11_TEST_ERROR << "ERROR: Failed should have been" << l1 << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A18.Attempting to get_long from dynamic any" << std::endl;
    int32_t const myLongExtracted = dany_boxed->get_long ();
    TAOX11_TEST_DEBUG << "checking value, long is " << myLongExtracted << std::endl;
    if (l1 != myLongExtracted)
    {
      TAOX11_TEST_ERROR << "Error: FAILED should have been  "<< l1 << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A19. Narrow DynAny to DynValueBox " << std::endl;
    IDL::traits<DynamicAny::DynValueBox>::ref_type davb1 =
          IDL::traits<DynamicAny::DynValueBox>::narrow (dany_boxed);

    if (!davb1)
    {
      TAOX11_TEST_ERROR << "DynArray::narrow() returned nil" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "A20. get_boxed_value and set_boxed_value " << std::endl;
    CORBA::Any any_bv = davb1->get_boxed_value();
    int32_t extracted_long { 5 };
    IDL::traits<DynValue_Test::BoxedLong>::ref_type extracted_bl {};
    if (!(any_bv >>= extracted_long))
    {
      TAOX11_TEST_ERROR << "Error, Failed extraction" << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "after get_boxed_value, checking value, long is " << extracted_long << std::endl;
    if (l1 != extracted_long)
    {
      TAOX11_TEST_ERROR << "ERROR: Failed should have been" << l1 << std::endl;
      return 1;
    }
    extracted_long = 23;
    any_bv <<= extracted_long;
    davb1->set_boxed_value(any_bv);
    TAOX11_TEST_DEBUG << "after set_boxed_value, checking value, long is " << davb1->get_long() << std::endl;
    if (davb1->get_long() != 23 )
    {
      TAOX11_TEST_ERROR << "ERROR: Failed set_boxed_value." << std::endl;
    }

    TAOX11_TEST_DEBUG << "A21. get_boxed_value_as_dyn_any and set_boxed_value_as_dyn_any_as_dyn_any " << std::endl;
    IDL::traits<DynamicAny::DynAny>::ref_type  dynany_long = davb1->get_boxed_value_as_dyn_any();
    if (dynany_long->get_long() != 23 )
    {
      TAOX11_TEST_ERROR << "ERROR: Failed get_boxed_value_as_dyn_any." << std::endl;
    }
    dynany_long->insert_long(34);
    davb1->set_boxed_value_as_dyn_any(dynany_long);
    extracted_long = davb1->get_long();
    TAOX11_TEST_DEBUG << "after set_boxed_value_as_dyn_any, checking value, long is " << extracted_long << std::endl;
    if (extracted_long != 34 )
    {
      TAOX11_TEST_ERROR << "ERROR: set_boxed_value_as_dyn_any." << std::endl;
    }

    ////////////////////////////////////////////
    // DynValue_Test::NestedValue
    ////////////////////////////////////////////

    int16_t const s1 = -17;
    int16_t const s2 = -23;
    TAOX11_TEST_DEBUG <<"B *=*=*=*= DynValue_Test::NestedValue =*=*=*=*" << std::endl;
    IDL::traits<DynValue_Test::NestedValue>::ref_type myNestedValue =
        CORBA::make_reference <IDL::traits<DynValue_Test::NestedValue>::obv_type>  (s1, s2);

    TAOX11_TEST_DEBUG << "B1.checking value, shorts are " << myNestedValue->Nested_s1 ()
                     << "," << myNestedValue->Nested_s2() << std::endl;
    if (s1 != myNestedValue->Nested_s1 () ||
        s2 != myNestedValue->Nested_s2 ())
    {
      TAOX11_TEST_ERROR << " FAILED should have been " << s1 << " ," << s2 << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B2.Inserting into an any" << std::endl;
    myAny <<= myNestedValue;

    TAOX11_TEST_DEBUG << "B3.Extracting from an any" << std::endl;
    IDL::traits<DynValue_Test::NestedValue>::ref_type myNestedValueExtracted =
        CORBA::make_reference <IDL::traits<DynValue_Test::NestedValue>::obv_type> (9,9);

    if (!(myAny >>= myNestedValueExtracted))
    {
      TAOX11_TEST_ERROR << " FAILED extraction" << std::endl;
      return 1;
    }
    if (!myNestedValueExtracted)
    {
      TAOX11_TEST_ERROR << " FAILED null pointer extracted" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B4.checking value, shorts are " << myNestedValueExtracted->Nested_s1 ()
                      << "," << myNestedValueExtracted->Nested_s2 ()<< std::endl;
    if (s1 != myNestedValueExtracted->Nested_s1 () ||
        s2 != myNestedValueExtracted->Nested_s2 ()   )
    {
      TAOX11_TEST_ERROR << " Error, Failed should have been ," << s1 << " ," << s2 << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B5. Creating dynamic any from any" << std::endl;
    IDL::traits<DynamicAny::DynAny>::ref_type dany_nested = dany_fact_->create_dyn_any  (myAny);
    analyzer.analyze (dany_nested);

    TAOX11_TEST_DEBUG << "B6.Testing dynamic any for non-null" << std::endl;
    dvc = IDL::traits<DynamicAny::DynValueCommon>::narrow(dany_nested);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << " FAILURE can not obtain DynamicAny::DynValueCommon *" << std::endl;
      return 1;
    }
    if (dvc->is_null ())
    {
      TAOX11_TEST_ERROR << "Error, Failed this dynamic any is null!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B7.Creating dynamic any from IDL generated typecode" << std::endl;
    dany = dany_fact_->create_dyn_any_from_type_code (DynValue_Test::_tc_NestedValue);
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "B8.Testing typecode generated dynamic any for null" << std::endl;
    dvc= IDL::traits<DynamicAny::DynValueCommon>::narrow(dany);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << " FAILURE can not obtain DynamicAny::DynValueCommon" << std::endl;
      return 1;
    }
    if (!(dvc->is_null ()))
    {
      TAOX11_TEST_ERROR << " FAILED this dynamic any has a value!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B9.Testing for inequality with original dynamic any" << std::endl;
    if (dany->equal (dany_nested))
    {
      TAOX11_TEST_ERROR << " FAILED they test equal before setting shorts" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B10.Setting to value and test for inequality with original dynamic any" << std::endl;
    dvc->set_to_value ();
    analyzer.analyze (dany);
    if (dany->equal (dany_nested))
    {
      TAOX11_TEST_ERROR << " FAILED they test equal before setting shorts" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B11.Setting the shorts of the typecode generated dynamic any" << std::endl;
    dany->rewind ();
    dany->insert_short (s1);
    if (!(dany->next ()))
    {
      TAOX11_TEST_ERROR << "Error, Failed to move on to second short" << std::endl;
      return 1;
    }
    dany->insert_short (s2);
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "B12.Testing for equality with original dynamic any" << std::endl;
    if (!(dany->equal (dany_nested)))
    {
      TAOX11_TEST_ERROR << " FAILED they test unequal after setting shorts" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B13.Setting typecode dynamic any back to null" << std::endl;
    dvc->set_to_null ();
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "B14.Setting the original dynamic any to null object" << std::endl;
    dvc= IDL::traits<DynamicAny::DynValueCommon>::narrow(dany_nested);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << " FAILURE can not obtain DynamicAny::DynValueCommon *" << std::endl;
      return 1;
    }
    dvc->set_to_null ();
    analyzer.analyze (dany_nested);

    TAOX11_TEST_DEBUG << "B15.Testing for equality with typecode dynamic any" << std::endl;
    if (!(dany->equal (dany_nested)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B16.Converting from null dynamic any back to any" << std::endl;
    myAny2 = dany_nested->to_any ();

    TAOX11_TEST_DEBUG << "B17. Extracting from this any" << std::endl;
    myNestedValueExtracted=CORBA::make_reference <IDL::traits<DynValue_Test::NestedValue>::obv_type> (9,9);
    if (!(myAny2 >>= myNestedValueExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed extraction" << std::endl;
      return 1;
    }
    if (myNestedValueExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed non-null pointer extracted" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "b18. Setting the dynamic any to VALUE object" << std::endl;
    dvc->set_to_value ();
    analyzer.analyze (dany_nested);

    TAOX11_TEST_DEBUG << "B19. Resetting typecode dynamic any from original any" << std::endl;
    dany->from_any (myAny);
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "Testing for equality with original dynamic any" << std::endl;
    if (!(dany->equal (dany_nested)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B20. Converting from dynamic any back to any" << std::endl;
    myAny2 = dany_nested->to_any ();

    TAOX11_TEST_DEBUG << "Attempting to extract from this any (without factory)" << std::endl;
    myNestedValueExtracted=CORBA::make_reference <IDL::traits<DynValue_Test::NestedValue>::obv_type> (9,9);
    TAOX11_TEST_DEBUG << "Attempting to extract from this any (without factory)" << std::endl;
    try
    {
      if (myAny2 >>= myNestedValueExtracted)
      {
       TAOX11_TEST_ERROR << " FAILED the extraction occurred without factory" << std::endl;
       return 1;
      }
    }
    catch (... )
    {
      // Expected with no factory registered
    }
    TAOX11_TEST_DEBUG << "B21. Attempting to get_val from dynamic any (without factory)" << std::endl;
    try
    {
      myNestedValueExtracted=IDL::traits<DynValue_Test::NestedValue>::narrow (
            dany_nested->get_val ());
      if (myNestedValueExtracted)
      {
        TAOX11_TEST_ERROR << "ERROR: Failed get_val successful without factory" << std::endl;
        return 1;
      }
    }
    catch (...)
    {
      // Expected with no factory registered
    }
    IDL::traits<DynValue_Test::NestedValue >::factory_ref_type va_factory3 =
       CORBA::make_reference<DynValue_Test::NestedValue_init > ();

    orb_->register_value_factory (va_factory3->_obv_repository_id (),
       va_factory3);

    TAOX11_TEST_DEBUG << "B22. Extracting from this any (now with factory)" << std::endl;
    if (!(myAny2 >>= myNestedValueExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed the extraction" << std::endl;
      return 1;
    }
    if (!myNestedValueExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed null pointer extracted" << std::endl;
      return 1;
     }
     TAOX11_TEST_DEBUG << "  checking value, shorts are "
                       << myNestedValueExtracted->Nested_s1 ()
                       << "," << myNestedValueExtracted->Nested_s2 ()
                       << std::endl;
    if (s1 != myNestedValueExtracted->Nested_s1 () ||
        s2 != myNestedValueExtracted->Nested_s2 ()   )
    {
      TAOX11_TEST_ERROR << " Error, Expected " << s1 << "," << s2 << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "B23. Attempting to get_val from dynamic any (now with factory)" << std::endl;
    myNestedValueExtracted=
        IDL::traits<DynValue_Test::NestedValue>::narrow(dany_nested->get_val ());
    if (!myNestedValueExtracted)
    {
      TAOX11_TEST_ERROR << " ERROR: FAILED get_val unsuccessful even with factory" << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "   checking value, shorts are "
                    << myNestedValueExtracted->Nested_s1 ()
                    << "," << myNestedValueExtracted->Nested_s2 ()
                    << std::endl;
    if (s1 != myNestedValueExtracted->Nested_s1 () ||
        s2 != myNestedValueExtracted->Nested_s2 ()   )
    {
      TAOX11_TEST_ERROR << " Error, Expected " << s1 << "," << s2 << std::endl;
      return 1;
    }

    ////////////////////////////////////////////
    // DynValue_Test::NullValue
    ////////////////////////////////////////////

    TAOX11_TEST_DEBUG <<"C *=*=*=*= DynValue_Test::NullValue =*=*=*=*" << std::endl;
    IDL::traits<DynValue_Test::NullValue>::ref_type myNullValue =
        CORBA::make_reference <IDL::traits<DynValue_Test::NullValue>::obv_type>  ();

    TAOX11_TEST_DEBUG << "C1. Inserting into an any" << std::endl;
    myAny <<= myNullValue;

    TAOX11_TEST_DEBUG << "C2. Extracting from an any" << std::endl;
    IDL::traits<DynValue_Test::NullValue>::ref_type myNullValueExtracted =
       CORBA::make_reference <IDL::traits<DynValue_Test::NullValue>::obv_type>  ();
    if (!(myAny >>= myNullValueExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed extraction" << std::endl;
      return 1;
    }
    if (!myNullValueExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed null pointer extracted" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "C3. Creating dynamic any from any" << std::endl;
    IDL::traits<DynamicAny::DynAny>::ref_type dany_null  = dany_fact_->create_dyn_any  (myAny);
    analyzer.analyze (dany_null);

    TAOX11_TEST_DEBUG << "Testing dynamic any for non-null" << std::endl;
    dvc= IDL::traits<DynamicAny::DynValueCommon >::narrow(dany_null);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << " Error: FAILURE can not obtain DynamicAny::DynValueCommon *" << std::endl;
      return 1;
    }
    if (dvc->is_null ())
    {
      TAOX11_TEST_ERROR << "Error, Failed this dynamic any is null!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "C4. Creating dynamic any from IDL generated typecode" << std::endl;
    dany = dany_fact_->create_dyn_any_from_type_code (DynValue_Test::_tc_NullValue);
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "Testing typecode generated dynamic any for null" << std::endl;
    dvc= IDL::traits<DynamicAny::DynValueCommon >::narrow (dany);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << "ERROR: FAILURE can not obtain DynamicAny::DynValueCommon *" << std::endl;
      return 1;
    }
    if (!(dvc->is_null ()))
    {
      TAOX11_TEST_ERROR << "Error, Failed this dynamic any has a value!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "C5. Testing for inequality with original dynamic any" << std::endl;
    if (dany->equal (dany_null))
    {
      TAOX11_TEST_ERROR << " FAILED they test equal before setting to a value" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "Setting to value and test for equality with original dynamic any" << std::endl;
    dvc->set_to_value ();
    analyzer.analyze (dany);
    if (!(dany->equal (dany_null)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "Setting typecode dynamic any back to null" << std::endl;
    dvc->set_to_null ();
    analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "C6. Setting the original dynamic any to null object" << std::endl;
    dvc= IDL::traits<DynamicAny::DynValueCommon >::narrow(dany_null);
    if (!dvc)
    {
      TAOX11_TEST_ERROR << "ERROR: FAILURE can not obtain DynamicAny::DynValueCommon" << std::endl;
      return 1;
    }
    dvc->set_to_null ();
    analyzer.analyze (dany_null);

    TAOX11_TEST_DEBUG << "Testing for equality with typecode dynamic any" << std::endl;
    if (!(dany->equal (dany_null)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "C7. Converting from null dynamic any back to any" << std::endl;
    myAny2= dany_null->to_any ();

    TAOX11_TEST_DEBUG << "Extracting from this any" << std::endl;
    myNullValueExtracted= CORBA::make_reference <IDL::traits<DynValue_Test::NullValue>::obv_type> ();
    if (!(myAny2 >>= myNullValueExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed extraction" << std::endl;
      return 1;
    }
    if (myNullValueExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed non-null pointer extracted" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "C8.  Setting the original dynamic any to VALUE object" << std::endl;
    dvc->set_to_value ();
     analyzer.analyze (dany_null);

    TAOX11_TEST_DEBUG << "Resetting typecode dynamic any from original any" << std::endl;
    dany->from_any (myAny);
     analyzer.analyze (dany);

    TAOX11_TEST_DEBUG << "Testing for equality with original dynamic any" << std::endl;
    if (!(dany->equal (dany_null)))
    {
      TAOX11_TEST_ERROR << "Error, Failed they test unequal!" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "Converting from dynamic any back to any" << std::endl;
    myAny2 = dany_null->to_any ();

    TAOX11_TEST_DEBUG << "Attempting to extract from this any (without factory)" << std::endl;
    myNullValueExtracted= CORBA::make_reference <IDL::traits<DynValue_Test::NullValue>::obv_type> ();
    try{
      if (myAny2 >>= myNullValueExtracted)
      {
        TAOX11_TEST_ERROR << "Error, Failed the extraction occurred without factory" << std::endl;
        return 1;
      }
    }
    catch (...)
    {
      // Expected with no factory registered
    }
    TAOX11_TEST_DEBUG << "C9.  Attempting to get_val from dynamic any (without factory)" << std::endl;
    try
    {
      myNullValueExtracted= IDL::traits<DynValue_Test::NullValue>::narrow (dany_null->get_val ());
      if (myNullValueExtracted)
      {
        TAOX11_TEST_ERROR << "Error, Failed get_val successful without factory" << std::endl;
        return 1;
      }
    }
    catch (...)
    {
      // Expected with no factory registered
    }
    IDL::traits<DynValue_Test::NullValue >::factory_ref_type va_factory4 =
       CORBA::make_reference<DynValue_Test::NullValue_init > ();

    orb_->register_value_factory (va_factory4->_obv_repository_id (),
                                  va_factory4);
    TAOX11_TEST_DEBUG << "C10. Extracting from this any (now with factory)" << std::endl;
    if (!(myAny2 >>= myNullValueExtracted))
    {
      TAOX11_TEST_ERROR << "Error, Failed the extraction" << std::endl;
      return 1;
    }
    if (!myNullValueExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed null pointer extracted" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "C11. Attempting to get_val from dynamic any (now with factory)" << std::endl;
    myNullValueExtracted= IDL::traits<DynValue_Test::NullValue>::narrow(dany_null->get_val ());
    if (!myNullValueExtracted)
    {
      TAOX11_TEST_ERROR << "Error, Failed get_val unsuccessful even with factory" << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "All tests passed" << std::endl;
  }
  catch (const CORBA::TRANSIENT &)
  {
    TAOX11_TEST_DEBUG << "Error : Received CORBA::TRANSIENT" << std::endl;
    return 1;
  }
  catch (const CORBA::COMM_FAILURE &)
  {
    TAOX11_TEST_ERROR << "Error : Received CORBA::COMM_FAILURE" << std::endl;
    return 1;
  }
  catch (const CORBA::Exception & ex)
  {
    TAOX11_TEST_ERROR << "Tests failed with CORBA exception:" << ex << std::endl;
    return 1;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "Test failed with NON-CORBA exception" << std::endl;
  }
  orb_->destroy ();
  return 0;
}
