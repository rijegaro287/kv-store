/**
 * @file linked_list.h
 * @brief Linked list implementation for key-value store storage backend
 * 
 * This module provides a linked list data structure for storing database entries.
 * It serves as one of the storage backends for the key-value database, offering
 * sequential access and dynamic sizing capabilities.
 */
#pragma once

#include "kv_parser.h"
#include "logger.h"


/**
 * @brief Node structure for the linked list
 * 
 * Each node contains a database entry and a pointer to the next node,
 * forming a singly-linked list structure.
 */
typedef struct _node_t {
  db_entry_t *entry; /**< Pointer to the database entry stored in this node */
  struct _node_t* next; /**< Pointer to the next node in the list */
} node_t;

/**
 * @brief Linked list structure for storing database entries
 * 
 * This structure maintains a count of entries and a pointer to the head node
 * for efficient list management operations.
 */
typedef struct _list_t {
  uint64_t size;            /**< Number of entries currently in the list */
  node_t* head;             /**< Pointer to the first node in the list */
} list_t;

/**
 * @brief Creates a new empty linked list
 * 
 * Allocates and initializes a new linked list structure with zero size
 * and NULL head pointer.
 * 
 * @return list_t* Pointer to the newly created list, or NULL on failure
 * 
 * @note The caller is responsible for freeing the list using free_list()
 * @see free_list()
 */
extern list_t* create_list();

/**
 * @brief Inserts a database entry into the linked list
 * 
 * Adds the given entry to the list. If an entry with the same key already exists,
 * it will be updated with the new entry's value and type.
 * 
 * @param list Pointer to the linked list
 * @param entry Pointer to the database entry to insert
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The list takes ownership of the entry pointer
 * @note Duplicate keys result in value updates, not multiple entries
 * @see list_put(), list_delete()
 */
extern int64_t list_insert(list_t *list, db_entry_t *entry);

/**
 * @brief Creates and inserts a new entry with the given key, value, and type
 * 
 * This is a convenience function that creates a new entry from the provided
 * parameters and inserts it into the list. Handles key collision by updating
 * existing entries.
 * 
 * @param list Pointer to the linked list
 * @param key Key for the entry (null-terminated string)
 * @param value Value for the entry (null-terminated string)
 * @param type Type identifier for the value (e.g., "int32", "float", "bool")
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note All parameters must be non-NULL and non-empty strings
 * @see list_insert(), list_get_entry_by_key()
 */
extern int64_t list_put(list_t *list, uint8_t *key, uint8_t* value, uint8_t* type);

/**
 * @brief Deletes an entry from the linked list by key
 * 
 * Searches for and removes the entry with the specified key from the list.
 * Updates the list size and properly deallocates the removed node and entry.
 * 
 * @param list Pointer to the linked list
 * @param key Key of the entry to delete (null-terminated string)
 * @return int64_t 0 on success, -1 on failure (including if key not found)
 * 
 * @note The key parameter must be non-NULL and non-empty
 * @see list_get_entry_by_key(), list_insert()
 */
extern int64_t list_delete(list_t *list, uint8_t *key);

/**
 * @brief Retrieves an entry from the linked list by index
 * 
 * Returns the entry at the specified zero-based index position in the list.
 * Provides O(n) access time due to the sequential nature of linked lists.
 * 
 * @param list Pointer to the linked list
 * @param idx Zero-based index of the entry to retrieve
 * @return db_entry_t* Pointer to the entry at the specified index, or NULL if not found
 * 
 * @note Index must be less than the list size
 * @note The returned pointer points to the actual entry, not a copy
 * @see list_get_entry_by_key()
 */
extern db_entry_t *list_get_entry_by_idx(list_t *list, uint64_t idx);

/**
 * @brief Retrieves an entry from the linked list by key
 * 
 * Searches the list for an entry with the specified key and returns it.
 * Provides O(n) access time due to linear search.
 * 
 * @param list Pointer to the linked list
 * @param key Key of the entry to retrieve (null-terminated string)
 * @return db_entry_t* Pointer to the found entry, or NULL if not found
 * 
 * @note The returned pointer points to the actual entry, not a copy
 * @see list_get_entry_by_idx(), list_put()
 */
extern db_entry_t *list_get_entry_by_key(list_t *list, uint8_t *key);

/**
 * @brief Saves all entries in the linked list to a file
 * 
 * Iterates through the list and writes each entry to the specified file
 * in the serialized format "type:key=value;". Used for database persistence.
 * 
 * @param file Open file pointer for writing
 * @param list Pointer to the linked list to save
 * @return int64_t 0 on success, -1 on failure
 * 
 * @note The file must be opened for writing before calling this function
 * @note Each entry is written on a separate line
 * @see list_insert(), parse_line()
 */
extern int64_t list_save(FILE *file, list_t *list);

/**
 * @brief Frees a single node and its associated entry
 * 
 * Deallocates the memory for a node and its contained database entry.
 * This is typically used internally by other list management functions.
 * 
 * @param node Pointer to the node to free (can be NULL)
 * 
 * @note Safe to call with NULL pointer
 * @note Also frees the database entry contained in the node
 * @see free_list(), free_entry()
 */
extern void free_node(node_t *node);

/**
 * @brief Frees all memory associated with the linked list
 * 
 * Properly deallocates the entire list structure including all nodes and
 * their contained database entries. Should be called when the list is no longer needed.
 * 
 * @param list Pointer to the linked list to free (can be NULL)
 * 
 * @note Safe to call with NULL pointer
 * @note After calling this function, the list pointer should not be used
 * @see create_list(), free_node()
 */
extern void free_list(list_t *list);

/**
 * @brief Prints all entries in the linked list to stdout
 * 
 * Iterates through the list and displays each entry in a formatted manner.
 * Useful for debugging and inspection purposes.
 * 
 * @param list Pointer to the linked list to print
 * 
 * @note Output format depends on the print_entry() function implementation
 * @see print_entry()
 */
extern void list_print(list_t *list);
