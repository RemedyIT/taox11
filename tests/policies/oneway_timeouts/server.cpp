/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testS.h"

#include "ace/High_Res_Timer.h"
#include "ace/Arg_Shifter.h"
#include "ace/Task.h"
#include <thread>

#include "tao/x11/portable_server/portableserver_functions.h"
#include "testlib/taox11_testlog.h"

#include <fstream>

uint32_t activate_delay = 0;
uint32_t run_delay = 0;
uint32_t request_delay = 0;
uint32_t abort_after = 0;
uint32_t num_expected = 0;
uint32_t elapsed_max = 0;
uint32_t elapsed_min = 0;
uint32_t first_min = 0;
uint32_t first_max = 0;

const ACE_TCHAR *server_ior = ACE_TEXT ("test.ior");

class ShutdownDelay final
  : public virtual ACE_Task_Base
{
public:
  ShutdownDelay () = default;
  ~ShutdownDelay () = default;

  virtual int svc () override;

  void set_orb (IDL::traits<CORBA::ORB>::ref_type orb);

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;

  ShutdownDelay (const ShutdownDelay&) = delete;
  ShutdownDelay (ShutdownDelay&&) = delete;
  ShutdownDelay& operator= (const ShutdownDelay&) = delete;
  ShutdownDelay& operator= (ShutdownDelay&&) = delete;
};

void
ShutdownDelay::set_orb (CORBA::object_traits< CORBA::ORB >::ref_type orb)
{
  this->orb_ = orb;
}

int
ShutdownDelay::svc ()
{
  std::this_thread::sleep_for (std::chrono::milliseconds (50));
  this->orb_->shutdown (false);
  return 0;
}

class Tester_i final
  : public virtual CORBA::servant_traits<Tester>::base_type
{
public:
  Tester_i (IDL::traits<CORBA::ORB>::ref_type orb);
  virtual ~Tester_i ();

  virtual void test (int32_t id) override;
  virtual int32_t test2 (int32_t id) override;

  void wait_for_sd ();

  bool failed () const;

private:
  Tester_i (const Tester_i&) = delete;
  Tester_i (Tester_i&&) = delete;
  Tester_i& operator= (const Tester_i&) = delete;
  Tester_i& operator= (Tester_i&&) = delete;

  int16_t testShared (int32_t id);

  IDL::traits<CORBA::ORB>::ref_type orb_;
  int32_t id1_;
  int32_t id2_;
  uint32_t count_;
  bool failed_;
  ACE_Time_Value start_;
  ACE_Time_Value first_;
  ACE_Time_Value last_;

  ShutdownDelay sd_;
};

Tester_i::Tester_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
  , id1_ (0)
  , id2_ (0)
  , count_ (0)
  , failed_ (false)
{
  this->start_ = ACE_High_Res_Timer::gettimeofday_hr ();
}

Tester_i::~Tester_i ()
{
  this->sd_.wait ();
}

void
Tester_i::test (int32_t id)
{
  // Ignore the extra message that were used for making messages
  // queued for SYNC_DELAYED_BUFFERING test cases.
  if (id == -3)
    return;

  testShared (id);
}

int32_t
Tester_i::test2 (int32_t id)
{
  if (id == -2)
    {
      // Special id used to force a connect. Ignore.
      this->start_ = ACE_High_Res_Timer::gettimeofday_hr ();
      return id;
    }
  return testShared (id);
}

int16_t
Tester_i::testShared (int32_t id)
{
  ACE_Time_Value now = ACE_High_Res_Timer::gettimeofday_hr ();
  if (id == -1)
  {
    // Signals the end of a test run
    if (num_expected > 0 && this->count_ != num_expected)
    {
      TAOX11_TEST_ERROR << "Error: Expected " << num_expected
        << ", but received " << this->count_ << std::endl;
      this->failed_ = true;
    }
    uint32_t ms = (this->last_ - this->first_).msec ();
    if (elapsed_max > 0 && ms > elapsed_max)
    {
      TAOX11_TEST_ERROR << "Error: Expected  < " << elapsed_max
        << "ms, but was " << ms << "ms" << std::endl;
      this->failed_ = true;
    }
    if (elapsed_min > 0 && ms < elapsed_min)
    {
      TAOX11_TEST_ERROR << "Error: Expected  > " << elapsed_min
        << "ms, but was " << ms << "ms" << std::endl;
      this->failed_ = true;
    }
    ms = (this->first_ - this->start_).msec ();
    if (first_max > 0 && ms > first_max)
    {
      TAOX11_TEST_ERROR << "Error: Expected first < " << first_max
        << "ms, but was " << ms << "ms" << std::endl;
      this->failed_ = true;
    }
    if (first_min > 0 && ms < first_min)
    {
      TAOX11_TEST_ERROR << "Error: Expected first > " << first_min
        << "ms, but was " << ms << "ms" << std::endl;
      this->failed_ = true;
    }
    TAOX11_TEST_DEBUG << x11_logger::now << " Shutting ORB down." << std::endl;
    this->sd_.set_orb (this->orb_);
    this->sd_.activate ();
    return id;
  }

  this->last_ = now;
  if (id == 0)
  {
    this->first_ = now;
  }
  ++this->count_;
  TAOX11_TEST_DEBUG << 's' << id << std::endl;
  if (abort_after > 0 && this->count_ >= abort_after)
  {
    TAOX11_TEST_DEBUG << "\nAborting..." << std::endl;
    ACE_OS::abort ();
  }
  if (request_delay > 0 && id == 0)
  {
    std::this_thread::sleep_for (std::chrono::milliseconds (request_delay));
  }
  return id;
}

