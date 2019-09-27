// FUZZ: disable check_for_ace_log_categories

#include "testC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
int result_ = 0;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:") );
  int c;

  while ( (c = get_opts () ) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ( (LM_ERROR,
                            "usage:  %s "
                            "-k <ior> "
                            "\n",
                            argv [0]),
                           false);
      }
  // Indicates successful parsing of the command line
  return true;
}

void
TestEqual (Test::enumType exp, Test::enumType e_rec)
{
  if (e_rec != exp)
    {
      ACE_DEBUG ( (LM_DEBUG, "(%P|%t) - ERROR: value expected <%i> , received  <%i>\n", exp, e_rec) );
      ++result_;
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == false)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object (ior);

      Test::Hello_var hello = Test::Hello::_narrow (tmp.in () );

      if (CORBA::is_nil (hello.in () ) )
        {
          ACE_ERROR_RETURN ( (LM_DEBUG, "Nil Test::Hello reference <%s>\n", ior), 1);
        }

      ACE_DEBUG ( (LM_DEBUG, "Test enumType: get_enum.\n") );
      Test::enumType e1 = hello->get_enum ();
      TestEqual (e1, Test::b);

      Test::enumType e2 = Test::b;
      ACE_DEBUG ( (LM_DEBUG, "Test enumType: inout_enum.\n") );
      Test::enumType e3 = hello->inout_enum (Test::c, e1, e2);
      TestEqual (e1, Test::c);
      TestEqual (e2, Test::c);
      TestEqual (e3, Test::b);

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      ++result_;
    }

  return result_;
}
