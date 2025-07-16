/**
 * @file hash_table.h
 * @brief Hash table implementation for key-value store storage backend
 * 
 * This module provides a hash table data structure for storing database entries.
 * It serves as one of the storage backends for the key-value database, offering
 * fast O(1) average-case access times through hash-based indexing with chaining
 * for collision resolution.
 */
#pragma once

#include "linked_list.h"


/**
 * @brief Hash table structure for storing database entries
 * 
 * This structure uses separate chaining for collision resolution, where each
 * bucket contains a linked list of entries that hash to the same index.
 */
typedef struct _hash_table_t {
  list_t **content;         /**< Array of pointers to linked lists (buckets) */
  uint64_t size;           /**< Number of buckets in the hash table */
} hash_table_t;

/**
 * @brief Calculates hash code for a given key
 * 
 * Computes a hash value for the specified key string using a hash function
 * suitable for distribution across the hash table buckets.
 * 
 * @param key Key string to hash (null-terminated)
 * @param size Number of buckets in the hash table
 * @return int64_t Hash code (bucket index) for the key, or -1 on error
 * 
 * @note This is a static/internal function for hash computation
 * @note The returned value is in the range [0, size-1]
 */
static int64_t calculate_hash_code(uint8_t *key, uint64_t size);

/**
 * @brief Creates a new hash table with the specified number of buckets
 * 
 * Allocates and initializes a new hash table structure with the given size.
 * Each bucket is initialized as an empty linked list for collision handling.
 * 
 * @param size Number of buckets to create in the hash table
 * @return hash_table_t* Pointer to the newly created hash table, or NULL on failure
 * 
 * @note The caller is responsible for freeing the hash table using free_hash_table()
 * @note Larger sizes generally provide better performance but use more memory
 * @see free_hash_table()
 */
extern hash_table_t* create_hash_table(uint64_t size);

/**
 * @brief Inserts a database entry into the hash table
 * 
 * Adds the given entry to the appropriate bucket based on its key's hash value.
 * If an entry with the same key already exists, it will be updated with the
 * new entry's value and type.
 * 
 * @param hash Pointer to the hash table
 * @param entry Pointer to the database entry to insert
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The hash table takes ownership of the entry pointer
 * @note Duplicate keys result in value updates, not multiple entries
 * @note Average time complexity: O(1), worst case: O(n) with many collisions
 * @see hash_put(), hash_delete()
 */
extern int64_t hash_insert(hash_table_t *hash, db_entry_t *entry);

/**
 * @brief Creates and inserts a new entry with the given key, value, and type
 * 
 * This is a convenience function that creates a new entry from the provided
 * parameters and inserts it into the hash table. Handles key collision by
 * updating existing entries.
 * 
 * @param hash Pointer to the hash table
 * @param key Key for the entry (null-terminated string)
 * @param value Value for the entry (null-terminated string)
 * @param type Type identifier for the value (e.g., "int32", "float", "bool")
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note All parameters must be non-NULL and non-empty strings
 * @note Average time complexity: O(1)
 * @see hash_insert(), hash_get_entry()
 */
extern int64_t hash_put(hash_table_t *hash, uint8_t *key, uint8_t* value, uint8_t* type);

/**
 * @brief Deletes an entry from the hash table by key
 * 
 * Searches for and removes the entry with the specified key from the appropriate
 * bucket. Updates the bucket's linked list and properly deallocates the entry.
 * 
 * @param hash Pointer to the hash table
 * @param key Key of the entry to delete (null-terminated string)
 * @return int64_t 0 on success, -1 on failure (including if key not found)
 * 
 * @note The key parameter must be non-NULL and non-empty
 * @note Average time complexity: O(1)
 * @see hash_get_entry(), hash_insert()
 */
extern int64_t hash_delete(hash_table_t *hash, uint8_t *key);

/**
 * @brief Retrieves an entry from the hash table by key
 * 
 * Searches the appropriate bucket for an entry with the specified key and
 * returns it. Uses hash-based lookup for efficient access.
 * 
 * @param hash Pointer to the hash table
 * @param key Key of the entry to retrieve (null-terminated string)
 * @return db_entry_t* Pointer to the found entry, or NULL if not found
 * 
 * @note The returned pointer points to the actual entry, not a copy
 * @note Average time complexity: O(1)
 * @see hash_put(), hash_delete()
 */
extern db_entry_t *hash_get_entry(hash_table_t *hash, uint8_t *key);

/**
 * @brief Saves all entries in the hash table to a file
 * 
 * Iterates through all buckets and writes each entry to the specified file
 * in the serialized format "type:key=value;". Used for database persistence.
 * 
 * @param file Open file pointer for writing
 * @param hash Pointer to the hash table to save
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The file must be opened for writing before calling this function
 * @note Each entry is written on a separate line
 * @note The order of entries in the file is not guaranteed due to hashing
 * @see hash_insert(), parse_line()
 */
extern int64_t hash_save(FILE *file, hash_table_t *hash);

/**
 * @brief Frees all memory associated with the hash table
 * 
 * Properly deallocates the entire hash table structure including all buckets
 * and their contained database entries. Should be called when the hash table
 * is no longer needed.
 * 
 * @param hash Pointer to the hash table to free (can be NULL)
 * 
 * @note Safe to call with NULL pointer
 * @note After calling this function, the hash table pointer should not be used
 * @see create_hash_table(), free_list()
 */
extern void free_hash_table(hash_table_t *hash);

/**
 * @brief Prints all entries in the hash table to stdout
 * 
 * Iterates through all buckets and displays each entry in a formatted manner.
 * Useful for debugging and inspection purposes.
 * 
 * @param hash Pointer to the hash table to print
 * 
 * @note Output format depends on the print_entry() function implementation
 * @note Entries are printed in hash table order, not insertion order
 * @see print_entry(), list_print()
 */
extern void hash_print(hash_table_t *hash);
