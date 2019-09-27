/**
 * @file    stdext.h
 * @author  Martin Corino
 *
 * @brief   Platform dependent std namespace extensions/additions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_STDEXT_H_INCLUDED
#define TAOX11_STDEXT_H_INCLUDED

#pragma once

/*
 * Provide std::strtoXX for crippled platforms.
 */
#include "tao/x11/ext/std_str_to.h"
/*
 * Provide std::to_string for crippled platforms.
 */
#include "tao/x11/ext/std_to_string.h"
/*
 * Provide std::make_unique older/crippled platforms
 */
#include "tao/x11/ext/std_make_unique_t.h"
/*
 * Provide std::mutex for crippled platforms.
 */
#include "tao/x11/ext/std_mutex.h"
/*
 * Provide std::condition_variable for crippled platforms.
 */
#include "tao/x11/ext/std_condition_variable.h"
/*
 * Provide std::this_thread for crippled platforms.
 */
#include "tao/x11/ext/std_thread.h"

#endif
