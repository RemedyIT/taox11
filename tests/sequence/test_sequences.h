/**
 * @file    test_sequences.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TEST_SEQUENCES_H
#define TEST_SEQUENCES_H

#include "testC.h"

void
test_sequences (IDL::traits<Test::Foo>::ref_type foo, int &error_count);

#endif /* TEST_SEQUENCES_H */
