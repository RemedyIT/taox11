/**
 * @file    servant_forward.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core servant class forward decl
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_SERVANT_FORWARD_H_INCLUDED
#define TAOX11_SERVANT_FORWARD_H_INCLUDED

#include "tao/x11/portable_server/servant_traits_t.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class Servant; // forward
  } // namespace PortableServer

  namespace CORBA
  {
    template <>
    struct servant_traits<PortableServer::Servant>
    {
      typedef PortableServer::Servant                             base_type;
      typedef CORBA::servant_reference<PortableServer::Servant>   ref_type;
    };
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_SERVANT_FORWARD_H_INCLUDED
