#include "timeline.h"

#include <stdlib.h>

#include <tap.h>

#include "component/link.h"
#include "component/plant.h"
#include "component/zone.h"
#include "scenario.h"
#include "unit.h"

// JSON verification helpers
// =========================

/**
 * Checks that the JSON shape of a JSON scenario is as expected
 *
 * @param j_scenario  The JSON value of a scenario
 */
void test_scenario_json_shape(const json_t* j_scenario) {
  ok(json_is_object(j_scenario), "json value is an object");
  cmp_ok(json_object_size(j_scenario), "==", 4, "json object has size 4");
  const json_t* j_links = json_object_get(j_scenario, "links");
  const json_t* j_plants = json_object_get(j_scenario, "plants");
  const json_t* j_timeline = json_object_get(j_scenario, "timeline");
  const json_t* j_zones = json_object_get(j_scenario, "zones");
  ok(j_links != NULL, "json value has key \"links\"");
  ok(j_plants != NULL, "json value has key \"plants\"");
  ok(j_timeline != NULL, "json value has key \"timeline\"");
  ok(j_zones != NULL, "json value has key \"zones\"");
  ok(json_is_array(j_links), "value associated with \"links\" is array");
  ok(json_is_array(j_plants), "value associated with \"plants\" is array");
  ok(json_is_object(j_timeline), "value associated with \"timeline\" is object");
  ok(json_is_array(j_zones), "value associated with \"zones\" is array");
}

// Scenario equality
// =================

/**
 * Tests the scenario_are_equal function
 */
void test_scenario_are_equal(void) {
  diag("Testing scenario_are_equal");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline1, timeline2;
  timeline_initialize(&timeline1, 3, durations);
  timeline_initialize(&timeline2, 2, durations);
  mw expected_demands[] = {5.0, 6.0, 7.0};
  struct Zone zone1, zone2;
  zone_initialize(&zone1, "Z1", &timeline1, expected_demands);
  zone_initialize(&zone2, "Z2", &timeline1, expected_demands);
  mw min_powers[] = {1.0, 2.0, 3.0},
     max_powers[] = {7.0, 8.0, 9.0};
  struct Plant plant1, plant2;
  plant_initialize(&plant1, "P1", &timeline1, &zone1, min_powers, max_powers);
  plant_initialize(&plant2, "P2", &timeline1, &zone1, min_powers, max_powers);
  struct Scenario scenario1, scenario2, scenario3, scenario4, scenario5, scenario6;
  scenario_initialize(&scenario1, &timeline1);
  scenario_initialize(&scenario2, &timeline2);
  scenario_initialize(&scenario3, &timeline1);
  scenario_initialize(&scenario4, &timeline1);
  scenario_initialize(&scenario5, &timeline1);
  scenario_initialize(&scenario6, &timeline1);
  scenario_add_zone(&scenario3, &zone1);
  scenario_add_zone(&scenario4, &zone2);
  scenario_add_plant(&scenario5, &plant1);
  scenario_add_plant(&scenario6, &plant2);

  ok(scenario_are_equal(&scenario1, &scenario1),
     "an empty scenario is equal to itself");
  ok(!scenario_are_equal(&scenario1, &scenario2),
     "scenario with different timelines are different");
  ok(!scenario_are_equal(&scenario1, &scenario3),
     "an empty scenario is different from a scenario with one zone");
  ok(!scenario_are_equal(&scenario3, &scenario4),
     "two scenarios with different zones are different");
  ok(!scenario_are_equal(&scenario1, &scenario5),
     "an empty scenario is different from a scenario with one plant");
  ok(!scenario_are_equal(&scenario5, &scenario6),
     "two scenarios with different pants are different");

  // Teardown
  scenario_free(&scenario1);
  scenario_free(&scenario2);
  scenario_free(&scenario3);
  scenario_free(&scenario4);
  scenario_free(&scenario5);
  scenario_free(&scenario6);
  plant_free(&plant1);
  plant_free(&plant2);
  zone_free(&zone1);
  zone_free(&zone2);
  timeline_free(&timeline1);
  timeline_free(&timeline2);
}

