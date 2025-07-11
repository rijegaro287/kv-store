#include "test_kv_parser.h"

// Test List:
// * test_create_entry_valid_inputs
// * test_create_entry_null_inputs
// * test_create_entry_invalid_type
// * test_set_entry_value_all_types
// * test_set_entry_value_null_inputs
// * test_set_entry_value_invalid_type
// * test_parse_line_valid_entry
// * test_parse_line_comment_line
// * test_parse_line_empty_line
// * test_parse_line_malformed_entry
// * test_parse_line_null_input
// * test_update_entry_valid
// * test_update_entry_null_inputs
// * test_update_entry_empty_type
// * test_parse_entry_valid
// * test_parse_entry_null_inputs
// * test_free_entry_valid
// * test_free_entry_null
// * test_print_entry_all_types
// * test_print_entry_null_input
// * test_print_entry_invalid_type

static void test_create_entry_valid_inputs() {
  logger(4, "*** test_create_entry_valid_inputs ***\n");

  db_entry_t *entry = create_entry("testkey", "42", "int32");
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL_STRING("testkey", entry->key);
  TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
  TEST_ASSERT_EQUAL(42, *(int32_t*)entry->value);
  free_entry(entry);
}

static void test_create_entry_null_inputs() {
  logger(4, "*** test_create_entry_null_inputs ***\n");

  TEST_ASSERT_NULL(create_entry(NULL, "42", "int32"));
  TEST_ASSERT_NULL(create_entry("key", NULL, "int32"));
  TEST_ASSERT_NULL(create_entry("key", "42", NULL));
}

static void test_create_entry_invalid_type() {
  logger(4, "*** test_create_entry_invalid_type ***\n");
  db_entry_t *entry = create_entry("key", "42", "invalid_type");
  TEST_ASSERT_NULL(entry);
}

static void test_set_entry_value_all_types() {
  logger(4, "*** test_set_entry_value_all_types ***\n");
  
  db_entry_t entry;

  // Test INT8
  entry.type = INT8_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "127"));
  TEST_ASSERT_EQUAL(127, *(int8_t*)entry.value);
  free(entry.value);
  
  // Test INT16
  entry.type = INT16_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "32767"));
  TEST_ASSERT_EQUAL(32767, *(int16_t*)entry.value);
  free(entry.value);
  
  // Test INT32
  entry.type = INT32_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "2147483647"));
  TEST_ASSERT_EQUAL(2147483647, *(int32_t*)entry.value);
  free(entry.value);
  
  // Test INT64
  entry.type = INT64_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "9223372036854775807"));
  TEST_ASSERT_EQUAL(9223372036854775807LL, *(int64_t*)entry.value);
  free(entry.value);
  
  // Test FLOAT
  entry.type = FLOAT_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "3.14"));
  TEST_ASSERT_EQUAL(3.14, *(float*)entry.value);
  free(entry.value);
  
  // Test DOUBLE
  entry.type = DOUBLE_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "3.141592653589793"));
  TEST_ASSERT_EQUAL(3.141592653589793, *(double*)entry.value);
  free(entry.value);
  
  // Test BOOL true
  entry.type = BOOL_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "true"));
  TEST_ASSERT_TRUE(*(bool*)entry.value);
  free(entry.value);
  
  // Test BOOL false
  entry.type = BOOL_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "false"));
  TEST_ASSERT_FALSE(*(bool*)entry.value);
  free(entry.value);
  
  // Test STRING
  entry.type = STR_TYPE;
  entry.value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(&entry, "hello world"));
  TEST_ASSERT_EQUAL_STRING("hello world", (char*)entry.value);
  free(entry.value);
}

static void test_set_entry_value_null_inputs() {
  db_entry_t entry;
  TEST_ASSERT_EQUAL(-1, set_entry_value(NULL, "42"));
  TEST_ASSERT_EQUAL(-1, set_entry_value(&entry, NULL));
}

// static void test_set_entry_value_invalid_type() {
//   db_entry_t entry;
//   entry.type = 999; // Invalid type
//   entry.value = NULL;
//   TEST_ASSERT_EQUAL(-1, set_entry_value(&entry, "42"));
// }

// static void test_parse_line_valid_entry() {
//   uint8_t line[] = "int32:testkey=42\n";
//   db_entry_t *entry = parse_line(line);
//   TEST_ASSERT_NOT_NULL(entry);
//   TEST_ASSERT_EQUAL_STRING("testkey", entry->key);
//   TEST_ASSERT_EQUAL(INT32_TYPE, entry->type);
//   TEST_ASSERT_EQUAL(42, *(int32_t*)entry->value);
//   free_entry(entry);
// }

// static void test_parse_line_comment_line() {
//   uint8_t line[] = "# This is a comment\n";
//   db_entry_t *entry = parse_line(line);
//   TEST_ASSERT_NULL(entry);
// }

// static void test_parse_line_empty_line() {
//   uint8_t line[] = "\n";
//   db_entry_t *entry = parse_line(line);
//   TEST_ASSERT_NULL(entry);
// }

// static void test_parse_line_malformed_entry() {
//   uint8_t line[] = "invalid_format\n";
//   db_entry_t *entry = parse_line(line);
//   TEST_ASSERT_NULL(entry);
// }

// static void test_parse_line_null_input() {
//   TEST_ASSERT_NULL(parse_line(NULL));
// }

