/**
 * @file    object_reference_template.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 ObjectReferenceTemplate
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_OBJREF_TEMPLATE_H
#define TAOX11_OBJREF_TEMPLATE_H

#pragma once

#include "tao/x11/stddef.h"

#include "tao/x11/ort/taox11_ort_export.h"

namespace TAOX11_NAMESPACE
{
  class TAOX11_ObjRef_Template_Export ObjectReferenceTemplate_Init final
  {
  public:

    /// Used to force the initialization of the ORT code.
    static int Initializer ();
  };

  static int
  TAOX11_Requires_ObjectReferenceTemplate_Initializer =
      ObjectReferenceTemplate_Init::Initializer ();
}

#define TAOX11_OBJREF_TEMPLATE_SAFE_INCLUDE
#include "tao/x11/ort/ObjectReferenceTemplateC.h"
#include "tao/x11/ort/ObjectReferenceTemplate_includeC.h"
#undef TAOX11_OBJREF_TEMPLATE_SAFE_INCLUDE

#endif  /* TAOX11_OBJREF_TEMPLATE_H */
