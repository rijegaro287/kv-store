#include "test_kv_parser.h"

static db_entry_t* helper_create_and_validate_entry(uint8_t *key, uint8_t *value, uint8_t *type_str) {
  db_entry_t *entry = create_entry(key, value, type_str);
  TEST_ASSERT_NOT_NULL(entry);
  return entry;
}

static void helper_test_create_entry_null_inputs() {
  TEST_ASSERT_NULL(create_entry(NULL, "42", INT32_TYPE_STR));
  TEST_ASSERT_NULL(create_entry("key", NULL, INT32_TYPE_STR));
  TEST_ASSERT_NULL(create_entry("key", "42", NULL));
}

static void helper_test_create_entry_invalid_inputs() {
  TEST_ASSERT_NULL(create_entry("key", "42", "invalid_type"));
  TEST_ASSERT_NULL(create_entry("key", "invalid_value", INT8_TYPE_STR));
  TEST_ASSERT_NULL(create_entry("", "42", INT8_TYPE_STR));
  TEST_ASSERT_NULL(create_entry("key", "", INT8_TYPE_STR));
  TEST_ASSERT_NULL(create_entry("key", "42", ""));
}

static void helper_test_set_entry_value_all_types(db_entry_t *entry) {
  entry->type = INT8_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "127"));
  TEST_ASSERT_EQUAL(127, *(int8_t*)entry->value);
  free(entry->value);
  
  entry->type = INT16_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "32767"));
  TEST_ASSERT_EQUAL(32767, *(int16_t*)entry->value);
  free(entry->value);
  
  entry->type = INT32_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "2147483647"));
  TEST_ASSERT_EQUAL(2147483647, *(int32_t*)entry->value);
  free(entry->value);
  
  entry->type = INT64_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "9223372036854775807"));
  TEST_ASSERT_EQUAL(9223372036854775807LL, *(int64_t*)entry->value);
  free(entry->value);
  
  entry->type = FLOAT_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "3.14"));
  TEST_ASSERT_EQUAL(3.14, *(float*)entry->value);
  free(entry->value);
  
  entry->type = DOUBLE_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "3.141592653589793"));
  TEST_ASSERT_EQUAL(3.141592653589793, *(double*)entry->value);
  free(entry->value);
  
  entry->type = BOOL_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "true"));
  TEST_ASSERT_TRUE(*(bool*)entry->value);
  free(entry->value);
  
  entry->type = BOOL_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "false"));
  TEST_ASSERT_FALSE(*(bool*)entry->value);
  free(entry->value);
  
  entry->type = STR_TYPE;
  entry->value = NULL;
  TEST_ASSERT_EQUAL(0, set_entry_value(entry, "hello world"));
  TEST_ASSERT_EQUAL_STRING("hello world", (char*)entry->value);
}

static void helper_test_update_entry_all_types(db_entry_t *entry) {
  TEST_ASSERT_EQUAL_INT8(0, update_entry(entry, "121", INT8_TYPE_STR));
  TEST_ASSERT_EQUAL_INT8(121, *(int8_t*)entry->value);

  TEST_ASSERT_EQUAL_INT16(0, update_entry(entry, "1222", INT16_TYPE_STR));
  TEST_ASSERT_EQUAL_INT16(1222, *(int16_t*)entry->value);

  TEST_ASSERT_EQUAL_INT32(0, update_entry(entry, "57899", INT32_TYPE_STR));
  TEST_ASSERT_EQUAL_INT32(57899, *(int32_t*)entry->value);

  TEST_ASSERT_EQUAL_INT64(0, update_entry(entry, "8947381", INT64_TYPE_STR));
  TEST_ASSERT_EQUAL_INT64(8947381, *(int64_t*)entry->value);

  TEST_ASSERT_EQUAL_FLOAT(0, update_entry(entry, "124.21312", FLOAT_TYPE_STR));
  TEST_ASSERT_EQUAL_FLOAT(124.21312, *(float*)entry->value);

  TEST_ASSERT_EQUAL_DOUBLE(0, update_entry(entry, "1200.1202310", DOUBLE_TYPE_STR));
  TEST_ASSERT_EQUAL_DOUBLE(1200.1202310, *(double*)entry->value);

  TEST_ASSERT_EQUAL(0, update_entry(entry, "true", BOOL_TYPE_STR));
  TEST_ASSERT_EQUAL(1, *(bool*)entry->value);

  TEST_ASSERT_EQUAL(0, update_entry(entry, "false", BOOL_TYPE_STR));
  TEST_ASSERT_EQUAL(0, *(bool*)entry->value);

  TEST_ASSERT_EQUAL(0, update_entry(entry, "Buenos dias\n", STR_TYPE_STR));
  TEST_ASSERT_EQUAL_STRING("Buenos dias\n", (int8_t*)entry->value);
}

