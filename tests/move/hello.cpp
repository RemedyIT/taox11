/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"
#include "testlib/taox11_testlog.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb) :
  orb_(orb)
{
}

// Test
Test::FileStore Hello::create(const std::wstring& owner)
{
  Test::FileStore store;
  store.owner(owner);
  store.tb(Test::DateTime(Test::Time_t(), true, true));
  return store;
}

bool Hello::validate(const std::wstring& owner, const Test::FileStore& store)
{
  bool sameowner = store.owner() == owner;
  return sameowner;
}

uint32_t Hello::update_content(Test::Block& bl, double change_request)
{
  // update_content of measurement-block
  TAOX11_TEST_DEBUG << "," << bl.id() << std::flush;
  bl.state(Test::BlockState::InUse);
  bl.block_modified(Test::DateTime(Test::Time_t(), true, true));
  Test::Measurement& m = bl.meas();
  for (int ix = 0; ix < 2500; ++ix)
  {
    m.push_back(change_request * ix);
  }

  return ACE_Utils::truncate_cast<uint32_t> (m.size ());
}

uint32_t Hello::convert_content(Test::Measurement& ct)
{
  double val = 1.0;
  for (Test::Measurement::size_type ix = 1; ix < ct.size(); ix *= 2, val += 1.0)
  {
    ct[ix] = val;
  }
  return (int) ((val - 1.0) * 10);
}

// End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
