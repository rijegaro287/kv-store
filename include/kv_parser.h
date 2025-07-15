#pragma once

#include "string_conversion.h"
#include "logger.h"
#include "constants.h"

/**
 * @file kv_parser.h
 * @brief Key-value parser for database entries with type-safe value handling
 * 
 * This module provides functionality to parse, create, and manipulate database
 * entries with strongly-typed values including integers, floats, booleans, and strings.
 */

/** @brief Delimiter used to separate type from key in serialized format */
#define TYPE_DELIMETER KV_PARSER_TYPE_DELIMITER
/** @brief Delimiter used to separate key from value in serialized format */
#define KEY_DELIMETER KV_PARSER_KEY_DELIMITER
/** @brief Delimiter used to terminate value in serialized format */
#define VALUE_DELIMETER KV_PARSER_VALUE_DELIMITER

/**
 * @brief Database entry structure representing a typed key-value pair
 * 
 * This structure stores a key-value pair where the value can be of various types
 * (integers, floats, booleans, etc.) and is dynamically allocated based on the type.
 */
typedef struct _db_entry_t {
  int64_t type;                    /**< Type identifier from ENTRY_VALUE_TYPE enum */
  uint8_t key[SM_BUFFER_SIZE];     /**< Key string (null-terminated) */
  void *value;                     /**< Pointer to dynamically allocated typed value */
} db_entry_t;

/**
 * @brief Sets an integer value in a database entry
 * 
 * @param dest Pointer to the database entry to modify
 * @param str_value String representation of the integer value
 * @param type_size Size of the target integer type (1, 2, 4, or 8 bytes)
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note This is a static/internal function for integer type conversion
 * @see set_entry_value()
 */
static int64_t set_integer_value(db_entry_t *dest, uint8_t *str_value, uint64_t type_size);

/**
 * @brief Sets a float value in a database entry
 * 
 * @param dest Pointer to the database entry to modify
 * @param str_value String representation of the float value
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note This is a static/internal function for float type conversion
 * @see set_entry_value()
 */
static int64_t set_float_value(db_entry_t *dest, uint8_t *str_value);

/**
 * @brief Sets a double value in a database entry
 * 
 * @param dest Pointer to the database entry to modify
 * @param str_value String representation of the double value
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note This is a static/internal function for double type conversion
 * @see set_entry_value()
 */
static int64_t set_double_value(db_entry_t *dest, uint8_t *str_value);

/**
 * @brief Sets a boolean value in a database entry
 * 
 * @param dest Pointer to the database entry to modify
 * @param str_value String representation of the boolean value ("true"/"false")
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note This is a static/internal function for boolean type conversion
 * @see set_entry_value()
 */
static int64_t set_bool_value(db_entry_t *dest, uint8_t *str_value);

/**
 * @brief Sets a string value in a database entry
 * 
 * @param dest Pointer to the database entry to modify
 * @param str_value String value to store
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note This is a static/internal function for string type conversion
 * @see set_entry_value()
 */
static int64_t set_string_value(db_entry_t *dest, uint8_t *str_value);

/**
 * @brief Sets the value of a database entry based on its type
 * 
 * This function converts the string representation to the appropriate typed value
 * and stores it in the entry. The conversion is performed based on the entry's
 * type field which must be set before calling this function.
 * 
 * @param dest Pointer to the database entry (type field must be set)
 * @param str_value String representation of the value to set
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The entry's type field must be properly initialized before calling
 * @note Any existing value will be freed and replaced
 * @see create_entry(), update_entry()
 */
extern int64_t set_entry_value(db_entry_t *dest, uint8_t *str_value);

/**
 * @brief Updates an existing database entry with new value and optionally new type
 * 
 * This function allows updating both the value and type of an existing entry.
 * If the type parameter is empty, the current type is preserved.
 * 
 * @param entry Pointer to the database entry to update
 * @param value New string representation of the value
 * @param type New type string (can be empty to preserve current type)
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note If type conversion fails, the entry remains unchanged
 * @see set_entry_value(), create_entry()
 */
extern int64_t update_entry(db_entry_t *entry, uint8_t* value, uint8_t* type);

/**
 * @brief Creates a new database entry with the specified key, value, and type
 * 
 * Allocates and initializes a new database entry with the given parameters.
 * The value string is converted to the appropriate type based on the type parameter.
 * 
 * @param key Key string for the entry (must be non-empty)
 * @param value String representation of the value
 * @param type Type identifier string (e.g., "int32", "float", "bool")
 * @return db_entry_t* Pointer to the newly created entry, or NULL on failure
 * 
 * @note The caller is responsible for freeing the returned entry using free_entry()
 * @note All parameters must be non-NULL and non-empty strings
 * @see free_entry(), parse_line()
 */
extern db_entry_t* create_entry(uint8_t *key, uint8_t *value, uint8_t *type);

/**
 * @brief Parses a text line into a database entry
 * 
 * Parses a line with the format "type:key=value;" into a database entry.
 * Lines starting with '#' are treated as comments and ignored.
 * Empty lines or lines containing only whitespace are also ignored.
 * 
 * @param line Input line to parse (will be modified by strtok)
 * @return db_entry_t* Pointer to the parsed entry, or NULL if parsing fails or line is ignored
 * 
 * @note The input line is modified during parsing (uses strtok)
 * @note The caller is responsible for freeing the returned entry using free_entry()
 * @see create_entry(), parse_entry()
 */
extern db_entry_t* parse_line(uint8_t *line);

/**
 * @brief Serializes a database entry into a text format
 * 
 * Converts a database entry back into the text format "type:key=value;"
 * for storage in files or transmission.
 * 
 * @param entry Pointer to the database entry to serialize
 * @param dest Buffer to store the serialized string
 * @param max_len Maximum length of the destination buffer
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The destination buffer must be large enough to hold the serialized entry
 * @note The resulting string includes a newline character at the end
 * @see parse_line(), print_entry()
 */
extern int64_t parse_entry(db_entry_t *entry, uint8_t *dest, uint64_t max_len);

/**
 * @brief Frees all memory associated with a database entry
 * 
 * Properly deallocates both the entry structure and its dynamically allocated value.
 * This function should be called for every entry created with create_entry() or parse_line().
 * 
 * @param entry Pointer to the database entry to free (can be NULL)
 * 
 * @note Safe to call with NULL pointer
 * @note After calling this function, the entry pointer should not be used
 * @see create_entry(), parse_line()
 */
extern void free_entry(db_entry_t *entry);

/**
 * @brief Prints a database entry to stdout in a formatted manner
 * 
 * Displays the entry's type, key, and value in a tab-separated format.
 * The value is formatted according to its type for optimal readability.
 * 
 * @param entry Pointer to the database entry to print
 * 
 * @note Output format: "type\tkey\tvalue\n"
 * @note Different value types are formatted with appropriate precision
 * @see parse_entry()
 */
extern void print_entry(db_entry_t *entry);
