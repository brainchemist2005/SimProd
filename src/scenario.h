#ifndef SCENARIO_H
#define SCENARIO_H

#include <jansson.h>

#include "component/link.h"
#include "component/plant.h"
#include "component/zone.h"
#include "constants.h"
#include "timeline.h"

// JSON keys
// ---------

#define JSON_SCENARIO_LINKS "links"
#define JSON_SCENARIO_PLANTS "plants"
#define JSON_SCENARIO_TIMELINE "timeline"
#define JSON_SCENARIO_ZONES "zones"

// Type
// ----

struct Scenario {
  // The reference timeline
  struct Timeline timeline;
  // The number of links considered in the scenario
  unsigned int num_links;
  // The links considered in the scenario
  struct Link links[MAX_NUM_LINKS];
  // The number of plants considered in the scenario
  unsigned int num_plants;
  // The plants considered in the scenario
  struct Plant plants[MAX_NUM_PLANTS];
  // The number of zones considered in the scenario
  unsigned int num_zones;
  // The zones considered in the scenario
  struct Zone zones[MAX_NUM_ZONES];
};

// Initialization
// --------------

/**
 * Initializes a scenario
 *
 * @param scenario  The scenario to initialize
 * @param timeline  The reference timeline
 */
void scenario_initialize(struct Scenario* scenario,
                         const struct Timeline* timeline);

/**
 * Initializes a scenario from a JSON value
 *
 * @param scenario  The scenario to initialize
 * @param j         The JSON value
 */
void scenario_from_json(struct Scenario* scenario, json_t* j);

// Destruction
// -----------

/**
 * Frees a scenario
 *
 * @param scenario  The scenario to delete
 */
void scenario_free(struct Scenario* scenario);

// Modifiers
// ---------

/**
 * Adds a link to a scenario
 *
 * @param scenario  The scenario to which the link is added
 * @param link      The link to add
 */
void scenario_add_link(struct Scenario* scenario, const struct Link* link);

/**
 * Adds a plant to a scenario
 *
 * @param scenario  The scenario to which the plant is added
 * @param plant     The plant to add
 */
void scenario_add_plant(struct Scenario* scenario,
                        const struct Plant* plant);

/**
 * Adds a zone to a scenario
 *
 * @param scenario  The scenario to which the zone is added
 * @param zone      The zone to add
 */
void scenario_add_zone(struct Scenario* scenario,
                       const struct Zone* zone);

// Accessors
// ---------

/**
 * Returns the zone of a scenario with given identifier
 *
 * If no zone has the given identifier, returns NULL.
 *
 * @param scenario  The scenario
 * @param id        The identifier of the zone
 * @return          The zone with given identifier or NULL
 */
const struct Zone* scenario_zone_by_id(const struct Scenario* scenario,
                                       const char* id);

/**
 * Indicates if two scenarios are equal
 *
 * @param scenario1  The first scenario
 * @param scenario2  The second scenario
 */
bool scenario_are_equal(const struct Scenario* scenario1,
                        const struct Scenario* scenario2);

/**
 * Prints a scenario to stdout
 *
 * @param scenario  The scenario to print
 */
void scenario_print(const struct Scenario* scenario);

// JSON serialization
// ------------------

/**
 * Converts a scenario to a JSON value
 *
 * @param scenario  The scenario to convert
 * @return          The JSON value
 */
json_t* scenario_to_json(const struct Scenario* scenario);

#endif
