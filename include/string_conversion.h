#pragma once

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include "logger.h"

/**
 * @file string_conversion.h
 * @brief String conversion utilities for type-safe value handling in the key-value store
 * 
 * This module provides functions to convert between string representations and typed values,
 * as well as type mapping utilities for the database entry system.
 */

#define INT8_TYPE_STR "int8"
#define INT16_TYPE_STR "int16"
#define INT32_TYPE_STR "int32"
#define INT64_TYPE_STR "int64"
#define FLOAT_TYPE_STR "float"
#define DOUBLE_TYPE_STR "double"
#define BOOL_TYPE_STR "bool"

/**
 * @brief Enumeration of supported value types in the database
 * 
 * These type identifiers are used internally to track the data type
 * of values stored in database entries.
 */
enum ENTRY_VALUE_TYPE {
  INT8_TYPE,
  INT16_TYPE,
  INT32_TYPE,
  INT64_TYPE,
  FLOAT_TYPE,
  DOUBLE_TYPE,
  BOOL_TYPE
};

/**
 * @brief Maps a type string to its corresponding enum value
 * 
 * Converts string representations like "int32", "float", "bool" to their
 * corresponding ENTRY_VALUE_TYPE enum values.
 * 
 * @param type String representation of the type (null-terminated)
 * @return int64_t The corresponding ENTRY_VALUE_TYPE enum value, or -1 if invalid
 * 
 * @note Supported type strings: "int8", "int16", "int32", "int64", "float", "double", "bool"
 * @see map_datatype_to_str()
 */
extern int64_t map_datatype_from_str(uint8_t *type);

/**
 * @brief Maps an enum type value to its string representation
 * 
 * Converts ENTRY_VALUE_TYPE enum values back to their string representations
 * for serialization and display purposes.
 * 
 * @param type ENTRY_VALUE_TYPE enum value
 * @param dest Buffer to store the resulting type string
 * @param max_len Maximum length of the destination buffer
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The destination buffer must be large enough to hold the type string
 * @see map_datatype_from_str()
 */
extern int64_t map_datatype_to_str(uint64_t type, uint8_t *dest, uint64_t max_len);

/**
 * @brief Converts a typed value to its string representation
 * 
 * Takes a void pointer to a typed value and converts it to a string representation
 * based on the specified type. Used for serialization and display.
 * 
 * @param type ENTRY_VALUE_TYPE enum indicating the type of the value
 * @param value Pointer to the typed value to convert
 * @param dest Buffer to store the resulting string
 * @param max_len Maximum length of the destination buffer
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The value pointer must point to data of the specified type
 * @note Different types are formatted with appropriate precision and format
 * @see str_to_int64(), str_to_float(), str_to_double(), str_to_bool()
 */
extern int64_t map_value_to_str(uint64_t type, void *value, uint8_t *dest, uint64_t max_len);

/**
 * @brief Converts a string to a 64-bit signed integer
 * 
 * Parses a string representation of an integer and stores the result
 * in the provided destination. Handles error checking and validation.
 * 
 * @param str_value String representation of the integer (null-terminated)
 * @param dest Pointer to store the converted integer value
 * @return int64_t 0 on success, -1 on failure (invalid format, overflow, etc.)
 * 
 * @note Uses standard library functions with proper error handling
 * @note Supports decimal integers with optional sign
 * @see str_to_float(), str_to_double(), str_to_bool()
 */
extern int64_t str_to_int64(uint8_t *str_value, int64_t *dest);

/**
 * @brief Converts a string to a single-precision floating point number
 * 
 * Parses a string representation of a float and stores the result
 * in the provided destination. Handles scientific notation and special values.
 * 
 * @param str_value String representation of the float (null-terminated)
 * @param dest Pointer to store the converted float value
 * @return int64_t 0 on success, -1 on failure (invalid format, overflow, etc.)
 * 
 * @note Supports decimal notation, scientific notation (e.g., "1.23e-4")
 * @note Handles special values like infinity and NaN
 * @see str_to_int64(), str_to_double(), str_to_bool()
 */
extern int64_t str_to_float(uint8_t *str_value, float *dest);

/**
 * @brief Converts a string to a double-precision floating point number
 * 
 * Parses a string representation of a double and stores the result
 * in the provided destination. Provides higher precision than str_to_float().
 * 
 * @param str_value String representation of the double (null-terminated)
 * @param dest Pointer to store the converted double value
 * @return int64_t 0 on success, -1 on failure (invalid format, overflow, etc.)
 * 
 * @note Supports decimal notation, scientific notation with extended precision
 * @note Handles special values like infinity and NaN
 * @see str_to_int64(), str_to_float(), str_to_bool()
 */
extern int64_t str_to_double(uint8_t *str_value, double *dest);

/**
 * @brief Converts a string to a boolean value
 * 
 * Parses string representations of boolean values and converts them
 * to C boolean type. Supports multiple string formats for true/false.
 * 
 * @param str_value String representation of the boolean (null-terminated)
 * @param dest Pointer to store the converted boolean value
 * @return int64_t 0 on success, -1 on failure (unrecognized format)
 * 
 * @note Typical supported formats: "true"/"false", "1"/"0", "yes"/"no"
 * @note Case sensitivity depends on implementation
 * @see str_to_int64(), str_to_float(), str_to_double()
 */
extern int64_t str_to_bool(uint8_t *str_value, bool *dest);