static void helper_test_parse_entry_all_types(db_entry_t *entry, uint8_t *line) {
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(INT8_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "42" VALUE_DELIMETER "\n", line);

  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "5889", INT16_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(INT16_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "5889" VALUE_DELIMETER "\n", line);

  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "25763", INT32_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(INT32_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "25763" VALUE_DELIMETER "\n", line);

  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "123812929", INT64_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(INT64_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "123812929" VALUE_DELIMETER "\n", line);

  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "1.1234567", FLOAT_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(FLOAT_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "1.1234567" VALUE_DELIMETER "\n", line);

  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "5.123456789123456", DOUBLE_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(DOUBLE_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "5.123456789123456" VALUE_DELIMETER "\n", line);

  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "true", BOOL_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(BOOL_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "true" VALUE_DELIMETER "\n", line);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "false", BOOL_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(BOOL_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "false" VALUE_DELIMETER "\n", line);
  
  TEST_ASSERT_GREATER_OR_EQUAL(0, update_entry(entry, "String 1", STR_TYPE_STR));
  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(STR_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "String 1" VALUE_DELIMETER "\n", line);
}

static void helper_test_parse_line_single_type(uint8_t *type_str, uint8_t *value, 
                                               int64_t  expected_type, void* expected_value, 
                                               uint8_t *assert_type) {
  uint8_t line[BG_BUFFER_SIZE];
  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s%s%s", 
           type_str, TYPE_DELIMETER,
           "testkey", KEY_DELIMETER, 
           value, VALUE_DELIMETER);
  
  db_entry_t *entry = parse_line(line);
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL_STRING("testkey", entry->key);
  TEST_ASSERT_EQUAL(expected_type, entry->type);
  
  if (strcmp(assert_type, "int8") == 0) {
    TEST_ASSERT_EQUAL(*(int8_t*)expected_value, *(int8_t*)entry->value);
  } else if (strcmp(assert_type, "int16") == 0) {
    TEST_ASSERT_EQUAL(*(int16_t*)expected_value, *(int16_t*)entry->value);
  } else if (strcmp(assert_type, "int32") == 0) {
    TEST_ASSERT_EQUAL(*(int32_t*)expected_value, *(int32_t*)entry->value);
  } else if (strcmp(assert_type, "int64") == 0) {
    TEST_ASSERT_EQUAL(*(int64_t*)expected_value, *(int64_t*)entry->value);
  } else if (strcmp(assert_type, "float") == 0) {
    TEST_ASSERT_EQUAL(*(float*)expected_value, *(float*)entry->value);
  } else if (strcmp(assert_type, "double") == 0) {
    TEST_ASSERT_EQUAL(*(double*)expected_value, *(double*)entry->value);
  } else if (strcmp(assert_type, "bool") == 0) {
    TEST_ASSERT_EQUAL(*(bool*)expected_value, *(bool*)entry->value);
  } else if (strcmp(assert_type, "string") == 0) {
    TEST_ASSERT_EQUAL_STRING((char*)expected_value, (uint8_t*)entry->value);
  }
  
  free_entry(entry);
}

static void test_create_entry_valid_inputs() {
  logger(4, "*** test_create_entry_valid_inputs ***\n");
  db_entry_t *entry_1 = helper_create_and_validate_entry("testkey_1", "42", INT32_TYPE_STR);
  TEST_ASSERT_EQUAL_STRING("testkey_1", entry_1->key);
  TEST_ASSERT_EQUAL(INT32_TYPE, entry_1->type);
  TEST_ASSERT_EQUAL(42, *(int32_t*)entry_1->value);

  db_entry_t *entry_2 = helper_create_and_validate_entry("testkey_2", "15.231231231", DOUBLE_TYPE_STR);
  TEST_ASSERT_EQUAL_STRING("testkey_2", entry_2->key);
  TEST_ASSERT_EQUAL(DOUBLE_TYPE, entry_2->type);
  TEST_ASSERT_EQUAL(15.231231231, *(double*)entry_2->value);
  
  free_entry(entry_1);
  free_entry(entry_2);
}

