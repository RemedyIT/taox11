/**
 * @file    driver.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/pi/pi.h"
#include "tao/x11/pi/orb_initializer_registry.h"

#include "testlib/taox11_testlog.h"

PortableInterceptor::SlotId slot_id;

class ORB_Initializer final
  : public IDL::traits<PortableInterceptor::ORBInitializer>::base_type
{
public:
  void
  pre_init (IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type) override
  {
  }

  void
  post_init (IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info) override
  {
    slot_id = info->allocate_slot_id ();
    TAOX11_TEST_INFO << "Allocated slot with id " << slot_id << std::endl;
  }
};

int
main(int argc, ACE_TCHAR *argv[])
{

  try
  {
    IDL::traits<PortableInterceptor::ORBInitializer>::ref_type orb_initializer =
        CORBA::make_reference<ORB_Initializer> ();
    PortableInterceptor::register_orb_initializer (orb_initializer);

    IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

    IDL::traits<CORBA::Object>::ref_type pic_obj =
        orb->resolve_initial_references ("PICurrent");
    IDL::traits<PortableInterceptor::Current>::ref_type pic (
        IDL::traits<PortableInterceptor::Current>::narrow (pic_obj));

    CORBA::Any in;
    in <<= uint32_t (1);
    pic->set_slot (slot_id, in);

    uint32_t v (0);
    CORBA::Any out = pic->get_slot (slot_id);
    out >>= v;

    if (v != 1)
    {
      TAOX11_TEST_ERROR << "ERROR: Slot value was not preserved." << std::endl;
      return 1;
    }

    orb->destroy ();
  }
  catch (PortableInterceptor::InvalidSlot const&)
  {
    TAOX11_TEST_ERROR << "ERROR: Caught InvalidSlot exception." << std::endl;
    return 1;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught: " << ex.what () << std::endl;
    return 1;
  }

  return 0;
}
