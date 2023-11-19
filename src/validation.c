#include "validation.h"

#include <string.h>

// Validating relations
// ====================

void ensure_zone_identifiers_are_the_same(const char* id1, const char* id2) {
  if (strcmp(id1, id2) != 0) {
    fprintf(stderr, "Different zone identifiers: %s and %s\n", id1, id2);
    exit(1);
  }
}

// Validating JSON
// ===============

// JSON value content
// ------------------

void ensure_json_is_string(const json_t* j) {
  if (!json_is_string(j)) {
    fprintf(stderr, "JSON value is not a string\n");
    exit(1);
  }
}

void ensure_json_is_object(const json_t* j) {
  if (!json_is_object(j)) {
    fprintf(stderr, "JSON value is not an object\n");
    exit(1);
  }
}

void ensure_json_is_array(const json_t* j) {
  if (!json_is_array(j)) {
    fprintf(stderr, "JSON value is not an array\n");
    exit(1);
  }
}

void ensure_json_is_array_of_integers(const json_t* j) {
  ensure_json_is_array(j);
  int size = json_array_size(j);
  for (int i = 0; i < size; ++i)
    if (!json_is_integer(json_array_get(j, i))) {
      fprintf(stderr,
              "The value at index %d of JSON array is not an integer\n",
              i);
      exit(1);
    }
}

void ensure_json_is_array_of_numbers(const json_t* j) {
  ensure_json_is_array(j);
  int size = json_array_size(j);
  for (int i = 0; i < size; ++i)
    if (!json_is_number(json_array_get(j, i))) {
      fprintf(stderr, "The value at index %d of JSON array is not a number\n", i);
      exit(1);
    }
}

// JSON object content
// -------------------

void ensure_json_object_has_size(const json_t* j, int size) {
  ensure_json_is_object(j);
  if (json_object_size(j) != size) {
    fprintf(stderr, "Size of JSON object is not %d\n", size);
    exit(1);
  }
}

void ensure_json_object_contains_key(const json_t* j, const char* key) {
  ensure_json_is_object(j);
  if (json_object_get(j, key) == NULL) {
    fprintf(stderr, "JSON object does not contain the key \"%s\"\n", key);
    exit(1);
  }
}

// JSON array content
// ------------------

void ensure_json_array_has_size(const json_t* j, int size) {
  ensure_json_is_array(j);
  if (json_array_size(j) != size) {
    fprintf(stderr, "Size of JSON array is not %d\n", size);
    exit(1);
  }
}