// Empty scenario
// ==============

// An example of an empty scenario with its timeline
struct EmptyScenarioExample {
  struct Timeline timeline; // The timeline
  struct Scenario scenario; // The scenario
};

/**
 * Initializes an example of an empty scenario
 *
 * @param example  The example to initialize
 */
void empty_scenario_example_initialize(
  struct EmptyScenarioExample* example) {
  diag("Building an empty scenario");
  int durations[] = {10, 30, 60};
  timeline_initialize(&example->timeline, 3, durations);
  scenario_initialize(&example->scenario, &example->timeline);
}

/**
 * Frees an example of an empty scenario
 *
 * @param example  The example to free
 */
void empty_scenario_example_free(struct EmptyScenarioExample* example) {
  scenario_free(&example->scenario);
  timeline_free(&example->timeline);
}

/**
 * Tests the scenario_initialize function on an example of empty scenario
 */
void test_empty_scenario_initialize(void) {
  diag("Testing scenario_initialize");
  struct EmptyScenarioExample example;
  empty_scenario_example_initialize(&example);
  const struct Timeline* timeline = &example.timeline;
  const struct Scenario* scenario = &example.scenario;

  ok(timeline_are_equal(&scenario->timeline, timeline),
     "scenario timeline is equal to the provided timeline");
  cmp_ok(scenario->num_plants, "==", 0,
         "number of plants in scenario is 0");
  cmp_ok(scenario->num_zones, "==", 0,
         "number of zones in scenario is 0");

  empty_scenario_example_free(&example);
}

/**
 * Tests the scenario_to_json function on an example of empty scenario
 */
void test_empty_scenario_to_json(void) {
  diag("Testing scenario_to_json");
  struct EmptyScenarioExample example;
  empty_scenario_example_initialize(&example);
  const struct Timeline* timeline = &example.timeline;
  const struct Scenario* scenario = &example.scenario;

  json_t* j_scenario = scenario_to_json(scenario);
  test_scenario_json_shape(j_scenario);
  const json_t* j_plants = json_object_get(j_scenario, "plants");
  const json_t* j_timeline = json_object_get(j_scenario, "timeline");
  const json_t* j_zones = json_object_get(j_scenario, "zones");
  cmp_ok(json_array_size(j_plants), "==", 0, "size of array \"plants\" is 0");
  json_t* j_source_timeline = timeline_to_json(timeline);
  ok(json_equal(j_timeline, j_source_timeline),
     "timeline and source timeline are equal as json values");
  cmp_ok(json_array_size(j_zones), "==", 0, "size of array \"zones\" is 0");

  json_decref(j_source_timeline);
  json_decref(j_scenario);
  empty_scenario_example_free(&example);
}

/**
 * Tests the scenario_from_json function on an example of empty scenario
 */
void test_empty_scenario_from_json(void) {
  diag("Testing scenario_from_json");
  struct EmptyScenarioExample example;
  empty_scenario_example_initialize(&example);
  const struct Scenario* scenario = &example.scenario;

  json_t* j_scenario = scenario_to_json(scenario);
  struct Scenario scenario_from_j;
  scenario_from_json(&scenario_from_j, j_scenario);
  ok(scenario_are_equal(scenario, &scenario_from_j),
     "manually built scenario and JSON scenario are equal");

  json_decref(j_scenario);
  scenario_free(&scenario_from_j);
  empty_scenario_example_free(&example);
}

/**
 * Tests an example of empty scenario
 */
void test_empty_scenario(void) {
  test_empty_scenario_initialize();
  test_empty_scenario_to_json();
  test_empty_scenario_from_json();
}

// Scenario with zone
// ==================

// An example of a scenario with zone
struct ScenarioWithZoneExample {
  struct Scenario scenario; // The scenario
  struct Timeline timeline; // The timeline
  struct Zone zone;         // The zone
};

