#include "test_kv_controller.h"

static db_t* helper_create_and_validate_db(uint8_t *storage_type) {
  db_t *db = create_db(storage_type);
  TEST_ASSERT_NOT_NULL(db);
  TEST_ASSERT_EQUAL_STRING(storage_type, db->storage_type);
  TEST_ASSERT_NOT_NULL(db->storage);
  return db;
}

static db_entry_t* helper_create_and_validate_entry(uint8_t *key, uint8_t *value, uint8_t *type_str) {
  db_entry_t *entry = create_entry(key, value, type_str);
  TEST_ASSERT_NOT_NULL(entry);
  return entry;
}

static void helper_test_null_inputs_db_key(int64_t  (*func)(db_t*, uint8_t*), int64_t  expected_error) {
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_EQUAL(expected_error, func(NULL, "test_key"));
  TEST_ASSERT_EQUAL(expected_error, func(db, NULL));
  
  free_db(db);
}

static void helper_test_empty_key(int64_t  (*func)(db_t*, uint8_t*), int64_t  expected_error) {
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_EQUAL(expected_error, func(db, ""));
  
  free_db(db);
}

static void helper_populate_db_with_sample_data(db_t *db) {
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key1", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key2", "3.14", FLOAT_TYPE_STR));
}

static void helper_validate_sample_data(db_t *db) {
  db_entry_t *entry1 = get_entry(db, "key1");
  TEST_ASSERT_NOT_NULL(entry1);
  TEST_ASSERT_EQUAL(INT32_TYPE, entry1->type);
  TEST_ASSERT_EQUAL(42, *(int32_t*)entry1->value);
  
  db_entry_t *entry2 = get_entry(db, "key2");
  TEST_ASSERT_NOT_NULL(entry2);
  TEST_ASSERT_EQUAL(FLOAT_TYPE, entry2->type);
  TEST_ASSERT_EQUAL_FLOAT(3.14, *(float*)entry2->value);
}

static void helper_test_put_entry_all_types(db_t *db) {
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int8_key", "127", INT8_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int16_key", "32767", INT16_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int32_key", "2147483647", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int64_key", "9223372036854775807", INT64_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "float_key", "3.14", FLOAT_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "double_key", "3.141592653589793", DOUBLE_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "bool_key", "true", BOOL_TYPE_STR));
}

static void helper_test_both_storage_types(void (*test_func)(db_t*)) {
  db_t *db_list = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  db_t *db_hash = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  
  test_func(db_list);
  test_func(db_hash);
  
  free_db(db_list);
  free_db(db_hash);
}

static void helper_test_save_null_inputs(db_t *db, uint8_t *path, int64_t  expected_error) {
  TEST_ASSERT_EQUAL(expected_error, save_db(NULL, path));
  TEST_ASSERT_EQUAL(expected_error, save_db(db, NULL));
}

static void helper_test_load_null_inputs(db_t *db, uint8_t *path, int64_t  expected_error) {
  TEST_ASSERT_EQUAL(expected_error, load_db(NULL, path));
  TEST_ASSERT_EQUAL(expected_error, load_db(db, NULL));
}

static void test_create_db_valid_inputs() {
  logger(4, "*** test_create_db_valid_inputs ***\n");
  db_t *db_list = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  free_db(db_list);
  
  db_t *db_hash = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  free_db(db_hash);
}

static void test_create_db_null_input() {
  logger(4, "*** test_create_db_null_input ***\n");  
  db_t *db = create_db(NULL);
  TEST_ASSERT_NULL(db);
}

static void test_create_db_empty_input() {
  logger(4, "*** test_create_db_empty_input ***\n");
  db_t *db = create_db("");
  TEST_ASSERT_NULL(db);
}

static void test_create_db_invalid_storage_type() {
  logger(4, "*** test_create_db_invalid_storage_type ***\n");
  db_t *db = create_db("INVALID");
  TEST_ASSERT_NULL(db);
}

