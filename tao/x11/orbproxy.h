/**
 * @file    orbproxy.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 core Object classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ORBPROXY_H_INCLUDED
#define TAOX11_ORBPROXY_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/base/tao_corba.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/Objref_VarOut_T.h"
#include "tao/VarOut_T.h"

#include "tao/x11/base/stddef.h"

namespace TAOX11_NAMESPACE
{
  class ORB_Proxy final
  {
  public:
    ORB_Proxy (TAO_CORBA::ORB_ptr _orb)
      : orb_ (_orb) {}
    ~ORB_Proxy ()
    {
      TAO_CORBA::release (this->orb_);
    }

    TAO_CORBA::ORB_ptr operator ->()
    {
      return this->orb_;
    }

  private:
    TAO_CORBA::ORB_ptr orb_;

    ORB_Proxy () = delete;
    ORB_Proxy(const ORB_Proxy&) = delete;
    ORB_Proxy(ORB_Proxy&&) = delete;
    ORB_Proxy& operator=(const ORB_Proxy&) = delete;
    ORB_Proxy& operator=(ORB_Proxy&&) = delete;
  }; // ORB_Proxy
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_ORBPROXY_H_INCLUDED