/**
 * Initializes an example of scenario with zone
 *
 * @param example  The example to initialize
 */
void scenario_with_zone_example_initialize(
  struct ScenarioWithZoneExample* example) {
  diag("Building a scenario with one zone");
  int durations[] = {10, 30, 60};
  timeline_initialize(&example->timeline, 3, durations);
  mw expected_demands[] = {5.0, 6.0, 7.0};
  zone_initialize(&example->zone, "Z", &example->timeline, expected_demands);
  scenario_initialize(&example->scenario, &example->timeline);
  scenario_add_zone(&example->scenario, &example->zone);
}

/**
 * Frees an example of scenario with zone
 *
 * @param example  The example to free
 */
void scenario_with_zone_example_free(struct ScenarioWithZoneExample* example) {
  scenario_free(&example->scenario);
  zone_free(&example->zone);
  timeline_free(&example->timeline);
}

/**
 * Tests the scenario_add_zone function on an example of scenario with zone
 */
void test_scenario_add_zone(void) {
  diag("Testing scenario_add_zone");
  struct ScenarioWithZoneExample example;
  scenario_with_zone_example_initialize(&example);
  const struct Timeline* timeline = &example.timeline;
  const struct Zone* zone = &example.zone;
  const struct Scenario* scenario = &example.scenario;

  cmp_ok(scenario->num_zones, "==", 1,
         "number of zones in scenario is 1");
  ok(zone_are_equal(scenario->zones, zone),
     "first zone of scenario is equal to the provided zone");

  scenario_with_zone_example_free(&example);
}

/**
 * Tests the scenario_to_json function on an example of scenario with zone
 */
void test_scenario_with_zone_to_json(void) {
  diag("Testing scenario_to_json");
  struct ScenarioWithZoneExample example;
  scenario_with_zone_example_initialize(&example);
  const struct Timeline* timeline = &example.timeline;
  const struct Scenario* scenario = &example.scenario;
  const struct Zone* zone = &example.zone;

  json_t* j_scenario = scenario_to_json(scenario);
  test_scenario_json_shape(j_scenario);
  const json_t* j_plants = json_object_get(j_scenario, "plants");
  const json_t* j_timeline = json_object_get(j_scenario, "timeline");
  const json_t* j_zones = json_object_get(j_scenario, "zones");
  cmp_ok(json_array_size(j_plants), "==", 0, "size of array \"plants\" is 0");
  json_t* j_source_timeline = timeline_to_json(timeline);
  ok(json_equal(j_timeline, j_source_timeline),
     "timeline and source timeline are equal as json values");
  cmp_ok(json_array_size(j_zones), "==", 1, "size of array \"zones\" is 1");
  const json_t* j_zone = json_array_get(j_zones, 0);
  json_t* j_source_zone = zone_to_json(zone);
  ok(json_equal(j_zone, j_source_zone),
     "zone and source zone are equal as json values");

  json_decref(j_source_timeline);
  json_decref(j_source_zone);
  json_decref(j_scenario);
  scenario_with_zone_example_free(&example);
}

/**
 * Tests the scenario_from_json function on an example of scenario with zone
 */
void test_scenario_with_zone_from_json(void) {
  diag("Testing scenario_from_json");
  struct ScenarioWithZoneExample example;
  scenario_with_zone_example_initialize(&example);
  const struct Scenario* scenario = &example.scenario;

  json_t* j_scenario = scenario_to_json(scenario);
  struct Scenario scenario_from_j;
  scenario_from_json(&scenario_from_j, j_scenario);
  ok(scenario_are_equal(scenario, &scenario_from_j),
     "manually built scenario and JSON scenario are equal");

  json_decref(j_scenario);
  scenario_free(&scenario_from_j);
  scenario_with_zone_example_free(&example);
}

/**
 * Tests a scenario with zone
 */
void test_scenario_with_zone(void) {
  test_scenario_add_zone();
  test_scenario_with_zone_to_json();
  test_scenario_with_zone_from_json();
}

