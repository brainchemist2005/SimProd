#include "treemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Help functions
// --------------

/**
 * Return the node associated with a given key in a tree map
 *
 * Note: if the key does not exist in the tree map, return NULL.
 *
 * @param node  The current node
 * @param key   The key to search
 * @return      The node associated with the key
 */
struct TreeNode* treemap_get_node(struct TreeNode* node,
                                  const char* key) {
  if (node == NULL) {
    return NULL;
  } else {
    int cmp = strcmp(key, node->kv.key);
    if (cmp == 0)
      return (struct TreeNode*)node;
    else if (cmp < 0)
      return treemap_get_node(node->left, key);
    else
      return treemap_get_node(node->right, key);
  }
}

/**
 * Insert a key-value node in a tree map
 *
 * @param node   The current node
 * @param key    The key to insert
 * @param value  The value to insert
 */
void treemap_insert_node(struct TreeNode** node,
                         const char* key,
                         double value) {
  if (*node == NULL) {
    *node = malloc(sizeof(struct TreeNode));
    (*node)->kv.key = strdup(key);
    (*node)->kv.value = value;
    (*node)->left = NULL;
    (*node)->right = NULL;
  } else if (strcmp(key, (*node)->kv.key) < 0) {
    treemap_insert_node(&(*node)->left, key, value);
  } else {
    treemap_insert_node(&(*node)->right, key, value);
  }
}

/**
 * Print the tree map starting from the given node
 *
 * @param node  The current node
 */
void treemap_print_node(const struct TreeNode* node) {
  if (node != NULL) {
    treemap_print_node(node->left);
    printf("  %s: %f\n", node->kv.key, node->kv.value);
    treemap_print_node(node->right);
  }
}

/**
 * Delete all nodes referenced by the given node
 *
 * @param node  The current node
 */
void treemap_delete_node(struct TreeNode* node) {
  if (node != NULL) {
    treemap_delete_node(node->left);
    treemap_delete_node(node->right);
    free(node->kv.key);
    free(node);
  }
}

/**
 * Indicates if all nodes in a given subtree belong to another treemap
 *
 * @param node  The root of the subtree
 * @param t     The treemap
 * @return      true if and only if all nodes in the given subtree occurs in
 *              the given treemap
 */
bool treemap_subtree_nodes_are_contained_in(const struct TreeNode* node,
                                            const struct Treemap* t) {
  if (node == NULL) {
    return true;
  } else {
    if (!treemap_has_key(t, node->kv.key) ||
        treemap_get(t, node->kv.key) != node->kv.value)
      return false;
    return treemap_subtree_nodes_are_contained_in(node->left, t) &&
           treemap_subtree_nodes_are_contained_in(node->right, t);
  }
}

// Initialization
// --------------

void treemap_initialize(struct Treemap* t) {
  t->root = NULL;
  t->num_entries = 0;
}

// Destruction
// -----------

void treemap_delete(struct Treemap *t) {
    treemap_delete_node(t->root);
}

// Modifiers
// ---------

void treemap_set(struct Treemap* t, const char* key, double value) {
  struct TreeNode* node = treemap_get_node(t->root, key);
  if (node != NULL) {
    node->kv.value = value;
  } else {
    treemap_insert_node(&(t->root), key, value);
    ++t->num_entries;
  }
}

// Accessors
// ---------

double treemap_get(const struct Treemap* t, const char* key) {
  struct TreeNode* node = treemap_get_node(t->root, key);
  return node == NULL ? 0.0 : node->kv.value;
}

bool treemap_has_key(const struct Treemap* t, const char* key) {
  return treemap_get_node(t->root, key) != NULL;
}

void treemap_add_keys(const struct TreeNode* node,
                      struct StringArray* sa) {
  if (node != NULL) {
    treemap_add_keys(node->left, sa);
    string_array_append(sa, node->kv.key);
    treemap_add_keys(node->right, sa);
  }
}

void treemap_compute_keys(const struct Treemap* t, struct StringArray* sa) {
  string_array_initialize(sa);
  treemap_add_keys(t->root, sa);
}

void treemap_print(const struct Treemap *t) {
  printf("Treemap {\n");
  treemap_print_node(t->root);
  printf("}\n");
}

bool treemap_are_equal(const struct Treemap* t1, const struct Treemap* t2) {
  return t1->num_entries == t2->num_entries &&
         treemap_subtree_nodes_are_contained_in(t1->root, t2);
}