static void test_create_entry_null_inputs() {
  logger(4, "*** test_create_entry_null_inputs ***\n");
  helper_test_create_entry_null_inputs();
}

static void test_create_entry_invalid_inputs() {
  logger(4, "*** test_create_entry_invalid_inputs ***\n");
  helper_test_create_entry_invalid_inputs();
}

static void test_set_entry_value_all_types() {
  logger(4, "*** test_set_entry_value_all_types ***\n");
  db_entry_t *entry = malloc(sizeof(db_entry_t));

  helper_test_set_entry_value_all_types(entry);

  free_entry(entry);
}

static void test_set_entry_value_null_inputs() {
  logger(4, "*** test_set_entry_value_null_inputs ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "10", INT8_TYPE_STR);
  TEST_ASSERT_EQUAL(-1, set_entry_value(NULL, "42"));
  TEST_ASSERT_EQUAL(-1, set_entry_value(entry, NULL));
  TEST_ASSERT_NOT_NULL(entry->value);
  TEST_ASSERT_EQUAL_INT8(10, *(int8_t*)entry->value);
  free_entry(entry);
}

static void test_set_entry_value_invalid_inputs() {
  logger(4, "*** test_set_entry_value_invalid_inputs ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "10", INT8_TYPE_STR);
  TEST_ASSERT_EQUAL(-1, set_entry_value(entry, ""));
  TEST_ASSERT_EQUAL(-1, set_entry_value(entry, "string"));
  entry->type = 999;
  TEST_ASSERT_EQUAL(-1, set_entry_value(entry, "42"));
  TEST_ASSERT_NOT_NULL(entry->value);
  TEST_ASSERT_EQUAL_INT8(10, *(int8_t*)entry->value);
  free_entry(entry);
}

static void test_update_entry_valid() {
  logger(4, "*** test_update_entry_valid ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "42", INT32_TYPE_STR);
  TEST_ASSERT_EQUAL(0, update_entry(entry, "100", INT32_TYPE_STR));
  TEST_ASSERT_EQUAL_INT32(100, *(int32_t*)entry->value);
  free_entry(entry);
}

static void test_update_entry_all_types() {
  logger(4, "*** test_update_entry_all_types ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "42", INT8_TYPE_STR);

  helper_test_update_entry_all_types(entry);

  free_entry(entry);
}

static void test_update_entry_null_inputs() {
  logger(4, "*** test_update_entry_null_inputs ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "10", INT8_TYPE_STR);
  TEST_ASSERT_EQUAL(-1, update_entry(NULL, "value", "type"));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, NULL, "type"));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "value", NULL));
  TEST_ASSERT_NOT_NULL((int8_t*)entry->value);
  TEST_ASSERT_EQUAL_INT8(10, *(int8_t*)entry->value);
  free_entry(entry);
}

static void test_update_entry_invalid_inputs() {
  logger(4, "*** test_update_entry_invalid_inputs ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "10", INT8_TYPE_STR);
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "value", "type"));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "value", INT8_TYPE_STR));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "12", "type"));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "", ""));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "12", ""));
  TEST_ASSERT_EQUAL(-1, update_entry(entry, "", INT8_TYPE_STR));
  TEST_ASSERT_NOT_NULL((int8_t*)entry->value);
  TEST_ASSERT_EQUAL_INT8(10, *(int8_t*)entry->value);
  free_entry(entry);
}

