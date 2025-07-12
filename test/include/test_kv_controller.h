#pragma once

#include <stdio.h>
#include <stdint.h>

#include "unity.h"
#include "kv_controller.h"

static db_t* helper_create_and_validate_db(uint8_t *storage_type);
static db_entry_t* helper_create_and_validate_entry(uint8_t *key, uint8_t *value, uint8_t *type_str);
static void helper_test_null_inputs_db_key(int64_t  (*func)(db_t*, uint8_t*), int64_t  expected_error);
static void helper_test_empty_key(int64_t  (*func)(db_t*, uint8_t*), int64_t  expected_error);
static void helper_populate_db_with_sample_data(db_t *db);
static void helper_validate_sample_data(db_t *db);
static void helper_test_put_entry_all_types(db_t *db);
static void helper_test_both_storage_types(void (*test_func)(db_t*));
static void helper_test_save_null_inputs(db_t *db, uint8_t *path, int64_t  expected_error);
static void helper_test_load_null_inputs(db_t *db, uint8_t *path, int64_t  expected_error);
static void helper_test_nonexistent_key_get(db_t *db);
static void helper_test_nonexistent_key_delete(db_t *db);
static int64_t  helper_get_entry_wrapper(db_t *db, uint8_t *key);

static void test_create_db_valid_inputs();
static void test_create_db_null_input();
static void test_create_db_empty_input();
static void test_create_db_invalid_storage_type();
static void test_insert_entry_valid_list();
static void test_insert_entry_valid_hash();
static void test_insert_entry_null_inputs();
static void test_put_entry_valid_list();
static void test_put_entry_valid_hash();
static void test_put_entry_all_types_list();
static void test_put_entry_all_types_hash();
static void test_put_entry_null_inputs();
static void test_put_entry_empty_strings();
static void test_get_entry_valid_list();
static void test_get_entry_valid_hash();
static void test_get_entry_nonexistent_key();
static void test_get_entry_null_inputs();
static void test_get_entry_empty_key();
static void test_delete_entry_valid_list();
static void test_delete_entry_valid_hash();
static void test_delete_entry_nonexistent_key();
static void test_delete_entry_null_inputs();
static void test_delete_entry_empty_key();
static void test_save_load_db_valid_list();
static void test_save_load_db_valid_hash();
static void test_save_db_null_inputs();
static void test_save_db_empty_path();
static void test_load_db_null_inputs();
static void test_load_db_empty_path();
static void test_load_db_nonexistent_file();
static void test_free_db_valid();
static void test_free_db_null();
static void test_print_db_valid();
static void test_print_db_null();
static void test_multiple_entries_operations();
static void test_long_strings();

extern void setUp(void);
extern void tearDown(void);
extern int main();