bool
Tester_i::failed () const
{
  return this->failed_;
}

void
Tester_i::wait_for_sd ()
{
  this->sd_.wait ();
}

namespace {

  void print_usage ()
  {
    TAOX11_TEST_DEBUG << "server [-activate_delay ms] [-run_delay ms] [-request_delay ms] "
      "[-abort_after n] [-server_ior <ior>]\n"
      "\t[-expected n=0] [-elapsed_max ms=0] [-elapsed_min ms=0] "
      "[-first_min ms=0]\n"
      "\t[-first_max ms=0]\n"
      "\tactivate_delay Millisecond delay before POAManager::activate.\n"
      "\trun_delay Millisecond delay before ORB::run ().\n"
      "\trequest_delay Millisecond delay within each servant request.\n"
      "\tabort_after abort () after N requests.\n"
      "\tior Server ior.\n" << std::endl;
  }

  bool
  parse_command_line (int ac, ACE_TCHAR *av[])
  {
    ACE_Arg_Shifter args (ac, av);
    args.consume_arg ();
    while (args.is_anything_left ())
    {
      if (args.cur_arg_strncasecmp (ACE_TEXT ("-activate_delay")) == 0)
      {
        args.consume_arg ();
        activate_delay = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-run_delay")) == 0)
      {
        args.consume_arg ();
        run_delay = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-request_delay")) == 0)
      {
        args.consume_arg ();
        request_delay = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-expected")) == 0)
      {
        args.consume_arg ();
        num_expected = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-elapsed_max")) == 0)
      {
        args.consume_arg ();
        elapsed_max = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-elapsed_min")) == 0)
      {
        args.consume_arg ();
        elapsed_min = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-first_min")) == 0)
      {
        args.consume_arg ();
        first_min = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-first_max")) == 0)
      {
        args.consume_arg ();
        first_max = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-abort_after")) == 0)
      {
        args.consume_arg ();
        abort_after = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT ("-server_ior")) == 0)
      {
        args.consume_arg ();
        server_ior = args.get_current ();
        args.consume_arg ();
      }
      else
      {
        TAOX11_TEST_ERROR << "Error: Unknown argument \""
             << args.get_current () << "\"" << std::endl;
        print_usage ();
        return false;
      }
    }
    return true;
  }

  void
  WriteIOR (const std::string& ior)
  {
    ofstream out (ACE_TEXT_ALWAYS_CHAR (server_ior));
    out << ior;
  }

  IDL::traits<PortableServer::POA>::ref_type
  create_poa (IDL::traits<CORBA::ORB>::ref_type orb)
  {
    CORBA::PolicyList pols (2);
    IDL::traits<CORBA::Object>::ref_type obj =
      orb->resolve_initial_references ("RootPOA");
    IDL::traits<PortableServer::POA>::ref_type root =
      IDL::traits<PortableServer::POA>::narrow (obj);

    IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type id_pol =
      root->create_id_assignment_policy (
        PortableServer::IdAssignmentPolicyValue::USER_ID);
    pols[0] = id_pol;

    IDL::traits<PortableServer::LifespanPolicy>::ref_type ls_pol =
      root->create_lifespan_policy (
        PortableServer::LifespanPolicyValue::PERSISTENT);
    pols[1] = ls_pol;

    IDL::traits<PortableServer::POAManager>::ref_type man =
      root->the_POAManager ();
    IDL::traits<PortableServer::POA>::ref_type poa =
      root->create_POA ("X", man, pols);
    return poa;
  }
}

int main (int ac, ACE_TCHAR *av[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb =
      CORBA::ORB_init (ac, av);

    if (!parse_command_line (ac, av))
      return 1;

    IDL::traits<PortableServer::POA>::ref_type poa =
      create_poa (orb);

    CORBA::servant_reference<Tester_i> svt =
      CORBA::make_reference<Tester_i> (orb);

    PortableServer::ObjectId id = PortableServer::string_to_ObjectId ("tester");

    poa->activate_object_with_id (id, svt);

    IDL::traits<CORBA::Object>::ref_type obj = poa->id_to_reference (id);
    std::string ior = orb->object_to_string (obj);
    WriteIOR (ior);

    TAOX11_TEST_DEBUG << "Servants registered and activated." << std::endl;

    if (activate_delay > 0)
    {
      std::this_thread::sleep_for (std::chrono::milliseconds (activate_delay));
    }

    IDL::traits<PortableServer::POAManager>::ref_type man =
      poa->the_POAManager ();
    man->activate ();

    TAOX11_TEST_DEBUG << "POAManager activated." << std::endl;

    if (run_delay > 0)
    {
      std::this_thread::sleep_for (std::chrono::milliseconds (run_delay));
    }
    TAOX11_TEST_DEBUG << "Running orb..." << std::endl;

    orb->run ();

    TAOX11_TEST_DEBUG << x11_logger::now << " ORB loop ended." << std::endl;

    if (svt->failed ())
    {
      TAOX11_TEST_ERROR << "ERROR: Errors in servant detected." << std::endl;
      return 1;
    }

    poa->destroy (true, true);

    svt->wait_for_sd ();

    orb->destroy ();

    return 0;
  }
  catch (const CORBA::Exception &ex)
  {
    TAOX11_TEST_ERROR << "Error : Unexpected exception caught in server: " << ex
      << std::endl;
  }

  return 1;
}
