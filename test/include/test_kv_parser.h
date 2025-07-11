#pragma once

#include <stdio.h>
#include <stdint.h>

#include "unity.h"
#include "kv_parser.h"

static void test_create_entry_valid_inputs();
static void test_create_entry_null_inputs();
static void test_create_entry_invalid_type();
static void test_set_entry_value_all_types();

extern void setUp(void);
extern void tearDown(void);

extern int64_t test_kv_parser();
