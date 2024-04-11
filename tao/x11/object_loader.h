/**
 * @file    object_loader.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 ORB core
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_OBJECT_LOADER_H
#define TAOX11_OBJECT_LOADER_H

#pragma once

#include "ace/Service_Object.h"
#include "tao/x11/base/taox11_export.h"
#include "tao/x11/orb.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class TAOX11_Object_Loader
   *
   * @brief A class to dynamically load object implementations into an
   * ORB.
   *
   * Many services and components of the ORB can be dynamically
   * loaded, often these components are returned to the application via
   * an object reference (CORBA::Object).  This class is used to
   * dynamically load such components, and encapsulate the creation of
   * the object reference.
   *
   */
  class TAOX11_Export Object_Loader
    : public ACE_Service_Object
  {
  public:
    /// The destructor
    virtual ~Object_Loader ();

    /**
     * Create and activate a new object into the orb.
     * This method cannot throw any exception, but it can return a nil
     * object to indicate an error condition.
     */
    virtual TAOX11_CORBA::object_reference<TAOX11_CORBA::Object> create_object (
      TAOX11_CORBA::object_reference<TAOX11_CORBA::ORB> orb,
      int argc,
      char* argv []) = 0;
  };
}

#endif /* TAOX11_OBJECT_LOADER_H */
