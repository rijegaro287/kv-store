#pragma once

/**
 * @file constants.h
 * @brief Global constants and configuration values for the key-value store
 * 
 * This header defines all the constant values used throughout the key-value store
 * system, including buffer sizes, delimiters, and storage configuration parameters.
 */

#define BG_BUFFER_SIZE 128
#define SM_BUFFER_SIZE 32

#define KV_PARSER_TYPE_DELIMITER ":"
#define KV_PARSER_KEY_DELIMITER "="
#define KV_PARSER_VALUE_DELIMITER ";"

#define KV_STORAGE_STRUCTURE_LIST "L"
#define KV_STORAGE_STRUCTURE_HASH "H"

#define KV_STORAGE_HASH_SIZE 32
