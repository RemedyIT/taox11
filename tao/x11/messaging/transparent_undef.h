/**
 * @file    transparent_undef.h
 * @author  Martin Corino
 *
 * @brief   protection against TRANSPARENT macro
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_TRANSPARENT_UNDEF_H
#define TAOX11_TRANSPARENT_UNDEF_H

#if defined(TRANSPARENT)
# undef TRANSPARENT
#endif

#endif
