/**
 * @file    main.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IDL test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "pragmaS.h"
// #include "unionC.h"
#include "repo_id_modC.h"
#include "constantsC.h"
#include "nested_scopeS.h"

#include "tao/x11/anytypecode/any.h"
#include "testlib/taox11_testlog.h"


class hello_i : public virtual CORBA::servant_traits<hello>::base_type
{
};

class goodbye_i : public virtual CORBA::servant_traits<goodbye>::base_type
{
};

class sayonara_i : public virtual CORBA::servant_traits<salutation::sayonara>::base_type
{
};

class ciao_i : public virtual CORBA::servant_traits<ciao>::base_type
{
};

class aloha_i : public virtual CORBA::servant_traits<aloha>::base_type
{
};

class schmooze_i : public virtual gleep::schmooze
{
};

class schmeer_i : public virtual gleep::schmeer
{
};

class schlemiel_i : public virtual gleep::schlemiel
{
};

class spilkis_i : public virtual gleep::spilkis
{
};

class schmuck_i : public virtual gleep::floop::schmuck
{
};

class schmendrick_i : public virtual gleep::floop::schmendrick
{
};

class schlemazel_i : public virtual gleep::floop::verklempt::schlemazel
{
};

class schmegegging_i : public virtual gleep::schmegegging
{
};

// TODO: when anynomous times are enabled
// struct something_handler
//   : public CORBA::servant_traits<bug_1985_c::d::AMI_somethingHandler>
// {
// };

int
main (int argc, char *argv[])
{
  int error_count = 0;

//   const ACE_CDR::ULongLong test_ull =
//     ACE_UINT64_LITERAL (122192928000000000);
//
//   if (test_ull != AAA)
//     {
//       ++error_count;
//       TAOX11_TEST_ERROR <<
//                   ACE_TEXT ("Generated value of unsigned")
//                   ACE_TEXT (" long long AAA in constants.idl")
//                   ACE_TEXT (" is incorrect\n")));
//     }

  const int64_t test_nll = -122192928000000000;

  if (test_nll != NAAA)
    {
      ++error_count;
      TAOX11_TEST_ERROR << "Generated value of signed"
            << " long long NAAA in constants.idl"
            << " is incorrect" << std::endl;
    }

  const int64_t test_pll = 122192928000000000;

  if (test_pll != PAAA)
    {
      ++error_count;
      TAOX11_TEST_ERROR << "Generated value of signed"
            << " long long PAAA in constants.idl"
            << " is incorrect" << std::endl;
    }

  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      IDL::traits<CORBA::Object>::ref_type poa_object =
        orb->resolve_initial_references ("RootPOA");

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

      // Test of #pragma prefix behavior.

      IDL::traits<CORBA::Object>::ref_type obj;

      CORBA::servant_traits<hello>::ref_type h =
        CORBA::make_reference<hello_i> ();

      PortableServer::ObjectId id = root_poa->activate_object (h);
      obj = root_poa->id_to_reference (id);
      obj = IDL::traits<hello>::narrow (obj);

      CORBA::servant_reference<goodbye_i> g =
        CORBA::make_reference<goodbye_i> ();
      id = root_poa->activate_object (g);
      obj = root_poa->id_to_reference (id);
      obj = IDL::traits<goodbye>::narrow (obj);

      CORBA::servant_reference<sayonara_i> s =
        CORBA::make_reference<sayonara_i> ();
      id = root_poa->activate_object (s);
      obj = root_poa->id_to_reference (id);
      obj = IDL::traits<salutation::sayonara>::narrow (obj);

      CORBA::servant_reference<ciao_i> c =
        CORBA::make_reference<ciao_i> ();
      id = root_poa->activate_object (c);
      obj = root_poa->id_to_reference (id);
      obj = IDL::traits<ciao>::narrow (obj);

      CORBA::servant_reference<aloha_i> a =
        CORBA::make_reference<aloha_i> ();
      id = root_poa->activate_object (a);
      obj = root_poa->id_to_reference (id);
      obj = IDL::traits<aloha>::narrow (obj);

      // Test of typeprefix, typeid, and #pragma version behavior.
      schmooze_i s_schmooze;
      schmeer_i s_schmeer;
      schlemiel_i s_schlemiel;
      spilkis_i s_spilkis;
      schmuck_i s_schmuck;
      schmendrick_i s_schmendrick;
      schlemazel_i s_schlemazel;
      schmegegging_i s_schmegegging;

//       {
//         something_handler x;
//         char const * base[] =
//         {
//           "IDL:bug_1985_c/d/AMI_somethingHandler:1.0",
//           "IDL:bug_1985_a/b/AMI_somethingHandler:1.0",
//           "IDL:omg.org/Messaging/ReplyHandler:1.0",
//           "IDL:omg.org/CORBA/Object:1.0"
//         };
//
//         for (int i = 0; i != sizeof (base)/sizeof (base[0]); ++i)
//           {
//             if (!x._is_a (base[i]))
//               {
//                 ++error_count;
//                 TAOX11_TEST_ERROR <<
//                            "something_handler::_is_a should return true for "
//                            << base[i] << std::endl;
//               }
//           }
//
//         if (!dynamic_cast<POA_bug_1985_c::d::AMI_somethingHandler*> (&x))
//           {
//             ++error_count;
//             TAOX11_TEST_ERROR <<
//                        "mismatch in downcast for "
//                        << base[0] << std::endl;
//           }
//
//         if (!dynamic_cast<POA_bug_1985_a::b::AMI_somethingHandler*> (&x))
//           {
//             ++error_count;
//             TAOX11_TEST_ERROR <<
//                        "mismatch in downcast for %s\n",
//                        base[1]));
//           }
//
//         if (!dynamic_cast<POA_Messaging::ReplyHandler*> (&x))
//           {
//             ++error_count;
//             TAOX11_TEST_ERROR <<
//                        "mismatch in downcast for %s\n",
//                        base[2]));
//           }
//       }

      // Testing (de)marshaling of IDL union values
      // under duplicate and default case labels.

//       Field field;
//       field.value.strValue (
//           CORBA::string_dup ("duplicate case label test string")
//         );
//       field.value._d (FTYPE_VARCHAR);
//       CORBA::Any any1;
//       any1 <<= field;
//       Field *outfield;
//
//       if ((any1 >>= outfield) == 0)
//         {
//           ++error_count;
//           TAOX11_TEST_ERROR <<
//                       "error in extraction of "
//                       "duplicate case label value" << std::endl;
//         }
//
//       const char *str = outfield->value.strValue ();
//
//       if (ACE_OS::strcmp (str, "duplicate case label test string") != 0)
//         {
//           ++error_count;
//           TAOX11_TEST_ERROR <<
//                       "error - corruption of "
//                       "duplicate case label value" << std::endl;
//         }
//
//       field.value.defstr (CORBA::string_dup ("default case test string"));
//       any1 <<= field;
//
//       if ((any1 >>= outfield) == 0)
//         {
//           ++error_count;
//           TAOX11_TEST_ERROR <<
//                       "error in extraction of "
//                       "default case label value" << std::endl;
//         }
//
//       str = outfield->value.defstr ();
//
//       if (ACE_OS::strcmp (str, "default case test string") != 0)
//         {
//           ++error_count;
//           TAOX11_TEST_ERROR <<
//                       "error - corruption of "
//                       "default case label value" << std::endl;
//         }
//
//       if (SignedGen::val !=  -3)
//         {
//           ++error_count;
//           TAOX11_TEST_ERROR <<
//                       "error - signed integer literal "
//                       "generated as unsigned" << std::endl;
//         }

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception <main> - "
            << ex << std::endl;
      ++error_count;
    }

  return error_count;
}
