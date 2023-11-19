#include <jansson.h>

// Validating relations
// ====================

/**
 * Ensures that the two given zone identifiers are the same
 *
 * If not, prints an error message and exits the program.
 *
 * @param id1  The first identifier
 * @param id2  The second identifier
 */
void ensure_zone_identifiers_are_the_same(const char* id1, const char* id2);

// Validating JSON
// ===============

// JSON value content
// ------------------

/**
 * Ensures that the given JSON value contains a string
 *
 * If not, prints an error message and exits the program.
 *
 * @param j  The JSON value
 */
void ensure_json_is_string(const json_t* j);

/**
 * Ensures that the given JSON value is an object
 *
 * If not, prints an error message and exits the program.
 *
 * @param j  The JSON value
 */
void ensure_json_is_object(const json_t* j);

/**
 * Ensures that the given JSON value is an array
 *
 * If not, prints an error message and exits the program.
 *
 * @param j  The JSON value
 */
void ensure_json_is_array(const json_t* j);

/**
 * Ensures that the given JSON value is an array of integers
 *
 * If not, prints an error message and exits the program.
 *
 * @param j  The JSON value
 */
void ensure_json_is_array_of_integers(const json_t* j);

/**
 * Ensures that the given JSON value is an array of numbers
 *
 * If not, prints an error message and exits the program.
 *
 * @param j  The JSON value
 */
void ensure_json_is_array_of_numbers(const json_t* j);

// JSON object content
// -------------------

/**
 * Ensures that the given JSON object has the given size
 *
 * If not, prints an error message and exits the program.
 *
 * @param j     The JSON object
 * @param size  The expected size
 */
void ensure_json_object_has_size(const json_t* j, int size);

/**
 * Ensures that the given JSON object has the given key
 *
 * If not, prints an error message and exits the program.
 *
 * @param j    The JSON object
 * @param key  The key
 */
void ensure_json_object_contains_key(const json_t* j, const char* key);

// JSON array content
// ------------------

/**
 * Ensures that the given JSON array has the given size
 *
 * If not, prints an error message and exits the program.
 *
 * @param j     The JSON object
 * @param size  The expected size
 */
void ensure_json_array_has_size(const json_t* j, int size);
