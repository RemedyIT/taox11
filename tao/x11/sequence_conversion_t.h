/**
 * @file    sequence_conversion_t.h
 * @author  Johnny Willemsen
 *
 * @brief   TAOX11 helper templates to convert sequence between TAO and TAOX11
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SEQUENCE_CONVERSION_T_H_INCLUDED
#define TAOX11_SEQUENCE_CONVERSION_T_H_INCLUDED

#pragma once

#include "tao/x11/stddef.h"
#include "ace/Truncate.h"

namespace TAOX11_NAMESPACE {

  template <typename SEQ_0X, typename SEQ>
  SEQ_0X seq_to_x11(const SEQ& seq_in)
  {
    SEQ_0X seq_out;
    uint32_t const len = seq_in.length ();
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
    uint32_t const len = seq_in.length ();
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
    uint32_t const len = seq_in.size ();
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
    uint32_t const len = ACE_Utils::truncate_cast<uint32_t> (seq_in.size ());
    seq_out.length (len);
    for (uint32_t i = 0; i < len ;++i)
    {
      seq_out[i] = seq_in[i];
    }
  }
} // namespace TAOX11_NAMESPACE

#endif
