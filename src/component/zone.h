#ifndef ZONE_H
#define ZONE_H

#include <stdbool.h>

#include <jansson.h>

#include "constants.h"
#include "unit.h"

// JSON keys
// ---------

#define JSON_ZONE_ID "id"
#define JSON_ZONE_EXPECTED_DEMANDS "expected-demands"

// Type
// ----

struct Zone {
  // The identifier of the zone
  char id[ID_MAX_LENGTH + 1];
  // The reference timeline
  const struct Timeline* timeline;
  // The expected demand for each timestep
  mw* expected_demands;
};

// Initialization
// --------------

/**
 * Initializes a zone
 *
 * @param zone              The zone to initialize
 * @param id                The identifier of the zone
 * @param timeline          The reference timeline of the zone
 * @param expected_demands  The expected demand in the zone for each time step
 */
void zone_initialize(struct Zone* zone,
                     const char* id,
                     const struct Timeline* timeline,
                     const mw* expected_demands);

/**
 * Initializes a zone from another one
 *
 * @param dest  The destination zone
 * @param src   The source zone
 */
void zone_copy(struct Zone* dest, const struct Zone* src);

/**
 * Initializes a zone from a JSON value
 *
 * @param zone      The zone to initialize
 * @param timeline  The reference timeline of the zone
 * @param j         The JSON value
 */
void zone_from_json(struct Zone* zone,
                    const struct Timeline* timeline,
                    json_t* j);

// Destruction
// -----------

/**
 * Frees a zone
 *
 * @param zone  The zone to free
 */
void zone_free(struct Zone* zone);

// Accessors
// ---------

/**
 * Indicates if two zones are equal
 *
 * @param zone1  The first zone
 * @param zone2  The second zone
 */
bool zone_are_equal(const struct Zone* zone1, const struct Zone* zone2);

/**
 * Prints a zone to stdout
 *
 * @param zone  The zone to print
 */
void zone_print(const struct Zone* zone);

// JSON serialization
// ------------------

/**
 * Converts a zone to a JSON value
 *
 * @param zone  The zone to convert
 * @return      The JSON value
 */
json_t* zone_to_json(const struct Zone* zone);

#endif
