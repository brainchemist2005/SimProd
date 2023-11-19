#ifndef PLANT_H
#define PLANT_H

#include "constants.h"
#include "timeline.h"
#include "unit.h"
#include "zone.h"

// JSON keys
// ---------

#define JSON_PLANT_ID "id"
#define JSON_PLANT_ZONE "zone"
#define JSON_PLANT_MIN_POWERS "min-powers"
#define JSON_PLANT_MAX_POWERS "max-powers"

// Type
// ----

struct Plant {
  // The identifier of the plant
  char id[ID_MAX_LENGTH + 1];
  // The reference timeline
  const struct Timeline* timeline;
  // The zone in which the plant is located
  const struct Zone* zone;
  // The maximum powers that the plant can produce for each timestep
  mw* max_powers;
  // The minimum powers that the plant can produce for each timestep
  mw* min_powers;
};

// Initialization
// --------------

/**
 * Initializes a plant
 *
 * @param plant       The plant to initialize
 * @param id          The identifier of the plant
 * @param timeline    The reference timeline of the plant
 * @param zone        The zone in which the plant is located
 * @param min_powers  The minimum powers that the plant can produce
 * @param max_powers  The maximum powers that the plant can produce
 */
void plant_initialize(struct Plant* plant,
                      const char* id,
                      const struct Timeline* timeline,
                      const struct Zone* zone,
                      const mw* min_powers,
                      const mw* max_powers);

/**
 * Initializes a plant from another one
 *
 * @param dest  The destination plant
 * @param src   The source plant
 */
void plant_copy(struct Plant* dest, const struct Plant* src);

/**
 * Initializes a plant from a JSON value
 *
 * @param plant     The plant to initialize
 * @param timeline  The reference timeline of the plant
 * @param zone      The zone containing the plant
 * @param j         The JSON value
 */
void plant_from_json(struct Plant* plant,
                     const struct Timeline* timeline,
                     const struct Zone* zone,
                     json_t* j);

// Destruction
// -----------

/**
 * Frees a plant
 *
 * @param plant  The plant to free
 */
void plant_free(struct Plant* plant);

// Accessors
// ---------

/**
 * Indicates if two plants are equal
 *
 * @param plant1  The first plant
 * @param plant2  The second plant
 */
bool plant_are_equal(const struct Plant* plant1, const struct Plant* plant2);

/**
 * Prints a plant to stdout
 *
 * @param plant  The plant to print
 */
void plant_print(const struct Plant* plant);

// JSON serialization
// ------------------

/**
 * Converts a plant to a JSON value
 *
 * @param plant  The plant to convert
 * @return       The JSON value
 */
json_t* plant_to_json(const struct Plant* plant);

#endif
