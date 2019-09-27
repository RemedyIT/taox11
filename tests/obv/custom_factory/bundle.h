/**
 * @file    bundle.h
 * @author  Martin Corino
 *
 * @brief   valuetype implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef __OBV_CUSTOM_FACTORY_BUNDLE_H_INCLUDED__
#define __OBV_CUSTOM_FACTORY_BUNDLE_H_INCLUDED__

class BundleImpl : public virtual IDL::traits<Bundle>::obv_type
{
  public:
    void Display () override
    {
      TAOX11_TEST_INFO << std::hex << iInteger () << std::endl;
    };
};

class BundleFactory : public virtual IDL::traits<Bundle>::factory_type
{
  private:
    virtual IDL::traits<CORBA::ValueBase>::ref_type create_for_unmarshal () override
    {
      return (CORBA::make_reference<BundleImpl> ());
    };
};

#endif
