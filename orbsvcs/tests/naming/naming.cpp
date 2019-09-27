/**
 * @file    naming.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 ORB services test module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "ace/SString.h"
#include "orbsvcs/orbsvcs/naming_server/CosNamingC.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_ctype.h"
#include "testC.h"

#include "testlib/taox11_testlog.h"

void
test_object (IDL::traits<CORBA::Object>::ref_type obj)
{
  IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

  if (!hello)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
            << std::endl;
      return;
    }

  TAOX11_TEST_DEBUG << "hello->get_string () returned " << hello->get_string ()
        << std::endl;

  TAOX11_TEST_DEBUG << "shutting down...";

  hello->shutdown ();

  TAOX11_TEST_DEBUG << std::endl;
}

//==========================================================================
// Display NS entries from a finite list.
void
show_chunk (IDL::traits<CORBA::ORB>::ref_type orb,
            IDL::traits<CosNaming::NamingContext>::ref_type nc,
            const CosNaming::BindingList &bl)
{
  for (const CosNaming::Binding& bn : bl)
    {
      TAOX11_TEST_DEBUG << "+ New server:"
            << std::endl;

      TAOX11_TEST_DEBUG << "\t- Name "
            << bn.binding_name ()[0].id ().c_str ()
            << std::endl;

      if (bn.binding_name ()[0].kind ()[0])
        TAOX11_TEST_DEBUG << "\t- Kind: "
              << bn.binding_name ()[0].kind ().c_str ()
              << std::endl;

      CosNaming::NameComponent comp;
      comp.id(bn.binding_name ()[0].id ());
      comp.kind (bn.binding_name ()[0].kind ());

      CosNaming::Name Name;
      Name.push_back(comp);

      IDL::traits<CORBA::Object>::ref_type obj = nc->resolve (Name);

      // If this is a context node, follow it down to the next level...
      if (bn.binding_type () == CosNaming::BindingType::ncontext)
        {
          TAOX11_TEST_DEBUG << ": Naming context";
        }
      else
        {
          TAOX11_TEST_DEBUG << "\t- Object Reference:";
          if (obj == nullptr)
            TAOX11_TEST_DEBUG << " {Null}";
          else
            {
              std::string str = orb->object_to_string (obj);
              TAOX11_TEST_DEBUG << " " << str.c_str ()
                    << std::endl;
              test_object (obj);
            }
        }
    }
}

//==========================================================================
void
list_context (IDL::traits<CORBA::ORB>::ref_type orb,
              IDL::traits<CosNaming::NamingContext>::ref_type nc)
{
  IDL::traits<CosNaming::BindingIterator>::ref_type it;
  CosNaming::BindingList bl;
  unsigned long const CHUNK = 100;

  nc->list (CHUNK, bl, it);

  if (bl.size () == 0)
    {
      TAOX11_TEST_ERROR << "list_context : "
            << "Error: Returned list seems to have "
            << "no content."
            << std::endl;
    }
  else
    {
      TAOX11_TEST_DEBUG <<"list_context : "
            << "Returned list seems to have "
            << bl.size ()
            << " items."
            << std::endl;
    }

  show_chunk (orb, nc, bl);
}

int
main (int argc, ACE_TCHAR *argv[])
{
  try {

    // Initialize orb
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init( argc, argv );

    // Find the Naming Service
    IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object ("file://ns.ior");

    if (obj == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
              << std::endl;
        return 1;
      }

    IDL::traits<CosNaming::NamingContext>::ref_type nc = IDL::traits<CosNaming::NamingContext>::narrow (obj);

    if (!nc)
      {
        TAOX11_TEST_ERROR << "ERROR: Narrow returned null object."
              << std::endl;
        return 1;
      }

    list_context(orb, nc);

    orb->destroy ();
  }
  catch(const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught a CORBA::Exception: " << ex
            << std::endl;
      return 1;
    }

  return 0;
}
