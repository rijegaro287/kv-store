#include "test_kv_controller.h"

extern void setUp(void) {
  // set stuff up here
}

extern void tearDown(void) {
  // clean stuff up here
}

static void test_create_db_valid_inputs() {
  logger(4, "*** test_create_db_valid_inputs ***\n");
  db_t *db_list = create_db(KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(db_list);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_LIST, db_list->storage_type);
  TEST_ASSERT_NOT_NULL(db_list->storage);
  free_db(db_list);
  
  db_t *db_hash = create_db(KV_STORAGE_STRUCTURE_HASH);
  TEST_ASSERT_NOT_NULL(db_hash);
  TEST_ASSERT_EQUAL_STRING(KV_STORAGE_STRUCTURE_HASH, db_hash->storage_type);
  TEST_ASSERT_NOT_NULL(db_hash->storage);
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
  db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(db);
  
  db_entry_t *entry = create_entry("test_key", "42", INT32_TYPE_STR);
  TEST_ASSERT_NOT_NULL(entry);
  
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
  
  db_t *db = create_db(KV_STORAGE_STRUCTURE_HASH);
  TEST_ASSERT_NOT_NULL(db);
  
  db_entry_t *entry = create_entry("test_key", "42", INT32_TYPE_STR);
  TEST_ASSERT_NOT_NULL(entry);
  
  int64_t result = insert_entry(db, entry);
  TEST_ASSERT_GREATER_OR_EQUAL(0, result);

  hash_table_t *hash = (hash_table_t*)db->storage;
  db_entry_t *read_entry = hash_get_entry(hash, "test_key");
  TEST_ASSERT_NOT_NULL(read_entry);
  TEST_ASSERT_EQUAL_PTR(entry, read_entry);
  
  free_db(db);
}

static void test_insert_entry_null_db() {
  logger(4, "*** test_insert_entry_null_db ***\n");
  db_entry_t *entry = create_entry("test_key", "42", INT32_TYPE_STR);
  TEST_ASSERT_NOT_NULL(entry);

  TEST_ASSERT_EQUAL(-1, insert_entry(NULL, entry));
  
  free_entry(entry);
}

static void test_insert_entry_null_entry() {
  logger(4, "*** test_insert_entry_null_entry ***\n");
  
  db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
  TEST_ASSERT_NOT_NULL(db);

  TEST_ASSERT_EQUAL(-1, insert_entry(db, NULL));
  
  free_db(db);
}

// static void test_put_entry_valid_list() {
//   logger(4, "*** test_put_entry_valid_list ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
//   free_db(db);
// }

// // Test put_entry with valid inputs for hash storage
// static void test_put_entry_valid_hash() {
//   logger(4, "*** test_put_entry_valid_hash ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_HASH);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
//   free_db(db);
// }

// // Test put_entry with all data types
// static void test_put_entry_all_types() {
//   logger(4, "*** test_put_entry_all_types ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int8_key", "127", INT8_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int16_key", "32767", INT16_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int32_key", "2147483647", INT32_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "int64_key", "9223372036854775807", INT64_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "float_key", "3.14", FLOAT_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "double_key", "3.141592653589793", DOUBLE_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "bool_key", "true", BOOL_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "string_key", "hello world", STR_TYPE_STR));
  
//   free_db(db);
// }

// // Test put_entry with NULL database
// static void test_put_entry_null_db() {
//   logger(4, "*** test_put_entry_null_db ***\n");
  
//   int64_t result = put_entry(NULL, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_EQUAL(-1, result);
// }

// // Test put_entry with NULL key
// static void test_put_entry_null_key() {
//   logger(4, "*** test_put_entry_null_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = put_entry(db, NULL, "42", INT32_TYPE_STR);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test put_entry with NULL value
// static void test_put_entry_null_value() {
//   logger(4, "*** test_put_entry_null_value ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = put_entry(db, "test_key", NULL, INT32_TYPE_STR);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test put_entry with NULL type
// static void test_put_entry_null_type() {
//   logger(4, "*** test_put_entry_null_type ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = put_entry(db, "test_key", "42", NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test put_entry with empty strings
// static void test_put_entry_empty_strings() {
//   logger(4, "*** test_put_entry_empty_strings ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   TEST_ASSERT_EQUAL(-1, put_entry(db, "", "42", INT32_TYPE_STR));
//   TEST_ASSERT_EQUAL(-1, put_entry(db, "test_key", "", INT32_TYPE_STR));
//   TEST_ASSERT_EQUAL(-1, put_entry(db, "test_key", "42", ""));
  
