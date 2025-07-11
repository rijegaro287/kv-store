#pragma once

#include <stdio.h>
#include <stdint.h>

#include "unity.h"
#include "kv_parser.h"

static void test_create_entry_valid_inputs();
static void test_create_entry_null_inputs();
static void test_create_entry_invalid_inputs();
static void test_set_entry_value_all_types();
static void test_set_entry_value_null_inputs();
static void test_set_entry_value_invalid_inputs();
static void test_update_entry_valid();
static void test_update_entry_all_types();
static void test_update_entry_null_inputs();
static void test_update_entry_invalid_inputs();
static void test_parse_entry_valid();
static void test_parse_entry_all_types();
static void test_parse_entry_null_inputs();
static void test_parse_entry_invalid_inputs();
static void test_parse_line_valid_entry();
static void test_parse_line_all_types();
static void test_parse_line_comment_line();
static void test_parse_line_empty_line();
static void test_parse_line_malformed_entry();
static void test_parse_line_null_input();
static void test_free_entry_valid();
static void test_free_entry_null();
static void test_print_entry_all_types();
static void test_print_entry_null_input();
static void test_print_entry_invalid_type();

extern void setUp(void);
extern void tearDown(void);
extern int64_t main();