// Scenario with link and zones
// ============================

// An example of a scenario with link and zones
struct ScenarioWithLinkAndZonesExample {
  struct Link link;         // The link
  struct Scenario scenario; // The scenario
  struct Timeline timeline; // The timeline
  struct Zone source_zone;  // The source zone of the link
  struct Zone target_zone;  // The target zone of the link
};

/**
 * Initializes an example of scenario with link and zones
 *
 * @param example  The example to initialize
 */
void scenario_with_link_and_zones_example_initialize(
  struct ScenarioWithLinkAndZonesExample* example) {
  diag("Building a scenario with one link and two zones");
  int durations[] = {10, 30, 60};
  timeline_initialize(&example->timeline, 3, durations);
  mw expected_demands[] = {5.0, 6.0, 7.0};
  zone_initialize(&example->source_zone,
                 "Z1",
                 &example->timeline,
                 expected_demands);
  zone_initialize(&example->target_zone,
                 "Z2",
                 &example->timeline,
                 expected_demands);
  link_initialize(&example->link,
                  "Z1->Z2",
                  &example->source_zone,
                  &example->target_zone);
  scenario_initialize(&example->scenario, &example->timeline);
  scenario_add_zone(&example->scenario, &example->source_zone);
  scenario_add_zone(&example->scenario, &example->target_zone);
  scenario_add_link(&example->scenario, &example->link);
}

/**
 * Frees an example of scenario with zone
 *
 * @param example  The example to free
 */
void scenario_with_link_and_zones_example_free(
  struct ScenarioWithLinkAndZonesExample* example) {
  link_free(&example->link);
  scenario_free(&example->scenario);
  timeline_free(&example->timeline);
  zone_free(&example->source_zone);
  zone_free(&example->target_zone);
}

/**
 * Tests the scenario_add_link function on an example of scenario with link and
 * zones
 */
void test_scenario_add_link(void) {
  diag("Testing scenario_add_link");
  struct ScenarioWithLinkAndZonesExample example;
  scenario_with_link_and_zones_example_initialize(&example);
  const struct Link* link = &example.link;
  const struct Scenario* scenario = &example.scenario;
  const struct Zone* source_zone = &example.source_zone;
  const struct Zone* target_zone = &example.target_zone;

  cmp_ok(scenario->num_zones, "==", 2,
         "number of zones in scenario is 2");
  ok(zone_are_equal(scenario->zones, source_zone),
     "first zone of scenario is equal to the source zone");
  ok(zone_are_equal(scenario->zones + 1, target_zone),
     "second zone of scenario is equal to the target zone");
  cmp_ok(scenario->num_links, "==", 1,
         "number of links in scenario is 1");
  ok(link_are_equal(scenario->links, link),
     "link of scenario is equal to the provided link");

  scenario_with_link_and_zones_example_free(&example);
}

/**
 * Tests the scenario_to_json function on an example of scenario with link and
 * zones
 */
