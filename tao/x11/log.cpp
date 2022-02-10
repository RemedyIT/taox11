/**
 * @file    log.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   TAOX11 logger implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/log.h"

#if !defined(X11_NLOGGING)

namespace TAOX11_NAMESPACE
{

  namespace taox11_logger
    {
      TAOX11_Log_Module*
      TAOX11_Log_Module::getInstance()
      {
        static TAOX11_Log_Module instance_;

        return std::addressof(instance_);
      }

      TAOX11_Log_Module::TAOX11_Log_Module()
      : x11_logger::Log_Module ("TAOX11")
      {
      }

      TAOX11_Log_Module::~TAOX11_Log_Module()
      {
      }

    } // namespace taox11_logger
} // namespace TAOX11_NAMESPACE

#endif
