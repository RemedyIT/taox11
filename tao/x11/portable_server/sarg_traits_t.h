/**
 * @file    sarg_traits_t.h
 * @author  Martin Corino
 *
 * @brief   x11 skeleton argument traits template
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_SARG_TRAITS_T_H
#define TAOX11_SARG_TRAITS_T_H

#pragma once

#include /**/ "ace/pre.h"

#include "tao/x11/stddef.h"

namespace TAOX11_NAMESPACE
{
  namespace PS {
    /**
    *
    * @brief Base class for all skeleton arg traits specializations.
    *
    */
    template<typename T>
    class SArg_Traits
    {
    };
  } // namespace PS
} // namespace TAOX11_NAMESPACE


#include /**/ "ace/post.h"

#endif /* TAOX11_SARG_TRAITS_T_H */