//   free_db(db);
// }

// // Test get_entry with valid key for list storage
// static void test_get_entry_valid_list() {
//   logger(4, "*** test_get_entry_valid_list ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t put_result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_result);
  
//   db_entry_t *entry = get_entry(db, "test_key");
//   TEST_ASSERT_NOT_NULL(entry);
//   TEST_ASSERT_EQUAL_STRING("test_key", entry->key);
//   TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
//   TEST_ASSERT_EQUAL(42, *(int32_t*)entry->value);
  
//   free_db(db);
// }

// // Test get_entry with valid key for hash storage
// static void test_get_entry_valid_hash() {
//   logger(4, "*** test_get_entry_valid_hash ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_HASH);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t put_result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_result);
  
//   db_entry_t *entry = get_entry(db, "test_key");
//   TEST_ASSERT_NOT_NULL(entry);
//   TEST_ASSERT_EQUAL_STRING("test_key", entry->key);
//   TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
//   TEST_ASSERT_EQUAL(42, *(int32_t*)entry->value);
  
//   free_db(db);
// }

// // Test get_entry with non-existent key
// static void test_get_entry_nonexistent_key() {
//   logger(4, "*** test_get_entry_nonexistent_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   db_entry_t *entry = get_entry(db, "nonexistent_key");
//   TEST_ASSERT_NULL(entry);
  
//   free_db(db);
// }

// // Test get_entry with NULL database
// static void test_get_entry_null_db() {
//   logger(4, "*** test_get_entry_null_db ***\n");
  
//   db_entry_t *entry = get_entry(NULL, "test_key");
//   TEST_ASSERT_NULL(entry);
// }

// // Test get_entry with NULL key
// static void test_get_entry_null_key() {
//   logger(4, "*** test_get_entry_null_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   db_entry_t *entry = get_entry(db, NULL);
//   TEST_ASSERT_NULL(entry);
  
//   free_db(db);
// }

// // Test get_entry with empty key
// static void test_get_entry_empty_key() {
//   logger(4, "*** test_get_entry_empty_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   db_entry_t *entry = get_entry(db, "");
//   TEST_ASSERT_NULL(entry);
  
//   free_db(db);
// }

// // Test delete_entry with valid key for list storage
// static void test_delete_entry_valid_list() {
//   logger(4, "*** test_delete_entry_valid_list ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t put_result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_result);
  
//   int64_t delete_result = delete_entry(db, "test_key");
//   TEST_ASSERT_GREATER_OR_EQUAL(0, delete_result);
  
//   // Verify entry is deleted
//   db_entry_t *entry = get_entry(db, "test_key");
//   TEST_ASSERT_NULL(entry);
  
//   free_db(db);
// }

// // Test delete_entry with valid key for hash storage
// static void test_delete_entry_valid_hash() {
//   logger(4, "*** test_delete_entry_valid_hash ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_HASH);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t put_result = put_entry(db, "test_key", "42", INT32_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_result);
  
//   int64_t delete_result = delete_entry(db, "test_key");
//   TEST_ASSERT_GREATER_OR_EQUAL(0, delete_result);
  
//   // Verify entry is deleted
//   db_entry_t *entry = get_entry(db, "test_key");
//   TEST_ASSERT_NULL(entry);
  
//   free_db(db);
// }

// // Test delete_entry with non-existent key
// static void test_delete_entry_nonexistent_key() {
//   logger(4, "*** test_delete_entry_nonexistent_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = delete_entry(db, "nonexistent_key");
//   TEST_ASSERT_LESS_THAN(0, result);
  
//   free_db(db);
// }

// // Test delete_entry with NULL database
// static void test_delete_entry_null_db() {
//   logger(4, "*** test_delete_entry_null_db ***\n");
  
