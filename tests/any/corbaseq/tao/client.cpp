/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client CORBA sequences Any test
 * Test of ANY insertion/extraction for all standard CORBA sequences
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

// FUZZ: disable check_for_ace_log_categories

#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/WCharSeqC.h"
#include "tao/AnyTypeCode/WCharSeqA.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");


bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                           false);
      }
  // Indicates successful parsing of the command line
  return true;
}

bool run_test (Test::Hello_ptr tester, const CORBA::WCharSeq& inseq)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - runtest start\n"));

  const CORBA::WCharSeq* check_ {};
  CORBA::Any input;
  input <<= inseq;

  input >>= check_;
  if (check_ == 0 || check_->length () != inseq.length ())
  {
    ACE_ERROR ((LM_ERROR, "(%P|%t) - ERROR: failed to verify input sequence\n"));
    return false;
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - calling Test::Hello::test_wcharseq()\n"));

  CORBA::Any_var output = tester->test_wcharseq (input);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - calling Test::Hello::test_wcharseq() returned\n"));

  const CORBA::WCharSeq* check2_ {};
  output.in () >>= check2_;
  if (check2_ == 0 || check2_->length () != inseq.length ())
  {
    ACE_ERROR ((LM_ERROR, "(%P|%t) - ERROR: failed to verify output sequence\n"));
    return false;
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - runtest end\n"));
  return true;
}

int
main(int argc, char* argv[])
{
  bool testFailed = false;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == false)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      Test::Hello_var hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // testing

      CORBA::WCharSeq wchars(5);
      wchars.length (0);

      // empty sequence
      testFailed &= run_test (hello.in (), wchars);

      // filled sequence
      wchars.length (5);
      wchars[0] = L'a';
      wchars[1] = L'b';
      wchars[2] = L'c';
      wchars[3] = L'd';
      wchars[4] = L'e';
      testFailed &= run_test (hello.in (), wchars);

      // end testing

      if (testFailed)
      {
        ACE_ERROR ((LM_ERROR, "(%P|%t) - ERROR : Any CORBA Sequences test failed\n"));
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Any CORBA Sequences Test OK\n"));
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - shutting down\n"));

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      testFailed = true;
    }

  if (testFailed)
   {
     return 1;
   }

  return 0;
}

