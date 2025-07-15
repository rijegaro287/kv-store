#pragma once

#include "kv_parser.h"
#include "linked_list.h"
#include "hash_table.h"

/** 
 * @file kv_controller.h
 * @brief Controller for managing key-value database operations
 * 
 * This header provides functions to create, manipulate, and manage a key-value
 * database using either a linked list or a hash table for storage.
 */

/**
 * @brief Database structure representing a key-value store
 * 
 * This structure contains the storage type and a pointer to the underlying
 * storage implementation (either linked list or hash table).
 */
typedef struct _db_t {
  uint8_t storage_type[SM_BUFFER_SIZE]; /**< Storage type identifier ("L" for list, "H" for hash) */
  void *storage;                        /**< Pointer to the underlying storage structure */
} db_t;

/**
 * @brief Creates a new database instance with the specified storage type
 * 
 * @param storage_type Storage type identifier ("L" for linked list, "H" for hash table)
 * @return db_t* Pointer to the newly created database, or NULL on failure
 * 
 * @note The caller is responsible for freeing the returned database using free_db()
 * @see free_db()
 */
extern db_t* create_db(uint8_t *storage_type);

/**
 * @brief Loads database entries from a file
 * 
 * Reads key-value pairs from the specified file and populates the database.
 * 
 * @param db Pointer to the database to load data into
 * @param file_path Path to the file containing the database data
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The database should be created before calling this function
 * @see save_db()
 */
extern int64_t load_db(db_t *db, uint8_t *file_path);

/**
 * @brief Saves database entries to a file
 * 
 * Writes all key-value pairs from the database to the specified file.
 * Uses a temporary file during the write process to ensure data integrity.
 * 
 * @param db Pointer to the database to save
 * @param file_path Path where the database should be saved
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The original file is replaced only if the save operation succeeds
 * @see load_db()
 */
extern int64_t save_db(db_t *db, uint8_t *file_path);

/**
 * @brief Inserts a database entry into the storage
 * 
 * Adds the given entry to the database using the appropriate storage method
 * (linked list or hash table based on the database's storage type).
 * 
 * @param db Pointer to the database
 * @param entry Pointer to the entry to insert
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The entry should be properly initialized before insertion
 * @see put_entry(), create_entry()
 */
extern int64_t insert_entry(db_t *db, db_entry_t *entry);

/**
 * @brief Creates and inserts a new entry with the given key, value, and type
 * 
 * This is a convenience function that creates a new entry from the provided
 * parameters and inserts it into the database. If an entry with the same key
 * already exists, it will be updated.
 * 
 * @param db Pointer to the database
 * @param key Key for the entry (null-terminated string)
 * @param value Value for the entry (null-terminated string)
 * @param type Type identifier for the value (e.g., "int", "string", "float")
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note All parameters must be non-NULL and non-empty strings
 * @see insert_entry(), get_entry()
 */
extern int64_t put_entry(db_t *db, uint8_t *key, uint8_t *value, uint8_t *type);

/**
 * @brief Deletes an entry from the database by key
 * 
 * Removes the entry with the specified key from the database storage.
 * 
 * @param db Pointer to the database
 * @param key Key of the entry to delete (null-terminated string)
 * @return int64_t 0 on success, -1 on failure (including if key not found)
 * 
 * @note The key parameter must be non-NULL and non-empty
 * @see get_entry(), put_entry()
 */
extern int64_t delete_entry(db_t *db, uint8_t *key);

/**
 * @brief Retrieves an entry from the database by key
 * 
 * Searches for and returns the entry with the specified key.
 * 
 * @param db Pointer to the database
 * @param key Key of the entry to retrieve (null-terminated string)
 * @return db_entry_t* Pointer to the found entry, or NULL if not found or on error
 * 
 * @note The returned pointer points to the actual entry in the database,
 *       not a copy. Do not free the returned pointer directly.
 * @see put_entry(), delete_entry()
 */
extern db_entry_t* get_entry(db_t *db, uint8_t *key);

/**
 * @brief Frees all memory associated with the database
 * 
 * Properly deallocates the database structure and all its contained entries.
 * This function should be called when the database is no longer needed.
 * 
 * @param db Pointer to the database to free
 * 
 * @note After calling this function, the db pointer should not be used
 * @see create_db()
 */
extern void free_db(db_t *db);

/**
 * @brief Prints all entries in the database to stdout
 * 
 * Displays all key-value pairs in the database in a formatted manner.
 * Useful for debugging and inspection purposes.
 * 
 * @param db Pointer to the database to print
 * 
 * @note Output format depends on the underlying storage implementation
 */
extern void print_db(db_t *db);
