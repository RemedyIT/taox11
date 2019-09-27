/**
 * @file    pi.h
 * @author  Marcel Smit
 *
 * @brief   TAOX11 Portable Interceptor Framework
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_PI_H
#define TAOX11_PI_H

#pragma once

#include "tao/x11/pi/taox11_pi_export.h"

#define TAOX11_PI_SAFE_INCLUDE
#include "tao/x11/pi/ORBInitInfoC.h"
#include "tao/x11/pi/PolicyFactoryC.h"
#include "tao/x11/pi/ORBInitializerC.h"
#include "tao/x11/pi/InterceptorC.h"
#include "tao/x11/pi/ClientRequestInfoC.h"
#include "tao/x11/pi/ClientRequestInterceptorC.h"
#include "tao/x11/pi/PICurrentC.h"
#include "tao/x11/pi/ProcessingModePolicyC.h"
#undef TAOX11_PI_SAFE_INCLUDE

#endif  /* TAO_PI_H */
