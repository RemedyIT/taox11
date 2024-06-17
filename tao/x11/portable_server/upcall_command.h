/**
 * @file    upcall_command.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 upcall command base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_UPCALL_COMMAND_H
#define TAOX11_UPCALL_COMMAND_H

#pragma once

#include "tao/x11/base/tao_corba.h"
#include "tao/x11/base/stddef.h"
#include "tao/x11/portable_server/taox11_portableserver_export.h"
#include "tao/PortableServer/Upcall_Command.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class Upcall_Command
   *
   * @brief "ABC" that all operation-specific command objects must
   *        subclass.
   *
   * See TAO::Upcall_Command.
   */
  class TAOX11_PortableServer_Export Upcall_Command final
    : public TAO_VERSIONED_NAMESPACE_NAME::TAO::Upcall_Command
  {
  public:
    Upcall_Command (std::function<void()> execute_function);

    /// Destructor.
    ~Upcall_Command () override = default;

    /// Calls the execute_function catching/wrapping any user exceptions.
    void execute () override;
  private:
    /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
    //@{
    Upcall_Command(const Upcall_Command&) = delete;
    Upcall_Command(Upcall_Command&&) = delete;
    Upcall_Command& operator=(const Upcall_Command&) = delete;
    Upcall_Command& operator=(Upcall_Command&&) = delete;
    //@}
    std::function<void()> execute_function_;
  };
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_UPCALL_COMMAND_H */
