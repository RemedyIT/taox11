/**
 * @file    test.cpp
 * @author  Marijke Hengstmengel
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/typecodefactory/typecodefactory.h"
#include "tao/x11/ifr_client/IFR_BaseC.h"
#include "tao/x11/ifr_client/IFR_ExtendedC.h"
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/object.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/VisibilityC.h"
#include "testlib/taox11_testlog.h"


int main( int argc, char* argv[] )
{
  int retcode = 0;

  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
      return 1;
    }

    TAOX11_TEST_INFO << "going to test  creating typecodes with the typcodefactory_adapter functions..." << std::endl;

    uint32_t bound = 12;
    CORBA::object_reference<CORBA::TypeCode> string_tc =
        _orb->create_string_tc (bound);

    if ((string_tc->kind() != CORBA::TCKind::tk_string) ||
        (string_tc->length() != 12))
    {
      TAOX11_TEST_ERROR << "ERROR  create_string_tc "
                        << " kind (tk_string) : " << string_tc->kind()
                        << " length (12) : " << string_tc->length() << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  create_string_tc " << std::endl;
    }

    bound = 0;
    CORBA::object_reference<CORBA::TypeCode> u_string_tc =
        _orb->create_string_tc (bound);

    if ((u_string_tc->kind() != CORBA::TCKind::tk_string) ||
        (u_string_tc->length() != 0))
    {
      TAOX11_TEST_ERROR << "ERROR  create_string_tc unbounded"
                        << " kind (tk_string) : " << u_string_tc->kind()
                        << " length (0) : " << u_string_tc->length() << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  create_string_tc unbounded" << std::endl;
    }

    bound = 120000;
    CORBA::object_reference<CORBA::TypeCode> wstring_tc =
          _orb->create_wstring_tc (bound);

    if ((wstring_tc->kind() != CORBA::TCKind::tk_wstring) ||
        (wstring_tc->length() != 120000))
    {
      TAOX11_TEST_ERROR << "ERROR  create_wstring_tc "
                        << " kind (tk_wstring) : " << wstring_tc->kind()
                        << " length (120000) : " << wstring_tc->length() << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  create_wstring_tc " << std::endl;
    }

    CORBA::object_reference<CORBA::TypeCode> fixed_tc =
            _orb->create_fixed_tc (10, 11);

    if ((fixed_tc->kind() != CORBA::TCKind::tk_fixed) ||
        (fixed_tc->fixed_digits() != 10 ) ||
        (fixed_tc->fixed_scale() != 11))
    {
      TAOX11_TEST_ERROR << "ERROR  create_fixed_tc "
                        << " digits (10) : " << fixed_tc->fixed_digits()
                        << " scale (11) :" << fixed_tc->fixed_scale() << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_fixed_tc" << std::endl;

    CORBA::object_reference<CORBA::TypeCode> alias_string_tc =
        _orb->create_alias_tc ("IDL:foo:1.0","foo",string_tc);

    if ((alias_string_tc->kind() != CORBA::TCKind::tk_alias) ||
        (alias_string_tc->content_type()->kind() != CORBA::TCKind::tk_string) ||
        (alias_string_tc->name() != "foo") ||
        (alias_string_tc->id() != "IDL:foo:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_alias_tc "
                        << " kind (tk_alias) : " << alias_string_tc->kind()
                        << " content_type (tk_string) :" << alias_string_tc->content_type()->kind()
                        << " name : " << alias_string_tc->name()
                        << " id : " << alias_string_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_alias_tc" << std::endl;

    CORBA::object_reference<CORBA::TypeCode> interface_tc =
        _orb->create_interface_tc ("IDL:foo:1.7","foo_interface");

    if ((interface_tc->kind() != CORBA::TCKind::tk_objref) ||
        (interface_tc->name() != "foo_interface") ||
        (interface_tc->id() != "IDL:foo:1.7"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_interface_tc "
                        << " kind (tk_objref) : " << interface_tc->kind()
                        << " name : " << interface_tc->name()
                        << " id : " << interface_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_interface_tc" << std::endl;

    //test wrong id, expect exception with minor ocde 16
    bool received = false;
    try {


      CORBA::object_reference<CORBA::TypeCode> interface_tc_excep =
        _orb->create_interface_tc ("lala","foo");

    }
    catch(const CORBA::BAD_PARAM & ex)
    {
      TAOX11_TEST_DEBUG << "OK  Found expected exception with minor"
                        << (ex.minor() & 0xFFFU) << std::endl;
      received = true;
      if ((ex.minor() & 0xFFFU) != 16)
      {
        TAOX11_TEST_ERROR << "ERROR  in expected exception create_interface_tc "
                          << " minor (16) : " << (ex.minor() & 0xFFFU)
                          << std::endl;
        retcode = 1;
      }
    }
    if(!received)
    {
      TAOX11_TEST_ERROR << "ERROR  Not found expected exception "
                        << " for create_interface_tc" << std::endl;
      retcode = 1;
    }


    CORBA::object_reference<CORBA::TypeCode> abstract_interface_tc =
         _orb->create_abstract_interface_tc ("IDL:foo:1.0","foo");

    if ((abstract_interface_tc->kind() != CORBA::TCKind::tk_abstract_interface) ||
        (abstract_interface_tc->name() != "foo") ||
        (abstract_interface_tc->id() != "IDL:foo:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_abstract_interface_tc "
                        << " kind (tk_abstract_interface) : " << abstract_interface_tc->kind()
                        << " name : " << abstract_interface_tc->name()
                        << " id : " << abstract_interface_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_abstract_interface_tc" << std::endl;

    CORBA::object_reference<CORBA::TypeCode> local_interface_tc =
             _orb->create_local_interface_tc ("IDL:foo:1.0","foo");

    if ((local_interface_tc->kind() != CORBA::TCKind::tk_local_interface) ||
        (local_interface_tc->name() != "foo") ||
        (local_interface_tc->id() != "IDL:foo:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_local_interface_tc "
                        << " kind (tk_local_interface) : " << local_interface_tc->kind()
                        << " name : " << local_interface_tc->name()
                        << " id : " << local_interface_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_local_interface_tc" << std::endl;



    CORBA::object_reference<CORBA::TypeCode> component_tc =
             _orb->create_component_tc ("IDL:bar:1.0","foo_comp");

    if ((component_tc->kind() != CORBA::TCKind::tk_component) ||
        (component_tc->name() != "foo_comp") ||
        (component_tc->id() != "IDL:bar:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_component_tc "
             << " kind (tk_component) : "
             << component_tc->kind()
             << " name : " << component_tc->name()
             << " id : " << component_tc->id()
            << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_component_tc" << std::endl;


    CORBA::object_reference<CORBA::TypeCode> home_tc =
             _orb->create_home_tc ("IDL:foo:1.0","foo");

    if ((home_tc->kind() != CORBA::TCKind::tk_home) ||
        (home_tc->name() != "foo") ||
        (home_tc->id() != "IDL:foo:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_home_tc "
                        << " kind (tk_home) : "
                        << home_tc->kind()
                        << " name : " << home_tc->name()
                        << " id : " << home_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
      TAOX11_TEST_DEBUG << "OK  create_home_tc" << std::endl;

    CORBA::StructMember one = CORBA::StructMember("one", string_tc, nullptr);
    CORBA::StructMember two = CORBA::StructMember ("two", interface_tc, nullptr);
    CORBA::StructMemberSeq struct_members {one, two};

    CORBA::object_reference<CORBA::TypeCode> struct_tc =
         _orb->create_struct_tc ("IDL:lala:1.0", "MyStruct", struct_members);

    if ((struct_tc->kind() != CORBA::TCKind::tk_struct) ||
        (struct_tc->member_count() != 2) ||
        (struct_tc->name() != "MyStruct") ||
        (struct_tc->id() != "IDL:lala:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_struct_tc "
                        << " kind (tk_struct) : " << struct_tc->kind()
                        << " member_count (2) : " << struct_tc->member_count()
                        << " name : " << struct_tc->name()
                        << " id : " << struct_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      if ((struct_tc->member_type(0)->kind() != CORBA::TCKind::tk_string) ||
          (struct_tc->member_type(1)->kind() != CORBA::TCKind::tk_objref) ||
          (struct_tc->member_name(0) != "one") ||
          (struct_tc->member_name(1) != "two"))
      {
        TAOX11_TEST_ERROR << "ERROR  create_struct_tc, error in members: " << std::endl;

        for (uint32_t count = 0; count < struct_tc->member_count(); count++ )
        {
          TAOX11_TEST_DEBUG << "Found typecode: tk_struct member [" << struct_tc->member_name(count)
                            << "] kind : " << struct_tc->member_type(count)->kind()<< std::endl;
        }
        retcode = 1;
      }
      else
        TAOX11_TEST_DEBUG << "OK  create_struct_tc" << std::endl;
    }

    received = false;
    one = CORBA::StructMember("one", CORBA::_tc_void, nullptr);
    CORBA::StructMemberSeq struct_members_except {one};
    try
    {
      CORBA::object_reference<CORBA::TypeCode> struct_tc_except =
         _orb->create_struct_tc ("IDL:lala:1.0", "MyStruct", struct_members_except);
    }
    catch(const CORBA::BAD_TYPECODE & ex)
    {
      TAOX11_TEST_DEBUG << "OK  Found expected exception with minor"
                        << (ex.minor() & 0xFFFU) << std::endl;
      received = true;
      if ((ex.minor() & 0xFFFU) != 2)
      {
        TAOX11_TEST_ERROR << "ERROR  in expected exception create_struct_tc "
                          << " minor (2) : " << (ex.minor() & 0xFFFU)
                          << std::endl;
        retcode = 1;
      }
    }
    if(!received)
    {
      TAOX11_TEST_ERROR << "ERROR  Not found expected exception "
                        << " for create_struct_tc" << std::endl;
      retcode = 1;
    }

    CORBA::StructMember exceptionone = CORBA::StructMember("one", CORBA::_tc_string, nullptr);
    CORBA::StructMember exceptiontwo = CORBA::StructMember ("", CORBA::_tc_short, nullptr);
    CORBA::StructMemberSeq exception_members {exceptionone, exceptiontwo};


    CORBA::object_reference<CORBA::TypeCode> exception_tc =
         _orb->create_exception_tc ("IDL:lala:1.0", "MyException", exception_members);

    if ((exception_tc->kind() != CORBA::TCKind::tk_except) ||
        (exception_tc->member_count() != 2) ||
        (exception_tc->name() != "MyException") ||
        (exception_tc->id() != "IDL:lala:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_exception_tc "
                        << " kind (tk_except) : " << exception_tc->kind()
                        << " member_count (2) : " << exception_tc->member_count()
                        << " name : " << exception_tc->name()
                        << " id : " << exception_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      if ((exception_tc->member_type(0)->kind() != CORBA::TCKind::tk_string) ||
          (exception_tc->member_type(1)->kind() != CORBA::TCKind::tk_short) ||
          (exception_tc->member_name(0) != "one") ||
          (exception_tc->member_name(1) != ""))
      {
        TAOX11_TEST_ERROR << "ERROR  create_exception_tc, error in members: " << std::endl;

        for (uint32_t count = 0; count < exception_tc->member_count(); count++ )
        {
          TAOX11_TEST_DEBUG << "Found typecode: tk_except member [" << exception_tc->member_name(count)
                            << "] kind : " << exception_tc->member_type(count)->kind()<< std::endl;
        }
        retcode = 1;
      }
      else
        TAOX11_TEST_DEBUG << "OK  create_exception_tc" << std::endl;
    }

    int16_t label = 1;
    CORBA::Any union_label;
    union_label <<= label;
    CORBA::UnionMember union_one = CORBA::UnionMember("string_member", union_label, string_tc, nullptr);
    label = 2;
    union_label <<= label;
    CORBA::UnionMember union_two = CORBA::UnionMember("iface_member", union_label, interface_tc, nullptr);
    label = 3;
    union_label <<= label;
    CORBA::UnionMember union_three = CORBA::UnionMember("string_member2", union_label, string_tc, nullptr);

    CORBA::UnionMemberSeq union_members {union_one, union_two, union_three};

    CORBA::object_reference<CORBA::TypeCode> union_tc =
            _orb->create_union_tc ("IDL:foo:1.0",
                                   "foo",
                                   CORBA::_tc_short,
                                   union_members);

    if ((union_tc->kind() != CORBA::TCKind::tk_union) ||
        (union_tc->discriminator_type ()->kind() != CORBA::TCKind::tk_short) ||
        (union_tc->name() != "foo") ||
        (union_tc->id() != "IDL:foo:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_union_tc "
                        << " kind (tk_union) : " << union_tc->kind()
                        << " discriminator_type (tk_short) : " << union_tc->discriminator_type ()->kind()
                        << " name : " << union_tc->name()
                        << " id : " << union_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
    {
       CORBA::Any label_;
       int16_t label;
       union_tc->member_label(2)>>=label;
       if ((union_tc->member_type(0)->kind() != CORBA::TCKind::tk_string) ||
           (union_tc->member_type(1)->kind() != CORBA::TCKind::tk_objref) ||
           (union_tc->member_name(0) != "string_member") ||
           (union_tc->member_name(2) != "string_member2") ||
           (label != 3))
       {
         TAOX11_TEST_ERROR << "ERROR  create_union_tc, error in members: " << std::endl;
         for (uint32_t count = 0; count < union_tc->member_count(); count++ )
         {
           TAOX11_TEST_DEBUG << "Found typecode: tk_struct member [" << union_tc->member_name(count)
                             << "] label : " << union_tc->member_label(count)
                             << "] kind : " << union_tc->member_type(count)->kind()<< std::endl;
         }
         retcode = 1;
       }
       else
         TAOX11_TEST_DEBUG << "OK  create_union_tc" << std::endl;
    }
    received = false;
    // expect exception because member name is used twice for different member types
    try{
      label = 1;
      union_label <<= label;
      CORBA::UnionMember union_one_excep = CORBA::UnionMember("string_member", union_label, string_tc, nullptr);
      label = 2;
      union_label <<= label;
      CORBA::UnionMember union_two_excep = CORBA::UnionMember("string_member", union_label, interface_tc, nullptr);

      CORBA::UnionMemberSeq union_members_excep {union_one_excep, union_two_excep};

      CORBA::object_reference<CORBA::TypeCode> union_tc_excep =
              _orb->create_union_tc ("IDL:foo:1.0",
                                     "foo",
                                     CORBA::_tc_short,
                                     union_members_excep);

    }
    catch(const CORBA::BAD_PARAM & ex)
    {
      TAOX11_TEST_DEBUG << "OK  Found expected exception, with minor code " << (ex.minor() & 0xFFFU)
                         << std::endl;
      received = true;
      if ((ex.minor() & 0xFFFU) != 17)
      {
        TAOX11_TEST_ERROR << "ERROR  in expected exception create_union_tc "
                          << " minor (17) : " << (ex.minor() & 0xFFFU)
                          << std::endl;
        retcode = 1;
      }
    }
    if(!received)
    {
      TAOX11_TEST_ERROR << "ERROR  Not found expected exception "
                        << " for create_union_tc" << std::endl;
      retcode = 1;
    }

    CORBA::EnumMemberSeq eseq {"ALARM_EVENT", "LOG_EVENT"};
    CORBA::object_reference<CORBA::TypeCode> enum_tc =
             _orb->create_enum_tc ("IDL:foo_enum:1.0",
                                   "foo_enum",
                                   eseq);

    if ((enum_tc->kind() != CORBA::TCKind::tk_enum) ||
        (enum_tc->name() != "foo_enum") ||
        (enum_tc->id() != "IDL:foo_enum:1.0") ||
        (enum_tc->member_count() != 2) ||
        (enum_tc->member_name(0) != "ALARM_EVENT") ||
        (enum_tc->member_name(1) != "LOG_EVENT"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_enum_tc  "
                        << " kind (tk_enum) : " << enum_tc->kind()
                        << " name (foo_enum) : " << enum_tc->name()
                        << " id (IDL:foo_enum:1.0) : " << enum_tc->id()
                        << " member_count (2) : " << enum_tc->member_count()
                        << " member_name (ALARM_EVENT) : " <<  enum_tc->member_name(0)
                        << " member_name (LOG_EVENT) : " <<  enum_tc->member_name(1)
                        << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  create_enum_tc" << std::endl;
    }

    CORBA::object_reference<CORBA::TypeCode> seq_tc =
                 _orb->create_sequence_tc (3, CORBA::_tc_short);

    if ((seq_tc->kind() != CORBA::TCKind::tk_sequence) ||
        (seq_tc->length() != 3) ||
        (seq_tc->content_type()->kind() !=  CORBA::TCKind::tk_short))
    {
      TAOX11_TEST_ERROR << "ERROR  create_sequence_tc "
                        << " kind (tk_sequence) : " << seq_tc->kind()
                        << " length (3) : " << seq_tc->length()
                        << " content type (tk_short) :" << seq_tc->content_type()->kind()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  create_sequence_tc" << std::endl;
    }

    CORBA::object_reference<CORBA::TypeCode> array_tc =
                 _orb->create_array_tc (4, CORBA::_tc_long);

    if ((array_tc->kind() != CORBA::TCKind::tk_array)||
        (array_tc->length() != 4) ||
        ( array_tc->content_type()->kind() !=  CORBA::TCKind::tk_long))
    {
      TAOX11_TEST_ERROR << "ERROR  create_array_tc "
                        << " kind (tk_array) : " << array_tc->kind()
                        << " length (4) : " << array_tc->length()
                        << " content type (tk_long) :" <<  array_tc->content_type()->kind()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  create_array_tc" << std::endl;
    }

    CORBA::object_reference<CORBA::TypeCode> recursive_tc =  _orb->create_recursive_tc ("IDL:foo:1.0");
    CORBA::ValueMember one_value = CORBA::ValueMember("one", "one_id", "nothing","unknown",recursive_tc, nullptr, CORBA::PUBLIC_MEMBER);
    CORBA::ValueMember two_value = CORBA::ValueMember("two", "two_id", "??","1.2",CORBA::_tc_short, nullptr,CORBA::PRIVATE_MEMBER);
    CORBA::ValueMemberSeq value_members {one_value, two_value};
    CORBA::object_reference<CORBA::TypeCode> value_tc =
        _orb->create_value_tc ("IDL:foo:1.0", "MyValue", CORBA::VM_CUSTOM, CORBA::_tc_null, value_members);

    if ((value_tc->kind() != CORBA::TCKind::tk_value) ||
        (value_tc->member_count() != 2) ||
        (value_tc->name() != "MyValue") ||
        (value_tc->id() != "IDL:foo:1.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_value_tc "
                        << " kind (tk_value) : " << value_tc->kind()
                        << " member count (2) : " << value_tc->member_count()
                        << " name : " << value_tc->name()
                        << " id : " << value_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  Found typecode: tk_value" << value_tc->kind() << std::endl;
      if ((value_tc->member_name(0) != "one") ||
          (value_tc->member_name(1) != "two") ||
          (value_tc->member_type(1)->kind()!= CORBA::TCKind::tk_short) ||
          (value_tc->member_visibility(0) != CORBA::PUBLIC_MEMBER) ||
          (value_tc->member_visibility(1) != CORBA::PRIVATE_MEMBER))
      {
        TAOX11_TEST_ERROR << "ERROR   create_value_tc in members "
                          << " name ('one') : " << value_tc->member_name(0)
                          << " name ('two') : " << value_tc->member_name(1)
                          << " kind (tk_short) : " << value_tc->member_type(1)->kind()
                          << " Visibility ('1') : " << value_tc->member_visibility(0)
                          << " Visibility ('0') : " << value_tc->member_visibility(1)
                          << std::endl;
        retcode = 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "OK  create_value_tc" << std::endl;
      }
    }

    CORBA::object_reference<CORBA::TypeCode> value_box_tc =
                 _orb->create_value_box_tc ("IDL:foo:1.1","valuebox", CORBA::_tc_long);

    if ((value_box_tc->kind() != CORBA::TCKind::tk_value_box) ||
        (value_box_tc->content_type()->kind() != CORBA::TCKind::tk_long) ||
        (value_box_tc->name() != "valuebox") ||
        (value_box_tc->id() != "IDL:foo:1.1"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_value_box_tc "
                        << "kind (tk_value_box) : " << value_box_tc->kind()
                        << " content_type (tc_long) " << value_box_tc->content_type()->kind()
                        << " name : " << value_box_tc->name()
                        << " id : " << value_box_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK create_value_box_tc" << std::endl;
    }

    CORBA::object_reference<CORBA::TypeCode> native_tc =
                _orb->create_native_tc ("IDL:foo:1.1","native");

    if ((native_tc->kind() != CORBA::TCKind::tk_native) ||
        (native_tc->name() != "native") ||
        (native_tc->id() != "IDL:foo:1.1"))
      {
        TAOX11_TEST_ERROR << "ERROR  create_native_tc  "
                          << " kind (tk_native) : "<< native_tc->kind()
                          << " name ('native') : " << native_tc->name()
                          << " id ('IDL:foo:1.1') : " << native_tc->id()
                          << std::endl;
        retcode = 1;
      }
    else
      TAOX11_TEST_DEBUG << "OK  create_native_tc" << std::endl;

    CORBA::ValueMember one_event = CORBA::ValueMember("one", "one_id", "nothing","unknown",CORBA::_tc_string, nullptr, CORBA::PUBLIC_MEMBER);
    CORBA::ValueMember two_event = CORBA::ValueMember("two", "two_id", "??","1.2",CORBA::_tc_short, nullptr,CORBA::PRIVATE_MEMBER);
    CORBA::ValueMemberSeq event_members {one_event, two_event};
    CORBA::object_reference<CORBA::TypeCode> event_tc =
        _orb->create_event_tc ("IDL:foo:2.0", "MyEvent", CORBA::VM_NONE, CORBA::_tc_null, event_members);


    if ((event_tc->kind() != CORBA::TCKind::tk_event) ||
        (event_tc->member_count() != 2) ||
        (event_tc->name() != "MyEvent") ||
        (event_tc->id() != "IDL:foo:2.0"))
    {
      TAOX11_TEST_ERROR << "ERROR  create_event_tc "
                        << " kind (tk_event) : " << event_tc->kind()
                        << " member count (2) : " << event_tc->member_count()
                        << " name : " << event_tc->name()
                        << " id : " << event_tc->id()
                        << std::endl;
      retcode = 1;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK  Found typecode: tk_event" << event_tc->kind() << std::endl;
      if ((event_tc->member_name(0) != "one") ||
          (event_tc->member_name(1) != "two") ||
          (event_tc->member_type(0)->kind()!= CORBA::TCKind::tk_string) ||
          (event_tc->member_visibility(0) != CORBA::PUBLIC_MEMBER) ||
          (event_tc->member_visibility(1) != CORBA::PRIVATE_MEMBER))
      {
        TAOX11_TEST_ERROR << "ERROR   create_event_tc in members "
                          << " name ('one') : " << event_tc->member_name(0)
                          << " name ('two') : " << event_tc->member_name(1)
                          << " kind (tc_string) : " << event_tc->member_type(0)->kind()
                          << " Visibility ('1') : " << event_tc->member_visibility(0)
                          << " Visibility ('0') : " << event_tc->member_visibility(1)
                          << std::endl;
        retcode = 1;
      }
      else
      {
        TAOX11_TEST_DEBUG << "OK  create_event_tc" << std::endl;
      }
    }

    TAOX11_TEST_INFO << "end of create_xxx_tc test" << std::endl;

    _orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception in test : "
      << ex << std::endl;
    return 1;
  }

  return retcode;
}
