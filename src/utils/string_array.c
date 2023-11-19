#include "string_array.h"

#include <stdlib.h>
#include <string.h>

// Initialization
// --------------

void string_array_initialize(struct StringArray* sa) {
  sa->strings = malloc(sizeof(char*));
  sa->size = 0;
  sa->capacity = 1;
}

// Destruction
// -----------

void string_array_delete(struct StringArray* sa) {
  free(sa->strings);
}

// Modifiers
// ---------

void string_array_append(struct StringArray* sa, const char* s) {
  if (sa->capacity == sa->size) {
    sa->capacity *= 2;
    sa->strings = realloc(sa->strings, sa->capacity * sizeof(sizeof(char*)));
  }
  sa->strings[sa->size] = strdup(s);
  ++sa->size;
}
