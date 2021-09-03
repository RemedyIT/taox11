/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 ORB services test module
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "orbsvcs/orbsvcs/naming_server/CosNamingC.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

std::string name_ ("");
std::string ns_ior_ ("");
IDL::traits<CORBA::ORB>::ref_type naming_orb_;


IDL::traits<CosNaming::NamingContext>::ref_type
get_naming_context (int argc=0, char *argv[]=0)
{

  if (naming_orb_ == nullptr)
    naming_orb_ = CORBA::ORB_init (argc, argv, "NAMING");

  // Find the Naming Service
  IDL::traits<CORBA::Object>::ref_type obj = naming_orb_->string_to_object (ns_ior_);

  if (obj == nullptr)
    {
      TAOX11_TEST_ERROR << "string_to_object(<ior>) returned null reference."
            << std::endl;
      return nullptr;
    }

  IDL::traits<CosNaming::NamingContext>::ref_type nc = IDL::traits<CosNaming::NamingContext>::narrow (obj);

  if (!nc)
    {
      TAOX11_TEST_ERROR << "Narrow returned null object."
            << std::endl;
      return nullptr;
    }
  return nc;
}

void
register_with_naming (int argc, char *argv[],
    IDL::traits<CORBA::Object>::ref_type ptr)
{
  const std::string loginfo ("register_with_naming <" + name_ + ">: ");

  try
    {
      IDL::traits<CosNaming::NamingContext>::ref_type nc = get_naming_context (argc, argv);

      TAOX11_TEST_INFO << loginfo << "Start binding"
            << std::endl;

      CosNaming::NameComponent comp;
      comp.id(name_);
      comp.kind (name_);

      CosNaming::Name Name;
      Name.push_back(comp);

      nc->bind (Name, ptr);
    }
  catch (const ::CosNaming::NamingContext::NotFound &)
    {
      TAOX11_TEST_ERROR << loginfo << "NotFound exception caught"
            << std::endl;
    }
  catch (const ::CosNaming::NamingContext::CannotProceed &)
    {
      TAOX11_TEST_ERROR << loginfo << "CannotProceed exception caught"
            << std::endl;
    }
  catch (const ::CosNaming::NamingContext::InvalidName &)
    {
      TAOX11_TEST_ERROR << loginfo << "InvalidName exception caught"
            << std::endl;
    }
  catch (const ::CosNaming::NamingContext::AlreadyBound &)
    {
      TAOX11_TEST_ERROR << loginfo << "AlreadyBound exception caught"
            << std::endl;
    }
}

void
unregister_with_naming ()
{
  const std::string loginfo ("unregister_with_naming <" + name_ + ">: ");

  try
    {
      IDL::traits<CosNaming::NamingContext>::ref_type nc = get_naming_context ();

      TAOX11_TEST_INFO << loginfo << "Start unbinding"
            << std::endl;

      CosNaming::NameComponent comp;
      comp.id(name_);
      comp.kind (name_);

      CosNaming::Name Name;
      Name.push_back(comp);

      nc->unbind (Name);
    }
  catch (const ::CosNaming::NamingContext::NotFound &)
    {
      TAOX11_TEST_ERROR << loginfo << "NotFound exception caught"
            << std::endl;
    }
  catch (const ::CosNaming::NamingContext::CannotProceed &)
    {
      TAOX11_TEST_ERROR << loginfo << "CannotProceed exception caught"
            << std::endl;
    }
  catch (const ::CosNaming::NamingContext::InvalidName &)
    {
      TAOX11_TEST_ERROR << loginfo << "InvalidName exception caught"
            << std::endl;
    }
}

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ns_ior_ = get_opts.opt_arg ();
        break;
      case 'n':
        name_ = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: " << std::endl;
        TAOX11_TEST_ERROR << "\t-k <ns ior>" << std::endl;
        TAOX11_TEST_ERROR << "\t-n <name>" << std::endl;
        return -1;
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
main(int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (orb == nullptr)
        {
          TAOX11_TEST_ERROR << "CORBA::ORB_init (argc, argv) returned null ORB."
                << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) != 0)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = orb->resolve_initial_references ("RootPOA");

      if (obj == nullptr)
        {
          TAOX11_TEST_ERROR << "resolve_initial_references (\"RootPOA\") returned null reference."
                << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR << "PortableServer::POA::_narrow (obj) returned null object."
                << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

      if (!poaman)
        {
          TAOX11_TEST_ERROR << "root_poa->the_POAManager () returned null object."
                << std::endl;
          return 1;
        }

      CORBA::servant_traits<Test::Hello>::ref_type hello_impl = CORBA::make_reference<Hello> (orb);

      PortableServer::ObjectId id = root_poa->activate_object (hello_impl);

      IDL::traits<CORBA::Object>::ref_type hello_obj = root_poa->id_to_reference (id);

      if (hello_obj == nullptr)
        {
          TAOX11_TEST_ERROR << "root_poa->id_to_reference (id) returned null reference."
                << std::endl;
          return 1;
        }

      register_with_naming (argc, argv, hello_obj);

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (hello_obj);

      if (hello == nullptr)
        {
          TAOX11_TEST_ERROR << "Test::Hello::_narrow (hello_obj) returned null reference."
                << std::endl;
          return 1;
        }

      std::string ior = orb->object_to_string (hello);

      poaman->activate ();

      orb->run ();

      unregister_with_naming ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "Server: exception caught: " << e << std::endl;
      return 1;
    }
  TAOX11_TEST_INFO << "Server <" << name_ << "> has ended"
        << std::endl;
  return 0;
}
