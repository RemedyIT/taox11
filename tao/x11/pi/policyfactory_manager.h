/**
 * @file    policyfactory_manager.h
 * @author  Marcel Smit
 *
 * @brief   Policy Factory manager class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_POLICYFACTORY_MANAGER_INCLUDED
#define TAOX11_POLICYFACTORY_MANAGER_INCLUDED

#pragma once

#include /**/ "tao/x11/base/pre.h"

#include "ace/Service_Config.h"
#include "tao/x11/pi/taox11_pi_export.h"
#include "tao/x11/policyfactory_manager_adapter.h"

#if !defined(CORBA_E_MICRO)

namespace TAOX11_NAMESPACE
{
  class TAOX11_PortableInterceptor_Export PolicyFactoryManager_impl final
    : public PolicyFactoryManagerAdapter
  {
  public:
    ~PolicyFactoryManager_impl() = default;

    IDL::traits<CORBA::Policy>::ref_type
    create_policy (
          ORB_Proxy& orb,
          CORBA::PolicyType policy_type,
          const CORBA::Any & value) override;

    static int Initializer ();
  };

  static int
  X11_PolicyFactoryManager_impl_Initializer =
      PolicyFactoryManager_impl::Initializer ();


  ACE_STATIC_SVC_DECLARE (PolicyFactoryManager_impl)
  ACE_FACTORY_DECLARE (TAOX11_PortableInterceptor, PolicyFactoryManager_impl)

} // namespace TAOX11_NAMESPACE

#endif

#include /**/ "tao/x11/base/post.h"

#endif /* TAOX11_POLICYFACTORY_MANAGER_INCLUDED */

