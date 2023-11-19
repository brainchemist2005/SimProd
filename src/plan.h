#ifndef PLAN_H
#define PLAN_H

#include <jansson.h>

#include "constants.h"
#include "scenario.h"
#include "timeline.h"
#include "unit.h"
#include "utils/treemap.h"

// JSON keys
// ---------

#define JSON_PLAN_PRODUCTIONS "productions"
#define JSON_PLAN_TIMELINE "timeline"

// Type
// ----

struct Plan {
  // The reference timeline of the plan
  struct Timeline timeline;
  // The treemaps associating plants with their production, for each time step
  struct Treemap* productions;
};

// Initialization
// --------------

/**
 * Initializes a plan on a reference timeline
 *
 * @param plan      The plan to initialize
 * @param timeline  The reference timeline
 */
void plan_initialize(struct Plan* plan,
                     const struct Timeline* timeline);

/**
 * Initializes a plan from a JSON value
 *
 * @param plan  The plan to initialize
 * @param j     The JSON value
 */
void plan_from_json(struct Plan* plan, json_t* j);

// Destruction
// -----------

/**
 * Frees a plan
 *
 * @param plan  The plan to delete
 */
void plan_free(struct Plan* plan);

// Modifiers
// ---------

/**
 * Sets the production of a plant at a given timestep
 *
 * @param plan        The plan to modify
 * @param t           The index of the time step
 * @param id          The identifier of the plant whose production is updated
 * @param production  The production to use
 */
void plan_set_production(struct Plan* plan,
                         int t,
                         const char* id,
                         mw production);

// Accessors
// ---------

/**
 * Returns the production of a plant at a given timestep
 *
 * @param plan   The accessed plan
 * @param t      The index of the time step
 * @param id     The identifier of the plant whose production is updated
 * @return       The production of the plant at a given time step
 */
mw plan_get_production(struct Plan* plan,
                       int t,
                       const char* id);

/**
 * Indicates if two plans are equal
 *
 * @param plan1  The first plan
 * @param plan2  The second plan
 */
bool plan_are_equal(const struct Plan* plan1, const struct Plan* plan2);

// JSON serialization
// ------------------

/**
 * Converts a plan to a JSON value
 *
 * @param plan  The plan to convert
 * @return      The JSON value
 */
json_t* plan_to_json(const struct Plan* plan);

#endif
