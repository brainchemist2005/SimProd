#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

#include <stdbool.h>

// Types
// -----

// An array of strings
struct StringArray {
  char** strings;        // The strings contained in the array
  unsigned int size;     // The size of the array
  unsigned int capacity; // The capacity of the array
};

// Initialization
// --------------

/**
 * Initializes an array of strings
 *
 * @param sa  The string array to initialize
 */
void string_array_initialize(struct StringArray* sa);

// Destruction
// -----------

/**
 * Deletes a string array
 *
 * @param sa  The string array to delete
 */
void string_array_delete(struct StringArray* sa);

// Modifiers
// ---------

/**
 * Set the value for the given key in a tree map
 *
 * @param sa  The string array to which we append
 * @param s   The string to append
 */
void string_array_append(struct StringArray* sa, const char* s);

#endif