//   int64_t result = delete_entry(NULL, "test_key");
//   TEST_ASSERT_EQUAL(-1, result);
// }

// // Test delete_entry with NULL key
// static void test_delete_entry_null_key() {
//   logger(4, "*** test_delete_entry_null_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = delete_entry(db, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test delete_entry with empty key
// static void test_delete_entry_empty_key() {
//   logger(4, "*** test_delete_entry_empty_key ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = delete_entry(db, "");
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test save_db and load_db with valid file path for list storage
// static void test_save_load_db_valid_list() {
//   logger(4, "*** test_save_load_db_valid_list ***\n");
  
//   // Create and populate database
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key1", "42", INT32_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key2", "3.14", FLOAT_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key3", "hello", STR_TYPE_STR));
  
//   // Save database
//   uint8_t *file_path = "/tmp/test_db_list.db";
//   int64_t save_result = save_db(db, file_path);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, save_result);
  
//   // Create new database and load
//   db_t *new_db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(new_db);
  
//   int64_t load_result = load_db(new_db, file_path);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, load_result);
  
//   // Verify loaded data
//   db_entry_t *entry1 = get_entry(new_db, "key1");
//   TEST_ASSERT_NOT_NULL(entry1);
//   TEST_ASSERT_EQUAL(INT32_TYPE, entry1->type);
//   TEST_ASSERT_EQUAL(42, *(int32_t*)entry1->value);
  
//   db_entry_t *entry2 = get_entry(new_db, "key2");
//   TEST_ASSERT_NOT_NULL(entry2);
//   TEST_ASSERT_EQUAL(FLOAT_TYPE, entry2->type);
//   TEST_ASSERT_EQUAL_FLOAT(3.14, *(float*)entry2->value);
  
//   db_entry_t *entry3 = get_entry(new_db, "key3");
//   TEST_ASSERT_NOT_NULL(entry3);
//   TEST_ASSERT_EQUAL(STR_TYPE, entry3->type);
//   TEST_ASSERT_EQUAL_STRING("hello", (char*)entry3->value);
  
//   free_db(db);
//   free_db(new_db);
//   remove(file_path);
// }

// // Test save_db and load_db with valid file path for hash storage
// static void test_save_load_db_valid_hash() {
//   logger(4, "*** test_save_load_db_valid_hash ***\n");
  
//   // Create and populate database
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_HASH);
//   TEST_ASSERT_NOT_NULL(db);
  
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key1", "42", INT32_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key2", "3.14", FLOAT_TYPE_STR));
//   TEST_ASSERT_GREATER_OR_EQUAL(0, put_entry(db, "key3", "hello", STR_TYPE_STR));
  
//   // Save database
//   uint8_t *file_path = "/tmp/test_db_hash.db";
//   int64_t save_result = save_db(db, file_path);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, save_result);
  
//   // Create new database and load
//   db_t *new_db = create_db(KV_STORAGE_STRUCTURE_HASH);
//   TEST_ASSERT_NOT_NULL(new_db);
  
//   int64_t load_result = load_db(new_db, file_path);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, load_result);
  
//   // Verify loaded data
//   db_entry_t *entry1 = get_entry(new_db, "key1");
//   TEST_ASSERT_NOT_NULL(entry1);
//   TEST_ASSERT_EQUAL(INT32_TYPE, entry1->type);
//   TEST_ASSERT_EQUAL(42, *(int32_t*)entry1->value);
  
//   db_entry_t *entry2 = get_entry(new_db, "key2");
//   TEST_ASSERT_NOT_NULL(entry2);
//   TEST_ASSERT_EQUAL(FLOAT_TYPE, entry2->type);
//   TEST_ASSERT_EQUAL_FLOAT(3.14, *(float*)entry2->value);
  
//   db_entry_t *entry3 = get_entry(new_db, "key3");
//   TEST_ASSERT_NOT_NULL(entry3);
//   TEST_ASSERT_EQUAL(STR_TYPE, entry3->type);
//   TEST_ASSERT_EQUAL_STRING("hello", (char*)entry3->value);
  
//   free_db(db);
//   free_db(new_db);
//   remove(file_path);
// }

