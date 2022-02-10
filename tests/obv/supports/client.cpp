/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "supports_test_impl.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int num_trials = 1;
int id = 0;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:i:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        num_trials = std::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        id = std::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: " << argv[0]
                  << "-t <trials> "
                  << "-k <iorfile> "
                  << "-i <id> "
                  << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = orb->resolve_initial_references ("RootPOA");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

      if (!poaman)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
        return 1;
      }

      poaman->activate ();

      /* Create, register factories */

      node_init_impl::_ref_type node_factory = CORBA::make_reference<node_init_impl> ();

      CORBA::ValueFactoryBase::_ref_type returned_factory =
          orb->register_value_factory (node_factory->_obv_repository_id (), node_factory);

      // Some extra tests regarding is_local and is_abstract
      // Check ValueFactoryBase on is_abstract and is_local
      if (!IDL::traits<CORBA::ValueFactoryBase>::is_local ())
        {
          TAOX11_TEST_ERROR << "ERROR: ValueFactoryBase should not be local" << std::endl;
          ++result;
        }
      if (IDL::traits<CORBA::ValueFactoryBase>::is_abstract ())
        {
          TAOX11_TEST_ERROR << "ERROR: ValueFactoryBase should be abstract" << std::endl;
          ++result;
        }

      // Check Supports_Test::Node_init on is_abstract and is_local
      if (!IDL::traits<Supports_Test::Node_init>::is_local ())
        {
          TAOX11_TEST_ERROR << "ERROR: Supports_Test::Node_init should not be local."
            << std::endl;
          ++result;
        }
      if (IDL::traits<Supports_Test::Node_init>::is_abstract ())
        {
          TAOX11_TEST_ERROR << "ERROR: Supports_Test::Node_init should be abstract"
            << std::endl;
          ++result;
        }


      ACE_ASSERT (returned_factory == nullptr);

      vt_graph_init_impl::_ref_type vt_graph_factory = CORBA::make_reference<vt_graph_init_impl> ();

      returned_factory =
          orb->register_value_factory (vt_graph_factory->_obv_repository_id (), vt_graph_factory);

      ACE_ASSERT (returned_factory == nullptr);

      /* Check return values for register_value_factory */

      node_init_impl::_ref_type node_factory2 = CORBA::make_reference<node_init_impl> ();



      returned_factory =
        orb->register_value_factory (node_factory2->_obv_repository_id (),
                                     node_factory2);

      ACE_ASSERT (returned_factory->_is_equivalent (node_factory));

      /* Get test object reference */

      IDL::traits<CORBA::Object>::ref_type tmp = orb->string_to_object (ior);

      Supports_Test::test::_ref_type my_test =
        IDL::traits<Supports_Test::test>::narrow (tmp);

      if (!my_test)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Supports_Test::test>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      my_test->start ();

      /* Perform test */

      for (int i = 0; i < num_trials; i++)
        {

          // Create a vt_graph_impl instance and store the reference as a
          // vt_graph valuetype reference. Then register the instance with the POA to obtain an
          // object reference, stored as a graph object reference.
          CORBA::servant_reference<vt_graph_impl> vt_graph_srv =
              CORBA::make_reference<vt_graph_impl> (3);

          IDL::traits<Supports_Test::vt_graph>::ref_type test_vt_graph =
              vt_graph_srv;

          IDL::traits<Supports_Test::graph>::ref_type test_graph =
              vt_graph_srv->_this ();

          if (!test_graph)
          {
            TAOX11_TEST_ERROR << "Nil Supports_Test::graph obj ref" << std::endl;
            ++result;
            break;
          }

          // At this point, test_vt_graph and test_graph refer to the same object.
          ACE_ASSERT (test_vt_graph->size () == 3);
          my_test->pass_vt_graph_in (test_vt_graph);
          ACE_ASSERT (test_vt_graph->size () == 3);

          ACE_ASSERT (test_graph->size () == 3);
          my_test->pass_obj_graph_in (test_graph);
          ACE_ASSERT (test_graph->size () == 4);
          // TEMP FOR NOW

          test_vt_graph->add_node ("NEW2");

          test_graph->add_node ("NEW3");
          TAOX11_TEST_DEBUG << "ostream test interface graph: " << test_graph << std::endl;
          TAOX11_TEST_DEBUG << "ostream test valuetype vt_graph: " << test_vt_graph << std::endl;

          // After the 'pass_vt_graph_out' call returns, test_vt_graph will refer to
          // a _new_ object.
          ACE_ASSERT (test_vt_graph->size () == 6);
          my_test->pass_vt_graph_out (test_vt_graph);
          ACE_ASSERT (test_vt_graph->size () == 5);

          // 'test_graph' still refers to the original object, but after the
          // 'pass_obj_graph_out' call returns, it will refer to a _new_ object,
          // residing on the server.
          ACE_ASSERT (test_graph->size () == 6);
          my_test->pass_obj_graph_out (test_graph);
          ACE_ASSERT (test_graph->size () == 5);

          // test_vt_graph and test_graph now refer to different objects.
          test_vt_graph->add_node ("NEW2");

          test_graph->add_node ("NEW2");

          ACE_ASSERT (test_vt_graph->size () == 6);
          my_test->pass_vt_graph_inout (test_vt_graph);
          ACE_ASSERT (test_vt_graph->size () == 7);

          ACE_ASSERT (test_graph->size () == 6);
          my_test->pass_obj_graph_inout (test_graph);
          ACE_ASSERT (test_graph->size () == 7);
        }

      my_test->finish ();

      /* Shut down */

      orb->destroy ();

      if (result == 0)
        {
          TAOX11_TEST_DEBUG << "Client completed test successfully" << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR : <" << result << "> errors were found in "
            << "this test" << std::endl;
        }
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }

  return result;
}
