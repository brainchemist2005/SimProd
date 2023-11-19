#ifndef TREEMAP_H
#define TREEMAP_H

#include <stdbool.h>

#include "string_array.h"

// Types
// -----

// A key-value pair
struct KeyValue {
  char* key;    // The key
  double value; // The value
};

// A node with string key and double value
struct TreeNode {
  struct KeyValue kv;     // The key-value pair stored in the node
  struct TreeNode* left;  // Left subtree
  struct TreeNode* right; // Right subtree
};

// A treemap associating strings with doubles
struct Treemap {
  struct TreeNode* root;    // Root of tree
  unsigned int num_entries; // The number of entries
};

// Initialization
// --------------

/**
 * Initialize an empty tree map
 *
 * @param t  The tree map to initialize
 */
void treemap_initialize(struct Treemap* t);

// Destruction
// -----------

/**
 * Delete a tree map
 *
 * @param t  The tree map to delete
 */
void treemap_delete(struct Treemap* t);

// Modifiers
// ---------

/**
 * Set the value for the given key in a tree map
 *
 * @param t      The tree map
 * @param key    The key
 * @param value  The value
 */
void treemap_set(struct Treemap* t, const char* key, double value);

// Accessors
// ---------

/**
 * Return the value associated with the given key in a tree map
 *
 * Note: if the key does not exist, return 0.0.
 *
 * @param t    The tree map
 * @param key  The key to search
 */
double treemap_get(const struct Treemap* t, const char* key);

/**
 * Indicate if a key exists in a tree map
 *
 * @param t    The tree map
 * @param key  The key
 * @return     True if and only if the key exists in the tree map
 */
bool treemap_has_key(const struct Treemap* t, const char* key);

/**
 * Computes the keys appearing in a tree map
 *
 * Note: The result is placed in an array `sa`, which must be freed with
 * stringarray_free when it is not needed anymore.
 *
 * @param t   The treemap
 * @param sa  The string array in which the keys are stored
 */
void treemap_compute_keys(const struct Treemap* t, struct StringArray* sa);

/**
 * Print a tree map to stdout
 *
 * @param t  The tree map to print
 */
void treemap_print(const struct Treemap* t);

/**
 * Indicates if two treemaps contains the same key and values
 *
 * @param t1  The first treemap
 * @param t2  The second treemap
 * @return    true if and only if both treemaps contains the same key-value
 *            pairs
 */
bool treemap_are_equal(const struct Treemap* t1, const struct Treemap* t2);

#endif