// // Test save_db with NULL database
// static void test_save_db_null_db() {
//   logger(4, "*** test_save_db_null_db ***\n");
  
//   int64_t result = save_db(NULL, "/tmp/test.db");
//   TEST_ASSERT_EQUAL(-1, result);
// }

// // Test save_db with NULL file path
// static void test_save_db_null_path() {
//   logger(4, "*** test_save_db_null_path ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = save_db(db, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test save_db with empty file path
// static void test_save_db_empty_path() {
//   logger(4, "*** test_save_db_empty_path ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = save_db(db, "");
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test load_db with NULL database
// static void test_load_db_null_db() {
//   logger(4, "*** test_load_db_null_db ***\n");
  
//   int64_t result = load_db(NULL, "/tmp/test.db");
//   TEST_ASSERT_EQUAL(-1, result);
// }

// // Test load_db with NULL file path
// static void test_load_db_null_path() {
//   logger(4, "*** test_load_db_null_path ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = load_db(db, NULL);
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test load_db with empty file path
// static void test_load_db_empty_path() {
//   logger(4, "*** test_load_db_empty_path ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = load_db(db, "");
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test load_db with non-existent file
// static void test_load_db_nonexistent_file() {
//   logger(4, "*** test_load_db_nonexistent_file ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   int64_t result = load_db(db, "/tmp/nonexistent_file.db");
//   TEST_ASSERT_EQUAL(-1, result);
  
//   free_db(db);
// }

// // Test free_db with valid database
// static void test_free_db_valid() {
//   logger(4, "*** test_free_db_valid ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   // Should not crash
//   free_db(db);
// }

// // Test free_db with NULL database
// static void test_free_db_null() {
//   logger(4, "*** test_free_db_null ***\n");
  
//   // Should not crash
//   free_db(NULL);
// }

// // Test print_db with valid database
// static void test_print_db_valid() {
//   logger(4, "*** test_print_db_valid ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   put_entry(db, "test_key", "42", INT32_TYPE_STR);
  
//   // Should not crash
//   print_db(db);
  
//   free_db(db);
// }

// // Test print_db with NULL database
// static void test_print_db_null() {
//   logger(4, "*** test_print_db_null ***\n");
  
//   // Should not crash
//   print_db(NULL);
// }

// // Test database operations with multiple entries
// static void test_multiple_entries_operations() {
//   logger(4, "*** test_multiple_entries_operations ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   // Insert multiple entries
//   for (int i = 0; i < 10; i++) {
//     char key[32];
//     char value[32];
//     snprintf(key, sizeof(key), "key_%d", i);
//     snprintf(value, sizeof(value), "%d", i * 10);
    
//     int64_t result = put_entry(db, key, value, INT32_TYPE_STR);
//     TEST_ASSERT_GREATER_OR_EQUAL(0, result);
//   }
  
//   // Verify all entries exist
//   for (int i = 0; i < 10; i++) {
//     char key[32];
//     snprintf(key, sizeof(key), "key_%d", i);
    
//     db_entry_t *entry = get_entry(db, key);
//     TEST_ASSERT_NOT_NULL(entry);
//     TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
//     TEST_ASSERT_EQUAL(i * 10, *(int32_t*)entry->value);
//   }
  
//   // Delete some entries
//   for (int i = 0; i < 5; i++) {
//     char key[32];
//     snprintf(key, sizeof(key), "key_%d", i);
    
//     int64_t result = delete_entry(db, key);
//     TEST_ASSERT_GREATER_OR_EQUAL(0, result);
//   }
  
//   // Verify deleted entries are gone
//   for (int i = 0; i < 5; i++) {
//     char key[32];
//     snprintf(key, sizeof(key), "key_%d", i);
    
//     db_entry_t *entry = get_entry(db, key);
//     TEST_ASSERT_NULL(entry);
//   }
  
//   // Verify remaining entries still exist
//   for (int i = 5; i < 10; i++) {
//     char key[32];
//     snprintf(key, sizeof(key), "key_%d", i);
    
