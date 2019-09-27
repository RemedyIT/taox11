/**
 * @file    messaging.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Messaging library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_MESSAGING_H
#define TAOX11_MESSAGING_H

#include "ace/Service_Object.h"

#include "tao/x11/messaging/taox11_messaging_export.h"

class TAOX11_MESSAGING_Export TAOX11_Messaging_Loader final
  : public ACE_Service_Object
{
public:
  TAOX11_Messaging_Loader () = default;

  ~TAOX11_Messaging_Loader () = default;

  static int Initializer ();
private:
  TAOX11_Messaging_Loader (const TAOX11_Messaging_Loader&) = delete;
  TAOX11_Messaging_Loader (TAOX11_Messaging_Loader&&) = delete;
  TAOX11_Messaging_Loader& operator= (const TAOX11_Messaging_Loader&) = delete;
  TAOX11_Messaging_Loader& operator= (TAOX11_Messaging_Loader&&) = delete;
};

static int TAOX11_Messaging_Initializer =
  TAOX11_Messaging_Loader::Initializer ();

ACE_STATIC_SVC_DECLARE (TAOX11_Messaging_Loader)
ACE_FACTORY_DECLARE (TAOX11_MESSAGING, TAOX11_Messaging_Loader)

#define TAOX11_MESSAGING_SAFE_INCLUDE
#include "tao/x11/messaging/MessagingC.h"
#undef TAOX11_MESSAGING_SAFE_INCLUDE

#endif /* TAOX11_MESSAGING_H */
