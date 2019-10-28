/**
 * @file    portableserver_functions.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_PORTABLESERVER_FUNCTIONS_H
#define TAOX11_PORTABLESERVER_FUNCTIONS_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"
#include "tao/x11/portable_server/PS_ForwardC.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    TAOX11_PortableServer_Export std::string ObjectId_to_string (
      const TAOX11_NAMESPACE::PortableServer::ObjectId &id);
    TAOX11_PortableServer_Export std::wstring ObjectId_to_wstring (
      const TAOX11_NAMESPACE::PortableServer::ObjectId &id);

    TAOX11_PortableServer_Export TAOX11_NAMESPACE::PortableServer::ObjectId string_to_ObjectId (
      const std::string& id);
    TAOX11_PortableServer_Export TAOX11_NAMESPACE::PortableServer::ObjectId wstring_to_ObjectId (
      const std::wstring& id);
  }
}

#endif /* TAOX11_PORTABLESERVER_FUNCTIONS_H */
