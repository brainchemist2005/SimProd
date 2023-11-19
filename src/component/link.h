#ifndef LINK_H
#define LINK_H

#include <stdbool.h>

#include "jansson.h"

#include "constants.h"
#include "unit.h"

// JSON keys
// ---------

#define JSON_LINK_ID "id"
#define JSON_LINK_SOURCE "source"
#define JSON_LINK_TARGET "target"

// Type
// ----

struct Link {
  // The identifier of the link
  char id[ID_MAX_LENGTH + 1];
  // The source zone of the link
  const struct Zone* source;
  // The target zone of the link
  const struct Zone* target;
};

// Initialization
// --------------

/**
 * Initializes a link
 *
 * @param link    The link to initialize
 * @param id      The identifier of the link
 * @param source  The source zone of the link
 * @param target  The source zone of the link
 */
void link_initialize(struct Link* link,
                     const char* id,
                     const struct Zone* source,
                     const struct Zone* target);

/**
 * Initializes a link from another one
 *
 * @param dest  The destination link
 * @param src   The source link
 */
void link_copy(struct Link* dest, const struct Link* src);

/**
 * Initializes a link from a JSON value
 *
 * @param link    The link to initialize
 * @param source  The source zone of the link
 * @param target  The source zone of the link
 * @param j       The JSON value
 */
void link_from_json(struct Link* link,
                    const struct Zone* source,
                    const struct Zone* target,
                    json_t* j);

// Destruction
// -----------

/**
 * Frees a link
 *
 * @param link  The link to free
 */
void link_free(struct Link* link);

// Accessors
// ---------

/**
 * Indicates if two links are equal
 *
 * @param link1  The first link
 * @param link2  The second link
 */
bool link_are_equal(const struct Link* link1, const struct Link* link2);

/**
 * Prints a link to stdout
 *
 * @param link  The link to print
 */
void link_print(const struct Link* link);

// JSON serialization
// ------------------

/**
 * Converts a link to a JSON value
 *
 * @param link  The link to convert
 * @return      The JSON value
 */
json_t* link_to_json(const struct Link* link);

#endif