//     db_entry_t *entry = get_entry(db, key);
//     TEST_ASSERT_NOT_NULL(entry);
//     TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
//     TEST_ASSERT_EQUAL(i * 10, *(int32_t*)entry->value);
//   }
  
//   free_db(db);
// }

// // Test edge case with very long keys and values
// static void test_edge_case_long_strings() {
//   logger(4, "*** test_edge_case_long_strings ***\n");
  
//   db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
//   TEST_ASSERT_NOT_NULL(db);
  
//   // Test with maximum key length (should work)
//   char long_key[SM_BUFFER_SIZE];
//   memset(long_key, 'a', SM_BUFFER_SIZE - 2);
//   long_key[SM_BUFFER_SIZE - 2] = '\0';
  
//   char long_value[BG_BUFFER_SIZE];
//   memset(long_value, 'b', BG_BUFFER_SIZE - 2);
//   long_value[BG_BUFFER_SIZE - 2] = '\0';
  
//   int64_t result = put_entry(db, long_key, long_value, STR_TYPE_STR);
//   TEST_ASSERT_GREATER_OR_EQUAL(0, result);
  
//   db_entry_t *entry = get_entry(db, long_key);
//   TEST_ASSERT_NOT_NULL(entry);
//   TEST_ASSERT_EQUAL(STR_TYPE, entry->type);
//   TEST_ASSERT_EQUAL_STRING(long_value, (char*)entry->value);
  
//   free_db(db);
// }

int64_t main() {
  UNITY_BEGIN();
  
  // create_db tests
  RUN_TEST(test_create_db_valid_inputs);
  RUN_TEST(test_create_db_null_input);
  RUN_TEST(test_create_db_empty_input);
  RUN_TEST(test_create_db_invalid_storage_type);
  
  // insert_entry tests
  RUN_TEST(test_insert_entry_valid_list);
  RUN_TEST(test_insert_entry_valid_hash);
  RUN_TEST(test_insert_entry_null_db);
  RUN_TEST(test_insert_entry_null_entry);
  
  // put_entry tests
  // RUN_TEST(test_put_entry_valid_list);
  // RUN_TEST(test_put_entry_valid_hash);
  // RUN_TEST(test_put_entry_all_types);
  // RUN_TEST(test_put_entry_null_db);
  // RUN_TEST(test_put_entry_null_key);
  // RUN_TEST(test_put_entry_null_value);
  // RUN_TEST(test_put_entry_null_type);
  // RUN_TEST(test_put_entry_empty_strings);
  
  // // get_entry tests
  // RUN_TEST(test_get_entry_valid_list);
  // RUN_TEST(test_get_entry_valid_hash);
  // RUN_TEST(test_get_entry_nonexistent_key);
  // RUN_TEST(test_get_entry_null_db);
  // RUN_TEST(test_get_entry_null_key);
  // RUN_TEST(test_get_entry_empty_key);
  
  // // delete_entry tests
  // RUN_TEST(test_delete_entry_valid_list);
  // RUN_TEST(test_delete_entry_valid_hash);
  // RUN_TEST(test_delete_entry_nonexistent_key);
  // RUN_TEST(test_delete_entry_null_db);
  // RUN_TEST(test_delete_entry_null_key);
  // RUN_TEST(test_delete_entry_empty_key);
  
  // // save_db and load_db tests
  // RUN_TEST(test_save_load_db_valid_list);
  // RUN_TEST(test_save_load_db_valid_hash);
  // RUN_TEST(test_save_db_null_db);
  // RUN_TEST(test_save_db_null_path);
  // RUN_TEST(test_save_db_empty_path);
  // RUN_TEST(test_load_db_null_db);
  // RUN_TEST(test_load_db_null_path);
  // RUN_TEST(test_load_db_empty_path);
  // RUN_TEST(test_load_db_nonexistent_file);
  
  // // free_db and print_db tests
  // RUN_TEST(test_free_db_valid);
  // RUN_TEST(test_free_db_null);
  // RUN_TEST(test_print_db_valid);
  // RUN_TEST(test_print_db_null);
  
  // // Integration and edge case tests
  // RUN_TEST(test_multiple_entries_operations);
  // RUN_TEST(test_edge_case_long_strings);
  
  return UNITY_END();
}