void test_scenario_with_link_and_zones_to_json(void) {
  diag("Testing scenario_to_json");
  struct ScenarioWithLinkAndZonesExample example;
  scenario_with_link_and_zones_example_initialize(&example);
  const struct Link* link = &example.link;
  const struct Scenario* scenario = &example.scenario;
  const struct Timeline* timeline = &example.timeline;
  const struct Zone* source_zone = &example.source_zone;
  const struct Zone* target_zone = &example.target_zone;

  json_t* j_scenario = scenario_to_json(scenario);
  test_scenario_json_shape(j_scenario);
  const json_t* j_links = json_object_get(j_scenario, "links");
  const json_t* j_plants = json_object_get(j_scenario, "plants");
  const json_t* j_timeline = json_object_get(j_scenario, "timeline");
  const json_t* j_zones = json_object_get(j_scenario, "zones");
  cmp_ok(json_array_size(j_links), "==", 1, "size of array \"links\" is 1");
  const json_t* j_link = json_array_get(j_links, 0);
  json_t* j_original_link = link_to_json(link);
  ok(json_equal(j_link, j_original_link),
     "link and original link are equal as json values");
  cmp_ok(json_array_size(j_plants), "==", 0, "size of array \"plants\" is 0");
  json_t* j_original_timeline = timeline_to_json(timeline);
  ok(json_equal(j_timeline, j_original_timeline),
     "timeline and original timeline are equal as json values");
  cmp_ok(json_array_size(j_zones), "==", 2, "size of array \"zones\" is 2");
  const json_t* j_source_zone = json_array_get(j_zones, 0);
  json_t* j_original_source_zone = zone_to_json(source_zone);
  ok(json_equal(j_source_zone, j_original_source_zone),
     "source zone and original source zone are equal as json values");
  const json_t* j_target_zone = json_array_get(j_zones, 1);
  json_t* j_original_target_zone = zone_to_json(target_zone);
  ok(json_equal(j_target_zone, j_original_target_zone),
     "target zone and original target zone are equal as json values");

  json_decref(j_original_link);
  json_decref(j_original_timeline);
  json_decref(j_original_source_zone);
  json_decref(j_original_target_zone);
  json_decref(j_scenario);
  scenario_with_link_and_zones_example_free(&example);
}

/**
 * Tests the scenario_from_json function on an example of scenario with link
 * and zones
 */
void test_scenario_with_link_and_zones_from_json(void) {
  diag("Testing scenario_from_json");
  struct ScenarioWithLinkAndZonesExample example;
  scenario_with_link_and_zones_example_initialize(&example);
  const struct Scenario* scenario = &example.scenario;

  json_t* j_scenario = scenario_to_json(scenario);
  struct Scenario scenario_from_j;
  scenario_from_json(&scenario_from_j, j_scenario);
  ok(scenario_are_equal(scenario, &scenario_from_j),
     "manually built scenario and JSON scenario are equal");

  json_decref(j_scenario);
  scenario_free(&scenario_from_j);
  scenario_with_link_and_zones_example_free(&example);
}

/**
 * Tests a scenario with zone
 */
void test_scenario_with_link_and_zones(void) {
  test_scenario_add_zone();
  test_scenario_with_link_and_zones_to_json();
  test_scenario_with_link_and_zones_from_json();
}

// Scenario with plant and zone
// ============================

// A scenario with plant and zone
struct ScenarioWithPlantAndZoneExample {
  struct Plant plant;       // The plant
  struct Scenario scenario; // The scenario
  struct Timeline timeline; // The timeline
  struct Zone zone;         // The zone
};

/**
 * Initializes an example of a scenario with plant and zone
 *
 * @param example  The example to initialize
 */
void scenario_with_plant_and_zone_initialize(
  struct ScenarioWithPlantAndZoneExample* example) {
  diag("Building a scenario with one plant and one zone");
  int durations[] = {10, 30, 60};
  timeline_initialize(&example->timeline, 3, durations);
  mw expected_demands[] = {5.0, 6.0, 7.0};
  zone_initialize(&example->zone, "Z", &example->timeline, expected_demands);
  mw min_powers[] = {1.0, 2.0, 3.0},
     max_powers[] = {7.0, 8.0, 9.0};
  plant_initialize(&example->plant,
                   "P",
                   &example->timeline,
                   &example->zone,
                   min_powers,
                   max_powers);
  scenario_initialize(&example->scenario, &example->timeline);
  scenario_add_zone(&example->scenario, &example->zone);
  scenario_add_plant(&example->scenario, &example->plant);
}

/**
 * Frees a one zone scenario
 *
 * @param example  The example to free
 */
void scenario_with_plant_and_zone_free(
  struct ScenarioWithPlantAndZoneExample* example) {
  scenario_free(&example->scenario);
  plant_free(&example->plant);
  zone_free(&example->zone);
  timeline_free(&example->timeline);
}
/**
 * Tests the scenario_add_plant function on a scenario with plant and zone
 */
