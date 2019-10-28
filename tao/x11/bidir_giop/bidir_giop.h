/**
 * @file    bidir_giop.h
 * @author  Marcel Smit
 *
 * @brief   Loader of the BiDIR library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_BIDIR_GIOP_H
#define TAOX11_BIDIR_GIOP_H

#pragma once

#include "ace/Service_Object.h"

#include "tao/x11/bidir_giop/taox11_bidir_giop_export.h"

class TAOX11_BIDIRGIOP_Export TAOX11_BiDirGIOP_Loader final
  : public ACE_Service_Object
{
public:

  TAOX11_BiDirGIOP_Loader () = default;

  ~TAOX11_BiDirGIOP_Loader () = default;

  static int Initializer ();

private:
  TAOX11_BiDirGIOP_Loader (const TAOX11_BiDirGIOP_Loader&) = delete;
  TAOX11_BiDirGIOP_Loader (TAOX11_BiDirGIOP_Loader&&) = delete;
  TAOX11_BiDirGIOP_Loader& operator= (const TAOX11_BiDirGIOP_Loader&) = delete;
  TAOX11_BiDirGIOP_Loader& operator= (TAOX11_BiDirGIOP_Loader&&) = delete;
};

static int
TAOX11_Requires_BiDirGIOP_Initializer = TAOX11_BiDirGIOP_Loader::Initializer ();

ACE_STATIC_SVC_DECLARE (TAOX11_BiDirGIOP_Loader)
ACE_FACTORY_DECLARE (TAOX11_BIDIRGIOP, TAOX11_BiDirGIOP_Loader)

#define TAOX11_BIDIRGIOP_SAFE_INCLUDE
#include "tao/x11/bidir_giop/BiDirPolicyC.h"
#undef TAOX11_BIDIRGIOP_SAFE_INCLUDE

#endif /* TAOX11_BIDIR_GIOP_H */
