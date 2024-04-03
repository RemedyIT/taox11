/**
 * @file    stddef.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 standard includes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_STDDEF_H_INCLUDED
#define TAOX11_STDDEF_H_INCLUDED

#pragma once

//#include "ace/config-macros.h"

#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <cwchar>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>
#include <array>
#include <iostream>
#include <cstring>

/*
 * Provide std namespace extensions/additions for older/crippled platforms.
 */
#include "tao/x11/ext/stdext.h"

// undefine GNU dev macro
#if defined(minor)
# undef minor
#endif

// undefine msvc min macro
#if defined (min)
# undef min
#endif

// undefine msvc max macro
#if defined (max)
# undef max
#endif

#define X11_UNUSED_ARG(a) ACE_UNUSED_ARG(a)

/* This has to be included here so both library code
 * as well as user code get build according to the
 * macros that get tested and defined inside this file.
 */
#if __has_include("tao/orbconf.h")
# include "tao/orbconf.h"
#endif
#include "tao/x11/taox11_export.h"
#include "tao/x11/versioned_x11_namespace.h"

#define TAOX11_CORBA TAOX11_NAMESPACE::CORBA

#define TAOX11_IDL TAOX11_NAMESPACE::IDL

#endif