// static void test_update_entry_valid() {
//   db_entry_t *entry = create_entry("key", "42", "int32");
//   TEST_ASSERT_NOT_NULL(entry);
  
//   TEST_ASSERT_EQUAL(0, update_entry(entry, "key", "100", "int32"));
//   TEST_ASSERT_EQUAL(100, *(int32_t*)entry->value);
  
//   free_entry(entry);
// }

// static void test_update_entry_null_inputs() {
//   db_entry_t entry;
//   TEST_ASSERT_EQUAL(-1, update_entry(NULL, "key", "value", "type"));
//   TEST_ASSERT_EQUAL(-1, update_entry(&entry, NULL, "value", "type"));
//   TEST_ASSERT_EQUAL(-1, update_entry(&entry, "key", NULL, "type"));
//   TEST_ASSERT_EQUAL(-1, update_entry(&entry, "key", "value", NULL));
// }

// static void test_update_entry_empty_type() {
//   db_entry_t *entry = create_entry("key", "42", "int32");
//   TEST_ASSERT_NOT_NULL(entry);
  
//   int64_t original_type = entry->type;
//   TEST_ASSERT_EQUAL(0, update_entry(entry, "key", "100", ""));
//   TEST_ASSERT_EQUAL(original_type, entry->type); // Type should remain unchanged
  
//   free_entry(entry);
// }

// static void test_parse_entry_valid() {
//   db_entry_t *entry = create_entry("testkey", "42", "int32");
//   uint8_t dest[128];
  
//   parse_entry(entry, dest, sizeof(dest));
//   TEST_ASSERT_EQUAL_STRING("int32:testkey=42\n", dest);
  
//   free_entry(entry);
// }

// static void test_parse_entry_null_inputs() {
//   db_entry_t entry;
//   uint8_t dest[128];
  
//   parse_entry(NULL, dest, sizeof(dest));
//   parse_entry(&entry, NULL, sizeof(dest));
//   // These should not crash (handled by logger)
// }

// static void test_free_entry_valid() {
//   db_entry_t *entry = create_entry("key", "42", "int32");
//   TEST_ASSERT_NOT_NULL(entry);
//   free_entry(entry); // Should not crash
// }

// static void test_free_entry_null() {
//   free_entry(NULL); // Should not crash
// }

// static void test_print_entry_all_types() {
//   // Test INT8
//   db_entry_t *entry = create_entry("key", "42", "int8");
//   print_entry(entry); // Should not crash
//   free_entry(entry);
  
//   // Test INT16
//   entry = create_entry("key", "42", "int16");
//   print_entry(entry);
//   free_entry(entry);
  
//   // Test INT32
//   entry = create_entry("key", "42", "int32");
//   print_entry(entry);
//   free_entry(entry);
  
//   // Test INT64
//   entry = create_entry("key", "42", "int64");
//   print_entry(entry);
//   free_entry(entry);
  
//   // Test BOOL
//   entry = create_entry("key", "true", "bool");
//   print_entry(entry);
//   free_entry(entry);
  
//   // Test FLOAT
//   entry = create_entry("key", "3.14", "float");
//   print_entry(entry);
//   free_entry(entry);
  
//   // Test DOUBLE
//   entry = create_entry("key", "3.14159", "double");
//   print_entry(entry);
//   free_entry(entry);
  
//   // Test STRING
//   entry = create_entry("key", "hello", "string");
//   print_entry(entry);
//   free_entry(entry);
// }

// static void test_print_entry_null_input() {
//   print_entry(NULL); // Should not crash
// }

// static void test_print_entry_invalid_type() {
//   db_entry_t entry;
//   entry.type = 999; // Invalid type
//   strcpy(entry.key, "testkey");
//   entry.value = malloc(sizeof(int));
//   *(int*)entry.value = 42;
  
//   print_entry(&entry); // Should handle invalid type gracefully
  
//   free(entry.value);
// }

extern void setUp(void) {
  // set stuff up here
}

extern void tearDown(void) {
  // clean stuff up here
}

extern int64_t test_kv_parser() {
  UNITY_BEGIN();
  
  RUN_TEST(test_create_entry_valid_inputs);
  RUN_TEST(test_create_entry_null_inputs);
  RUN_TEST(test_create_entry_invalid_type);
  RUN_TEST(test_set_entry_value_all_types);
  // RUN_TEST(test_set_entry_value_null_inputs);
  // RUN_TEST(test_set_entry_value_invalid_type);
  // RUN_TEST(test_parse_line_valid_entry);
  // RUN_TEST(test_parse_line_comment_line);
  // RUN_TEST(test_parse_line_empty_line);
  // RUN_TEST(test_parse_line_malformed_entry);
  // RUN_TEST(test_parse_line_null_input);
  // RUN_TEST(test_update_entry_valid);
  // RUN_TEST(test_update_entry_null_inputs);
  // RUN_TEST(test_update_entry_empty_type);
  // RUN_TEST(test_parse_entry_valid);
  // RUN_TEST(test_parse_entry_null_inputs);
  // RUN_TEST(test_free_entry_valid);
  // RUN_TEST(test_free_entry_null);
  // RUN_TEST(test_print_entry_all_types);
  // RUN_TEST(test_print_entry_null_input);
  // RUN_TEST(test_print_entry_invalid_type);
  
  return UNITY_END();
}
