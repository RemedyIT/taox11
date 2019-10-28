/**
 * @file    foo_iorinterceptor.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" IOR test interceptor.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo_iorinterceptor.h"
#include "testS.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/orb_constants.h"

FOO_IORInterceptor::FOO_IORInterceptor (IDL::traits<IOP::Codec>::ref_type codec)
  : codec_ (std::move (codec))
{
}

std::string
FOO_IORInterceptor::name ()
{
  return "FOO_IORInterceptor";
}

void
FOO_IORInterceptor::destroy ()
{
}

void
FOO_IORInterceptor::establish_components (
    IDL::traits<PortableInterceptor::IORInfo>::ref_type info)
{
  // According to the Portable Interceptors specification,
  // IORInterceptor::establish_components() must not throw an
  // exception.  We check for exceptions despite this fact.  The ORB
  // does the right thing, and ignores any IOR interceptors that throw
  // an exception.

  std::string name = this->name ();

  CORBA::Any data;
  data <<= name;

  CORBA::OctetSeq encoded_data =
    std::move (this->codec_->encode_value (data));

  // Construct a tagged component.
  IOP::TaggedComponent component;

  component.tag (FOO::COMPONENT_ID);

  component.component_data (std::move (encoded_data));

  // Add the tagged component to all profiles.
  info->add_ior_component (component);

  TAOX11_TEST_INFO << "Added tagged component containing the string \"" <<
      name << "\" to all profiles." << std::endl;

  // Add the tagged component to all IIOP profiles.
  info->add_ior_component_to_profile (component,
                                      IOP::TAG_INTERNET_IOP);


  TAOX11_TEST_INFO << "Added tagged component containing the string \"" <<
      name << "\" to all IIOP profiles." << std::endl;


  try
  {
    // Verify that policy retrieval internals work, and do not cause
    // memory access violations.
    IDL::traits<CORBA::Policy>::ref_type policy =
      info->get_effective_policy (PortableServer::LIFESPAN_POLICY_ID);

    TAOX11_TEST_INFO << "Successfully retrieved effective policy." << std::endl;
  }
  catch (const CORBA::INV_POLICY& ex)
  {
    if (ex.minor () != (CORBA::OMGVMCID | 3))
      throw;
  }
}

void
FOO_IORInterceptor::components_established (
    IDL::traits<PortableInterceptor::IORInfo>::ref_type info)
{
  // Construct a dummy tagged component.
  IOP::TaggedComponent component;

  component.tag (0);
  component.component_data ().push_back (0);


  try
  {
    // Add the tagged component to all profiles.
    //
    // This call should fail since it shouldn't be possible to add
    // an IOR component after establish_components() has been
    // called.
    info->add_ior_component (component);

    // The above call should NOT have succeeded.
    throw CORBA::INTERNAL ();
  }
  catch (const CORBA::BAD_INV_ORDER& ex)
  {
    if (ex.minor () != (CORBA::OMGVMCID | 14))
    {
      TAOX11_TEST_ERROR << "Unexpected exception : " << ex << std::endl;

      throw;
    }
  }
  try
  {
    // Add the tagged component to all profiles.
    //
    // This call should fail since it shouldn't be possible to add
    // an IOR component after establish_components() has been
    // called.
    info->add_ior_component_to_profile (component,
                                        IOP::TAG_INTERNET_IOP);

    // The above call should NOT have succeeded.
    throw CORBA::INTERNAL ();
  }
  catch (const CORBA::BAD_INV_ORDER& ex)
  {
    if (ex.minor () != (CORBA::OMGVMCID | 14))
    {
      TAOX11_TEST_ERROR << "Unexpected exception : " << ex << std::endl;

      throw;
    }
  }
}

void
FOO_IORInterceptor::adapter_manager_state_changed (
    const PortableInterceptor::AdapterManagerId&,
    PortableInterceptor::AdapterState)
{
}

void
FOO_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
{
}
