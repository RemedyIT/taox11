/**
 * @file    portableserver.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core POA classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_PORTABLESERVER_H_INCLUDED
#define TAOX11_PORTABLESERVER_H_INCLUDED

#include "tao/x11/object.h"
#include "tao/x11/portable_server/taox11_portableserver_export.h"
#include "tao/x11/portable_server/POAC.h"
#include "tao/x11/portable_server/POAManagerC.h"
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
#include "tao/x11/portable_server/POAManagerFactoryC.h"
#endif
#include "tao/x11/portable_server/PS_CurrentC.h"
using namespace TAOX11_NAMESPACE;

#endif /* TAOX11_PORTABLESERVER_H_INCLUDED */