static void test_parse_entry_valid() {
  logger(4, "*** test_parse_entry_valid ***\n");
  uint8_t line[BG_BUFFER_SIZE];
  db_entry_t *entry = helper_create_and_validate_entry("testkey", "42", INT32_TYPE_STR);

  TEST_ASSERT_GREATER_OR_EQUAL(0, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING(INT32_TYPE_STR TYPE_DELIMETER
                           "testkey" KEY_DELIMETER
                           "42" VALUE_DELIMETER "\n", line);
  free_entry(entry);
}

static void test_parse_entry_all_types() {
  logger(4, "*** test_parse_entry_all_types ***\n");
  uint8_t line[BG_BUFFER_SIZE];
  memset(line, '\0', BG_BUFFER_SIZE);

  db_entry_t *entry = helper_create_and_validate_entry("testkey", "42", INT8_TYPE_STR);
  helper_test_parse_entry_all_types(entry, line);

  free_entry(entry);
}

static void test_parse_entry_null_inputs() {
  logger(4, "*** test_parse_entry_null_inputs ***\n");
  uint8_t line[BG_BUFFER_SIZE];
  memset(line, '\0', BG_BUFFER_SIZE);

  db_entry_t *entry = helper_create_and_validate_entry("testkey", "42", INT32_TYPE_STR);

  TEST_ASSERT_GREATER_OR_EQUAL(-1, parse_entry(NULL, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("", line);
  TEST_ASSERT_GREATER_OR_EQUAL(-1, parse_entry(entry, NULL, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("", line);

  free_entry(entry);
}

static void test_parse_entry_invalid_inputs() {
  logger(4, "*** test_parse_entry_invalid_inputs ***\n");
  uint8_t line[BG_BUFFER_SIZE];
  memset(line, '\0', BG_BUFFER_SIZE);

  db_entry_t *entry = helper_create_and_validate_entry("testkey", "42", INT32_TYPE_STR);

  TEST_ASSERT_GREATER_OR_EQUAL(-1, parse_entry(entry, line, 0));
  TEST_ASSERT_EQUAL_STRING("", line);
  TEST_ASSERT_EQUAL_STRING("", line);
  
  entry->type = 999;
  TEST_ASSERT_GREATER_OR_EQUAL(-1, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("", line);
  
  free(entry->value);
  entry->type = INT8_TYPE;
  entry->value = NULL;
  TEST_ASSERT_GREATER_OR_EQUAL(-1, parse_entry(entry, line, BG_BUFFER_SIZE));
  TEST_ASSERT_EQUAL_STRING("", line);

  free_entry(entry);
}

static void test_parse_line_valid_entry() {
  logger(4, "*** test_parse_line_valid_entry ***\n");
  uint8_t line[BG_BUFFER_SIZE] = INT8_TYPE_STR TYPE_DELIMETER
                                 "testkey" KEY_DELIMETER
                                 "42" VALUE_DELIMETER;
  db_entry_t *entry = parse_line(line);
  TEST_ASSERT_NOT_NULL(entry);
  TEST_ASSERT_EQUAL_STRING("testkey", entry->key);
  TEST_ASSERT_EQUAL(INT8_TYPE, entry->type);
  TEST_ASSERT_EQUAL(42, *(int8_t*)entry->value);
  free_entry(entry);
}

static void test_parse_line_all_types() {
  logger(4, "*** test_parse_line_all_types ***\n");
  
  int8_t val_int8 = 42;
  helper_test_parse_line_single_type(INT8_TYPE_STR, "42", INT8_TYPE, &val_int8, "int8");
  
  int16_t val_int16 = 2182;
  helper_test_parse_line_single_type(INT16_TYPE_STR, "2182", INT16_TYPE, &val_int16, "int16");
  
  int32_t val_int32 = 82918;
  helper_test_parse_line_single_type(INT32_TYPE_STR, "82918", INT32_TYPE, &val_int32, "int32");
  
  int64_t val_int64 = 7827281;
  helper_test_parse_line_single_type(INT64_TYPE_STR, "7827281", INT64_TYPE, &val_int64, "int64");
  
  float val_float = 1.213432f;
  helper_test_parse_line_single_type(FLOAT_TYPE_STR, "1.213432", FLOAT_TYPE, &val_float, "float");
  
  double val_double = 3.123456789123456;
  helper_test_parse_line_single_type(DOUBLE_TYPE_STR, "3.123456789123456", DOUBLE_TYPE, &val_double, "double");
  
  bool val_bool_true = true;
  helper_test_parse_line_single_type(BOOL_TYPE_STR, "true", BOOL_TYPE, &val_bool_true, "bool");
  
  bool val_bool_false = false;
  helper_test_parse_line_single_type(BOOL_TYPE_STR, "false", BOOL_TYPE, &val_bool_false, "bool");
  
  char val_string[] = "Hello World";
  helper_test_parse_line_single_type(STR_TYPE_STR, "Hello World", STR_TYPE, val_string, "string");
}

static void test_parse_line_comment_line() {
  logger(4, "*** test_parse_line_comment_line ***\n");
  uint8_t line[] = "# This is a comment\n";
  db_entry_t *entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
}

static void test_parse_line_empty_line() {
  logger(4, "*** test_parse_line_empty_line ***\n");
  uint8_t line[] = "\n";
  db_entry_t *entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
}

static void test_parse_line_malformed_entry() {
  logger(4, "*** test_parse_line_malformed_entry ***\n");
  db_entry_t *entry;
  uint8_t line[BG_BUFFER_SIZE];
  
  snprintf(line, BG_BUFFER_SIZE, "invalid_format\n");
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s", 
           INT8_TYPE_STR, TYPE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s", 
           INT8_TYPE_STR, TYPE_DELIMETER,
           "testkey", KEY_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s", 
           INT8_TYPE_STR, TYPE_DELIMETER, 
           "42", VALUE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s", 
           "testkey", KEY_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s", 
           "testkey", KEY_DELIMETER, 
           "42", VALUE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s", 
           "42", VALUE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s", 
           "testkey", KEY_DELIMETER, 
           "42", VALUE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s", 
           INT8_TYPE_STR, "*",
           "testkey", KEY_DELIMETER, 
           "42", VALUE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);

  snprintf(line, BG_BUFFER_SIZE, "%s%s%s%s%s%s", 
           INT8_TYPE_STR, TYPE_DELIMETER,
           "testkey", ";", 
           "42", VALUE_DELIMETER);
  entry = parse_line(line);
  TEST_ASSERT_NULL(entry);
  free_entry(entry);
}

static void test_parse_line_null_input() {
  logger(4, "*** test_parse_line_null_input ***\n");
  TEST_ASSERT_NULL(parse_line(NULL));
}

static void test_free_entry_valid() {
  logger(4, "*** test_free_entry_valid ***\n");
  db_entry_t *entry = helper_create_and_validate_entry("key", "42", INT32_TYPE_STR);
  free_entry(entry);
}

static void test_free_entry_null() {
  logger(4, "*** test_free_entry_null ***\n");
  free_entry(NULL);
}

static void helper_test_print_entry_all_types() {
  db_entry_t *entry;
  
  entry = helper_create_and_validate_entry("key", "42", INT8_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "42", INT16_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "42", INT32_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "42", INT64_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "true", BOOL_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "3.14", FLOAT_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "3.14159", DOUBLE_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
  
  entry = helper_create_and_validate_entry("key", "hello", STR_TYPE_STR);
  print_entry(entry);
  free_entry(entry);
}

static void test_print_entry_all_types() {
  logger(4, "*** test_print_entry_all_types ***\n");
  helper_test_print_entry_all_types();
}

static void test_print_entry_null_input() {
  logger(4, "*** test_print_entry_null_input ***\n");
  print_entry(NULL);
}

static void test_print_entry_invalid_type() {
  logger(4, "*** test_print_entry_invalid_type ***\n");
  db_entry_t entry;
  entry.type = 999;
  strcpy(entry.key, "testkey");
  entry.value = malloc(sizeof(int64_t ));
  *(int64_t *)entry.value = 42;
  
  print_entry(&entry);
  
  free(entry.value);
}

extern void setUp(void) {
  // set stuff up here
}

extern void tearDown(void) {
  // clean stuff up here
}

extern int main() {
  UNITY_BEGIN();
  
  // create_entry
  RUN_TEST(test_create_entry_valid_inputs);
  RUN_TEST(test_create_entry_null_inputs);
  RUN_TEST(test_create_entry_invalid_inputs);
  
  // set_entry_value
  RUN_TEST(test_set_entry_value_all_types);
  RUN_TEST(test_set_entry_value_null_inputs);
  RUN_TEST(test_set_entry_value_invalid_inputs);
  
  // update_entry
  RUN_TEST(test_update_entry_valid);
  RUN_TEST(test_update_entry_all_types);
  RUN_TEST(test_update_entry_null_inputs);
  RUN_TEST(test_update_entry_invalid_inputs);

  // parse_entry
  RUN_TEST(test_parse_entry_valid);
  RUN_TEST(test_parse_entry_all_types);
  RUN_TEST(test_parse_entry_null_inputs);
  RUN_TEST(test_parse_entry_invalid_inputs);

  // parse_line
  RUN_TEST(test_parse_line_valid_entry);
  RUN_TEST(test_parse_line_all_types);
  RUN_TEST(test_parse_line_comment_line);
  RUN_TEST(test_parse_line_empty_line);
  RUN_TEST(test_parse_line_malformed_entry);
  RUN_TEST(test_parse_line_null_input);

  // free_entry
  RUN_TEST(test_free_entry_valid);
  RUN_TEST(test_free_entry_null);

  // print_entry
  RUN_TEST(test_print_entry_all_types);
  RUN_TEST(test_print_entry_null_input);
  RUN_TEST(test_print_entry_invalid_type);
  
  return UNITY_END();
}
