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

#include "ace/config-macros.h"

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
 * Provide very useful truncate_cast<> templates
 */
#include "ace/Truncate.h"

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
#include "tao/orbconf.h"

#include "tao/x11/taox11_export.h"
#include "tao/x11/versioned_x11_namespace.h"

#define TAOX11_CORBA \
  TAOX11_NAMESPACE::CORBA

#define TAOX11_IDL \
  TAOX11_NAMESPACE::IDL

namespace TAOX11_NAMESPACE {

  template <typename SEQ_0X, typename SEQ>
  SEQ_0X seq_to_x11(const SEQ& seq_in)
  {
    SEQ_0X seq_out;
    uint32_t len = seq_in.length ();
    seq_out.resize (len);
    for (uint32_t i = 0; i < len ;++i)
    {
      seq_out[i] = seq_in[i];
    }
    return seq_out;
  }

  template <typename SEQ_0X, typename TO_0X, typename SEQ>
  SEQ_0X convert_seq_to_x11(const SEQ& seq_in)
  {
    SEQ_0X seq_out;
    uint32_t len = seq_in.length ();
    seq_out.resize (len);
    for (uint32_t i = 0; i < len ;++i)
    {
      seq_out[i] = TO_0X (seq_in[i]);
    }
    return seq_out;
  }


  template <typename SEQ, typename SEQ_0X>
  SEQ* seq_from_x11(const SEQ_0X& seq_in)
  {
    SEQ* seq_out = new SEQ();
    uint32_t len = seq_in.size ();
    seq_out->length (len);
    for (uint32_t i = 0; i < len ;++i)
    {
      (*seq_out)[i] = seq_in[i];
    }
    return seq_out;
  }

  template <typename SEQ, typename SEQ_0X>
  void seq_from_x11(SEQ& seq_out, const SEQ_0X& seq_in)
  {
    uint32_t len = ACE_Utils::truncate_cast<uint32_t> (seq_in.size ());
    seq_out.length (len);
    for (uint32_t i = 0; i < len ;++i)
    {
      seq_out[i] = seq_in[i];
    }
  }
} // namespace TAOX11_NAMESPACE

#endif