void test_scenario_add_plant(void) {
  diag("Testing scenario_add_plant");
  struct ScenarioWithPlantAndZoneExample example;
  scenario_with_plant_and_zone_initialize(&example);
  const struct Plant* plant = &example.plant;
  const struct Scenario* scenario = &example.scenario;
  const struct Timeline* timeline = &example.timeline;
  const struct Zone* zone = &example.zone;

  cmp_ok(scenario->num_plants, "==", 1,
         "number of plants in scenario is 1");
  ok(plant_are_equal(&scenario->plants[0], plant),
     "first scenario plant is equal to the provided plant");
  cmp_ok(scenario->num_zones, "==", 1,
         "number of zones in scenario is 1");
  ok(zone_are_equal(&scenario->zones[0], zone),
     "first scenario zone is equal to the provided zone");

  scenario_with_plant_and_zone_free(&example);
}

/**
 * Tests the scenario_to_json function on an example of scenario with plant and
 * zone
 */
void test_scenario_with_plant_and_zone_to_json(void) {
  diag("Testing scenario_to_json");
  struct ScenarioWithPlantAndZoneExample example;
  scenario_with_plant_and_zone_initialize(&example);
  const struct Plant* plant = &example.plant;
  const struct Timeline* timeline = &example.timeline;
  const struct Scenario* scenario = &example.scenario;
  const struct Zone* zone = &example.zone;

  json_t* j_scenario = scenario_to_json(scenario);
  test_scenario_json_shape(j_scenario);
  const json_t* j_plants = json_object_get(j_scenario, "plants");
  const json_t* j_timeline = json_object_get(j_scenario, "timeline");
  const json_t* j_zones = json_object_get(j_scenario, "zones");
  cmp_ok(json_array_size(j_plants), "==", 1, "size of array \"plants\" is 0");
  const json_t* j_plant = json_array_get(j_plants, 0);
  json_t* j_source_plant = plant_to_json(plant);
  ok(json_equal(j_plant, j_source_plant),
     "plant and source plant are equal as json values");
  json_t* j_source_timeline = timeline_to_json(timeline);
  ok(json_equal(j_timeline, j_source_timeline),
     "timeline and source timeline are equal as json values");
  cmp_ok(json_array_size(j_zones), "==", 1, "size of array \"zones\" is 1");
  const json_t* j_zone = json_array_get(j_zones, 0);
  json_t* j_source_zone = zone_to_json(zone);
  ok(json_equal(j_zone, j_source_zone),
     "zone and source zone are equal as json values");

  json_decref(j_source_timeline);
  json_decref(j_source_plant);
  json_decref(j_source_zone);
  json_decref(j_scenario);
  scenario_with_plant_and_zone_free(&example);
}

/**
 * Tests the scenario_from_json function on an example of scenario with plant
 * and zone
 */
void test_scenario_with_plant_and_zone_from_json(void) {
  diag("Testing scenario_from_json");
  struct ScenarioWithPlantAndZoneExample example;
  scenario_with_plant_and_zone_initialize(&example);
  const struct Scenario* scenario = &example.scenario;

  json_t* j_scenario = scenario_to_json(scenario);
  struct Scenario scenario_from_j;
  scenario_from_json(&scenario_from_j, j_scenario);
  ok(scenario_are_equal(scenario, &scenario_from_j),
     "manually built scenario and JSON scenario are equal");
  json_decref(j_scenario);

  scenario_free(&scenario_from_j);
  scenario_with_plant_and_zone_free(&example);
}

/**
 * Tests a scenario with one plant and zone
 */
void test_scenario_with_plant_and_zone(void) {
  test_scenario_add_plant();
  test_scenario_with_plant_and_zone_to_json();
  test_scenario_with_plant_and_zone_from_json();
}

// Main
// ====

int main(void) {
  test_scenario_are_equal();
  test_empty_scenario();
  test_scenario_with_zone();
  test_scenario_with_link_and_zones();
  test_scenario_with_plant_and_zone();
  done_testing();
}
