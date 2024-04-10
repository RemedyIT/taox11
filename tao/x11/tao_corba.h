/**
 * @file    tao_corba.h
 * @author  Martin Corino
 *
 * @brief   include basic TAO CORBA definitions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_TAO_CORBA_H_INCLUDED
#define TAOX11_TAO_CORBA_H_INCLUDED

#pragma once

#if __has_include("tao/corba.h")
# include "tao/corba.h"
#endif

#if __has_include("tao/Versioned_Namespace.h")
# include /**/ "tao/Versioned_Namespace.h"
# define TAO_CORBA TAO_VERSIONED_NAMESPACE_NAME::CORBA
# define TAO_TAO TAO_VERSIONED_NAMESPACE_NAME::TAO
# define TAO_IOP TAO_VERSIONED_NAMESPACE_NAME::IOP
#else
# define TAO_CORBA
# define TAO_TAO
# define TAO_IOP
#endif

#endif // TAOX11_TAO_CORBA_H_INCLUDED
