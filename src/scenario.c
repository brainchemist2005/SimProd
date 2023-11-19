#include "scenario.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timeline.h"
#include "validation.h"

// Helpers
// -------

/**
 * Adds links from a JSON value to a scenario
 *
 * @param scenario  The scenario to which the links are added
 * @param j_links   The JSON value containing the links
 */
void scenario_add_links_from_json(struct Scenario* scenario,
                                  const json_t* j_links) {
  ensure_json_is_array(j_links);
  int num_links = json_array_size(j_links);
  for (int l = 0; l < num_links; ++l) {
    json_t* j_link = json_array_get(j_links, l);
    const char* source_z_id = json_string_value(
      json_object_get(j_link, JSON_LINK_SOURCE));
    const char* target_z_id = json_string_value(
      json_object_get(j_link, JSON_LINK_TARGET));
    const struct Zone* source_zone = scenario_zone_by_id(scenario, source_z_id);
    const struct Zone* target_zone = scenario_zone_by_id(scenario, target_z_id);
    struct Link link;
    link_from_json(&link, source_zone, target_zone, j_link);
    scenario_add_link(scenario, &link);
    link_free(&link);
  }
}

/**
 * Adds plants from a JSON value to a scenario
 *
 * @param scenario  The scenario to which the plants are added
 * @param j_plants  The JSON value containing the plants
 */
void scenario_add_plants_from_json(struct Scenario* scenario,
                                   const json_t* j_plants) {
  ensure_json_is_array(j_plants);
  int num_plants = json_array_size(j_plants);
  for (int p = 0; p < num_plants; ++p) {
    json_t* j_plant = json_array_get(j_plants, p);
    const char* z_id = json_string_value(json_object_get(j_plant,
                                                         JSON_PLANT_ZONE));
    const struct Zone* zone = scenario_zone_by_id(scenario, z_id);
    struct Plant plant;
    plant_from_json(&plant, &scenario->timeline, zone, j_plant);
    scenario_add_plant(scenario, &plant);
    plant_free(&plant);
  }
}

/**
 * Adds zones from a JSON value to a scenario
 *
 * @param scenario  The scenario to which the zones are added
 * @param j_zones   The JSON value containing the zones
 */
void scenario_add_zones_from_json(struct Scenario* scenario,
                                  const json_t* j_zones) {
  ensure_json_is_array(j_zones);
  int num_zones = json_array_size(j_zones);
  for (int z = 0; z < num_zones; ++z) {
    json_t* j_zone = json_array_get(j_zones, z);
    struct Zone zone;
    zone_from_json(&zone, &scenario->timeline, j_zone);
    scenario_add_zone(scenario, &zone);
    zone_free(&zone);
  }
}

// Initialization
// --------------

void scenario_initialize(struct Scenario* scenario,
                         const struct Timeline* timeline) {
  timeline_copy(&scenario->timeline, timeline);
  scenario->num_links = 0;
  scenario->num_plants = 0;
  scenario->num_zones = 0;
}

void scenario_from_json(struct Scenario* scenario, json_t* j) {
  ensure_json_is_object(j);
  ensure_json_object_contains_key(j, JSON_SCENARIO_TIMELINE);
  json_t* j_timeline = json_object_get(j, JSON_SCENARIO_TIMELINE);
  struct Timeline timeline;
  timeline_from_json(&timeline, j_timeline);
  scenario_initialize(scenario, &timeline);
  timeline_free(&timeline);
  const json_t* j_zones = json_object_get(j, JSON_SCENARIO_ZONES);
  if (j_zones != NULL)
    scenario_add_zones_from_json(scenario, j_zones);
  const json_t* j_links = json_object_get(j, JSON_SCENARIO_LINKS);
  if (j_links != NULL)
    scenario_add_links_from_json(scenario, j_links);
  const json_t* j_plants = json_object_get(j, JSON_SCENARIO_PLANTS);
  if (j_plants != NULL)
    scenario_add_plants_from_json(scenario, j_plants);
}

// Destruction
// -----------

void scenario_free(struct Scenario* scenario) {
  for (int p = 0; p < scenario->num_plants; ++p)
    plant_free(scenario->plants + p);
  for (int z = 0; z < scenario->num_zones; ++z)
    zone_free(scenario->zones + z);
  timeline_free(&scenario->timeline);
}

// Modifiers
// ---------

void scenario_add_link(struct Scenario* scenario, const struct Link* link) {
  link_copy(scenario->links + scenario->num_links, link);
  ++scenario->num_links;
}

void scenario_add_plant(struct Scenario* scenario,
                        const struct Plant* plant) {
  plant_copy(scenario->plants + scenario->num_plants, plant);
  ++scenario->num_plants;
}

void scenario_add_zone(struct Scenario* scenario,
                       const struct Zone* zone) {
  zone_copy(scenario->zones + scenario->num_zones, zone);
  ++scenario->num_zones;
}

// Accessors
// ---------

const struct Zone* scenario_zone_by_id(const struct Scenario* scenario,
                                       const char* id) {
  for (int z = 0; z < scenario->num_zones; ++z)
    if (strcmp(scenario->zones[z].id, id) == 0)
      return scenario->zones + z;
  return NULL;
}

bool scenario_are_equal(const struct Scenario* scenario1,
                        const struct Scenario* scenario2) {
  if (!timeline_are_equal(&scenario1->timeline, &scenario2->timeline))
    return false;
  if (scenario1->num_links != scenario2->num_links)
    return false;
  for (int l = 0; l < scenario1->num_links; ++l)
    if (!link_are_equal(scenario1->links + l, scenario2->links + l))
      return false;
  if (scenario1->num_plants != scenario2->num_plants)
    return false;
  for (int p = 0; p < scenario1->num_plants; ++p)
    if (!plant_are_equal(scenario1->plants + p, scenario2->plants + p))
      return false;
  if (scenario1->num_zones != scenario2->num_zones)
    return false;
  for (int z = 0; z < scenario1->num_zones; ++z)
    if (!zone_are_equal(scenario1->zones + z, scenario2->zones + z))
      return false;
  return true;
}

void scenario_print(const struct Scenario* scenario) {
  printf("A scenario with the following components:\n  ");
  timeline_print(&scenario->timeline);
  for (unsigned int p = 0; p < scenario->num_plants; ++p)
    plant_print(scenario->plants + p);
  for (unsigned int z = 0; z < scenario->num_zones; ++z)
    zone_print(scenario->zones + z);
}

// JSON serialization
// ------------------

json_t* scenario_to_json(const struct Scenario* scenario) {
  json_t* jlinks = json_array();
  for (int l = 0; l < scenario->num_links; ++l)
    json_array_append_new(jlinks, link_to_json(scenario->links + l));
  json_t* jplants = json_array();
  for (int p = 0; p < scenario->num_plants; ++p)
    json_array_append_new(jplants, plant_to_json(scenario->plants + p));
  json_t* jzones = json_array();
  for (int z = 0; z < scenario->num_zones; ++z)
    json_array_append_new(jzones, zone_to_json(scenario->zones + z));
  return json_pack("{s:o,s:o,s:o,s:o}",
                   JSON_SCENARIO_LINKS,
                   jlinks,
                   JSON_SCENARIO_PLANTS,
                   jplants,
                   JSON_SCENARIO_TIMELINE,
                   timeline_to_json(&scenario->timeline),
                   JSON_SCENARIO_ZONES,
                   jzones);
}