static void test_insert_entry_valid_list() {
  logger(4, "*** test_insert_entry_valid_list ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  db_entry_t *entry = helper_create_and_validate_entry("test_key", "42", INT32_TYPE_STR);
  
  int64_t result = insert_entry(db, entry);
  TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
  list_t *list = (list_t*)db->storage;
  db_entry_t *read_entry = list_get_entry_by_key(list, "test_key");
  TEST_ASSERT_NOT_NULL(read_entry);
  TEST_ASSERT_EQUAL_PTR(entry, read_entry);

  free_db(db);
}

static void test_insert_entry_valid_hash() {
  logger(4, "*** test_insert_entry_valid_hash ***\n");
  
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  
  db_entry_t *entry = helper_create_and_validate_entry("test_key", "42", INT32_TYPE_STR);
  
  int64_t result = insert_entry(db, entry);
  TEST_ASSERT_GREATER_OR_EQUAL(0, result);

  hash_table_t *hash = (hash_table_t*)db->storage;
  db_entry_t *read_entry = hash_get_entry(hash, "test_key");
  TEST_ASSERT_NOT_NULL(read_entry);
  TEST_ASSERT_EQUAL_PTR(entry, read_entry);
  
  free_db(db);
}

static void test_insert_entry_null_inputs() {
  logger(4, "*** test_insert_entry_null_db ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);

  db_entry_t *entry = helper_create_and_validate_entry("test_key", "42", INT32_TYPE_STR);

  TEST_ASSERT_EQUAL(-1, insert_entry(NULL, entry));
  TEST_ASSERT_EQUAL(-1, insert_entry(db, NULL));
  
  free_entry(entry);
  free_db(db);
}

static void test_put_entry_valid_list() {
  logger(4, "*** test_put_entry_valid_list ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);

  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key", "42", INT32_TYPE_STR));
  
  free_db(db);
}

static void test_put_entry_valid_hash() {
  logger(4, "*** test_put_entry_valid_hash ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  
  int64_t result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
  TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
  free_db(db);
}

static void test_put_entry_all_types_list() {
  logger(4, "*** test_put_entry_all_types ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  
  helper_test_put_entry_all_types(db);
  
  free_db(db);
}

static void test_put_entry_all_types_hash() {
  logger(4, "*** test_put_entry_all_types ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  helper_test_put_entry_all_types(db);
  
  free_db(db);
}

static void test_put_entry_null_inputs() {
  logger(4, "*** test_put_entry_null_inputs ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);

  TEST_ASSERT_EQUAL(-1, put_entry(NULL, "test_key",  "42", INT32_TYPE_STR));
  TEST_ASSERT_EQUAL(-1, put_entry(  db,       NULL,  "42", INT32_TYPE_STR));
  TEST_ASSERT_EQUAL(-1, put_entry(  db, "test_key",  NULL, INT32_TYPE_STR));
  TEST_ASSERT_EQUAL(-1, put_entry(  db, "test_key",  "42",           NULL));
  
  free_db(db);
}

static void test_put_entry_empty_strings() {
  logger(4, "*** test_put_entry_empty_strings ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_EQUAL(-1, put_entry(db,         "", "42", INT32_TYPE_STR));
  TEST_ASSERT_EQUAL(-1, put_entry(db, "test_key",   "", INT32_TYPE_STR));
  TEST_ASSERT_EQUAL(-1, put_entry(db, "test_key", "42",             ""));
  
  free_db(db);
}

static void test_get_entry_valid_list() {
  logger(4, "*** test_get_entry_valid_list ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key", "42", INT32_TYPE_STR));
  
  db_entry_t *entry = get_entry(db, "test_key");
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL_STRING("test_key", entry->key);
  TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
  TEST_ASSERT_EQUAL(42, *(int32_t*)entry->value);
  
  free_db(db);
}

static void test_get_entry_valid_hash() {
  logger(4, "*** test_get_entry_valid_hash ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key", "42", INT32_TYPE_STR));
  
  db_entry_t *entry = get_entry(db, "test_key");
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL_STRING("test_key", entry->key);
  TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
  TEST_ASSERT_EQUAL(42, *(int32_t*)entry->value);
  
  free_db(db);
}

static void helper_test_nonexistent_key_get(db_t *db) {
  db_entry_t *entry = get_entry(db, "nonexistent_key");
  TEST_ASSERT_NULL(entry);
}

static void helper_test_nonexistent_key_delete(db_t *db) {
  TEST_ASSERT_EQUAL(-1, delete_entry(db, "nonexistent_key"));
}

static void test_get_entry_nonexistent_key() {
  logger(4, "*** test_get_entry_nonexistent_key ***\n");
  helper_test_both_storage_types(helper_test_nonexistent_key_get);
}

static int64_t  helper_get_entry_wrapper(db_t *db, uint8_t *key) {
  db_entry_t *entry = get_entry(db, key);
  return entry ? 0 : -1;
}

static void test_get_entry_null_inputs() {
  logger(4, "*** test_get_entry_null_db ***\n");
  helper_test_null_inputs_db_key(helper_get_entry_wrapper, -1);
}

static void test_get_entry_empty_key() {
  logger(4, "*** test_get_entry_empty_key ***\n");
  helper_test_empty_key(helper_get_entry_wrapper, -1);
}

static void test_delete_entry_valid_list() {
  logger(4, "*** test_delete_entry_valid_list ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, delete_entry(db, "test_key"));
  
  db_entry_t *entry = get_entry(db, "test_key");
  TEST_ASSERT_NULL(entry);
  
  free_db(db);
}

static void test_delete_entry_valid_hash() {
  logger(4, "*** test_delete_entry_valid_hash ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, delete_entry(db, "test_key"));

  db_entry_t *entry = get_entry(db, "test_key");
  TEST_ASSERT_NULL(entry);
  
  free_db(db);
}

static void test_delete_entry_nonexistent_key() {
  logger(4, "*** test_delete_entry_nonexistent_key ***\n");
  helper_test_both_storage_types(helper_test_nonexistent_key_delete);
}

static void test_delete_entry_null_inputs() {
  logger(4, "*** test_delete_entry_null_db ***\n");
  helper_test_null_inputs_db_key(delete_entry, -1);
}

static void test_delete_entry_empty_key() {
  logger(4, "*** test_delete_entry_empty_key ***\n");
  helper_test_empty_key(delete_entry, -1);
}

static void test_save_load_db_valid_list() {
  logger(4, "*** test_save_load_db_valid_list ***\n");
  uint8_t *file_path = "./test_db.db";

  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  helper_populate_db_with_sample_data(db);
  TEST_ASSERT_GREATER_OR_EQUAL(0, save_db(db, file_path));
  
  db_t *new_db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_db(new_db, file_path));
  
  helper_validate_sample_data(new_db);
  
  free_db(db);
  free_db(new_db);
  remove(file_path);
}

static void test_save_load_db_valid_hash() {
  logger(4, "*** test_save_load_db_valid_hash ***\n");
  uint8_t *file_path = "/tmp/test_db_hash.db";
  
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  helper_populate_db_with_sample_data(db);
  TEST_ASSERT_GREATER_OR_EQUAL(0, save_db(db, file_path));
  
  db_t *new_db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_HASH);
  TEST_ASSERT_GREATER_OR_EQUAL(0, load_db(new_db, file_path));
  
  helper_validate_sample_data(new_db);
  
  free_db(db);
  free_db(new_db);
  remove(file_path);
}

static void test_save_db_null_inputs() {
  logger(4, "*** test_save_db_null_inputs ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  helper_test_save_null_inputs(db, "/tmp/test.db", -1);

  free_db(db);
}

static void test_save_db_empty_path() {
  logger(4, "*** test_save_db_empty_path ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_EQUAL(-1, save_db(db, ""));
  free_db(db);
}

static void test_load_db_null_inputs() {
  logger(4, "*** test_load_db_null_inputs ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);

  helper_test_load_null_inputs(db, "/tmp/test.db", -1);

  free_db(db);
}

static void test_load_db_empty_path() {
  logger(4, "*** test_load_db_empty_path ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_EQUAL(-1, load_db(db, ""));
  free_db(db);
}

static void test_load_db_nonexistent_file() {
  logger(4, "*** test_load_db_nonexistent_file ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_EQUAL(-1, load_db(db, "/tmp/nonexistent_file.db"));
  
  free_db(db);
}

static void test_free_db_valid() {
  logger(4, "*** test_free_db_valid ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  free_db(db);
}

static void test_free_db_null() {
  logger(4, "*** test_free_db_null ***\n");
  free_db(NULL);
}

static void test_print_db_valid() {
  logger(4, "*** test_print_db_valid ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key_1", "42", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "test_key_2", "24", INT32_TYPE_STR));
  
  print_db(db);
  
  free_db(db);
}

static void test_print_db_null() {
  logger(4, "*** test_print_db_null ***\n");
  print_db(NULL);
}

static void test_multiple_entries_operations() {
  logger(4, "*** test_multiple_entries_operations ***\n");
  db_t *db = helper_create_and_validate_db(KV_STORAGE_STRUCTURE_LIST);
  
  for (uint64_t  i = 0; i < 10; i++) {
    char key[32];
    char value[32];
    snprintf(key, sizeof(key), "key_%u", i);
    snprintf(value, sizeof(value), "%u", i * 10);
    TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, key, value, INT32_TYPE_STR));
  }
  
  for (uint64_t  i = 0; i < 10; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key_%u", i);
    
    db_entry_t *entry = get_entry(db, key);
    TEST_ASSERT_NOT_NULL(entry);
    TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
    TEST_ASSERT_EQUAL(i * 10, *(int32_t*)entry->value);
  }
  
  for (uint64_t  i = 0; i < 5; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key_%u", i);
    TEST_ASSERT_GREATER_OR_EQUAL(0, delete_entry(db, key));
  }
  
  for (uint64_t  i = 0; i < 5; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key_%u", i);
    TEST_ASSERT_NULL(get_entry(db, key));
  }
  
  for (uint64_t  i = 5; i < 10; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key_%u", i);
    db_entry_t *entry = get_entry(db, key);
    TEST_ASSERT_NOT_NULL(entry);
    TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
    TEST_ASSERT_EQUAL(i * 10, *(int32_t*)entry->value);
  }
  
  free_db(db);
}

extern void setUp(void) {
  // set stuff up here
}

extern void tearDown(void) {
  // clean stuff up here
}

extern int main() {
  UNITY_BEGIN();
  
  // create_db tests
  RUN_TEST(test_create_db_valid_inputs);
  RUN_TEST(test_create_db_null_input);
  RUN_TEST(test_create_db_empty_input);
  RUN_TEST(test_create_db_invalid_storage_type);
  
  // insert_entry tests
  RUN_TEST(test_insert_entry_valid_list);
  RUN_TEST(test_insert_entry_valid_hash);
  RUN_TEST(test_insert_entry_null_inputs);
  
  // put_entry tests
  RUN_TEST(test_put_entry_valid_list);
  RUN_TEST(test_put_entry_valid_hash);
  RUN_TEST(test_put_entry_all_types_list);
  RUN_TEST(test_put_entry_all_types_hash);
  RUN_TEST(test_put_entry_null_inputs);
  RUN_TEST(test_put_entry_empty_strings);
  
  // get_entry tests
  RUN_TEST(test_get_entry_valid_list);
  RUN_TEST(test_get_entry_valid_hash);
  RUN_TEST(test_get_entry_nonexistent_key);
  RUN_TEST(test_get_entry_null_inputs);
  RUN_TEST(test_get_entry_empty_key);
  
  // delete_entry tests
  RUN_TEST(test_delete_entry_valid_list);
  RUN_TEST(test_delete_entry_valid_hash);
  RUN_TEST(test_delete_entry_nonexistent_key);
  RUN_TEST(test_delete_entry_null_inputs);
  RUN_TEST(test_delete_entry_empty_key);
  
  // save_db and load_db tests
  RUN_TEST(test_save_load_db_valid_list);
  RUN_TEST(test_save_load_db_valid_hash);
  RUN_TEST(test_save_db_null_inputs);
  RUN_TEST(test_save_db_empty_path);
  RUN_TEST(test_load_db_null_inputs);
  RUN_TEST(test_load_db_empty_path);
  RUN_TEST(test_load_db_nonexistent_file);
  
  // free_db and print_db tests
  RUN_TEST(test_free_db_valid);
  RUN_TEST(test_free_db_null);
  RUN_TEST(test_print_db_valid);
  RUN_TEST(test_print_db_null);
  
  // Integration and edge case tests
  RUN_TEST(test_multiple_entries_operations);
  
  return UNITY_END();
}