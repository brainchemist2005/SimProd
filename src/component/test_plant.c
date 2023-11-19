#include "plant.h"

#include <stdlib.h>

#include <tap.h>

#include "timeline.h"
#include "zone.h"

/**
 * Tests the plant_initialize function
 */
void test_plant_initialize(void) {
  diag("Testing plant_initialize");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);
  mw min_powers[] = {2.0, 3.0, 4.0};
  mw max_powers[] = {7.0, 8.0, 9.0};
  struct Plant plant;
  plant_initialize(&plant, "P", &timeline, &zone, min_powers, max_powers);

  // Checks
  is(plant.id, "P", "plant identifier is \"P\"");
  ok(timeline_are_equal(plant.timeline, &timeline),
     "plant timeline is equal to provided timeline");
  ok(zone_are_equal(plant.zone, &zone),
     "plant zone is equal to provided zone");
  for (int t = 0; t <= 2; ++t) {
    cmp_ok(plant.min_powers[t], "==", min_powers[t],
           "minimum power at index %d of plant is equal to provided power", t);
    cmp_ok(plant.max_powers[t], "==", max_powers[t],
           "maximum power at index %d of plant is equal to provided power", t);
  }

  // Teardown
  plant_free(&plant);
  zone_free(&zone);
  timeline_free(&timeline);
}

/**
 * Tests the plant_are_equal function
 */
void test_plant_are_equal(void) {
  diag("Testing plant_are_equal");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline1, timeline2;
  timeline_initialize(&timeline1, 3, durations);
  timeline_initialize(&timeline2, 2, durations);
  mw expected_demands[] = {1.0, 2.0, 3.0};
  mw min_powers1[] = {4.0, 5.0, 6.0},
     min_powers2[] = {4.0, 5.0, 7.0},
     max_powers1[] = {7.0, 8.0, 9.0},
     max_powers2[] = {7.0, 9.0, 9.0};
  struct Zone zone1, zone2;
  zone_initialize(&zone1, "Z1", &timeline1, expected_demands);
  zone_initialize(&zone2, "Z2", &timeline1, expected_demands);
  struct Plant plant1, plant2, plant3, plant4, plant5, plant6, plant7;
  plant_initialize(&plant1, "P1", &timeline1, &zone1, min_powers1, max_powers1);
  plant_initialize(&plant2, "P1", &timeline1, &zone1, min_powers1, max_powers1);
  plant_initialize(&plant3, "P3", &timeline1, &zone1, min_powers1, max_powers1);
  plant_initialize(&plant4, "P1", &timeline2, &zone1, min_powers1, max_powers1);
  plant_initialize(&plant5, "P1", &timeline1, &zone2, min_powers1, max_powers1);
  plant_initialize(&plant6, "P1", &timeline1, &zone1, min_powers2, max_powers1);
  plant_initialize(&plant7, "P1", &timeline1, &zone1, min_powers1, max_powers2);

  // Test cases
  struct test_case {
    const struct Plant *plant1;
    const struct Plant *plant2;
    bool equal;
  };
  struct test_case test_cases[] = {
    {&plant1, &plant1, true},
    {&plant1, &plant2, true},
    {&plant1, &plant3, false},
    {&plant1, &plant4, false},
    {&plant1, &plant5, false},
    {&plant1, &plant6, false},
    {&plant1, &plant7, false},
  };
  int ntc = sizeof(test_cases) / sizeof(struct test_case);

  // Checks
  for (int tc = 0; tc < ntc; ++tc) {
    ok(plant_are_equal(test_cases[tc].plant1, test_cases[tc].plant2)
       == test_cases[tc].equal,
       "plants with identifiers %s and %s are %s",
       test_cases[tc].plant1,
       test_cases[tc].plant2,
       test_cases[tc].equal ? "equal" : "not equal");
  }

  // Teardown
  plant_free(&plant1);
  plant_free(&plant2);
  plant_free(&plant3);
  plant_free(&plant4);
  plant_free(&plant5);
  plant_free(&plant6);
  plant_free(&plant7);
  zone_free(&zone1);
  zone_free(&zone2);
  timeline_free(&timeline1);
  timeline_free(&timeline2);
}

/**
 * Tests the plant_copy function
 */
void test_plant_copy(void) {
  diag("Testing plant_copy");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);
  mw min_powers[] = {2.0, 3.0, 4.0};
  mw max_powers[] = {7.0, 8.0, 9.0};
  struct Plant plant1, plant2;
  plant_initialize(&plant1, "P", &timeline, &zone, min_powers, max_powers);
  plant_copy(&plant2, &plant1);

  // Checks
  ok(plant_are_equal(&plant1, &plant2),
     "copied plant is equal to original plant");

  // Teardown
  zone_free(&zone);
  plant_free(&plant1);
  plant_free(&plant2);
  timeline_free(&timeline);
}

/**
 * Tests the plant_to_json function
 */
void test_plant_to_json(void) {
  diag("Testing plant_to_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);
  mw min_powers[] = {2.0, 3.0, 4.0},
     max_powers[] = {7.0, 8.0, 9.0};
  struct Plant plant;
  plant_initialize(&plant, "P", &timeline, &zone, min_powers, max_powers);

  // Checks
  json_t* j = plant_to_json(&plant);
  ok(json_is_object(j), "json value is an object");
  cmp_ok(json_object_size(j), "==", 4, "json object has size 4");
  const json_t* j_id = json_object_get(j, "id");
  const json_t* j_min_powers = json_object_get(j, "min-powers");
  const json_t* j_max_powers = json_object_get(j, "max-powers");
  const json_t* j_zone = json_object_get(j, "zone");
  ok(j_id != NULL, "json value has key \"id\"");
  ok(j_min_powers != NULL, "json value has key \"min-powers\"");
  ok(j_max_powers != NULL, "json value has key \"max-powers\"");
  ok(j_zone != NULL, "json value has key \"zone\"");
  ok(json_is_string(j_id), "value associated with \"id\" is string");
  is(json_string_value(j_id), "P", "value associated with \"id\" is \"P\"");
  ok(json_is_array(j_min_powers),
     "value associated with \"min-powers\" is array");
  ok(json_is_array(j_max_powers),
     "value associated with \"max-powers\" is array");
  for (int t = 0; t <= 2; ++t) {
    const json_t* j_min_power = json_array_get(j_min_powers, t);
    const json_t* j_max_power = json_array_get(j_max_powers, t);
    ok(json_is_real(j_min_power),
       "value at index %d of \"min-powers\" is a real number");
    ok(json_is_real(j_max_power),
       "value at index %d of \"max-powers\" is a real number");
    ok(json_number_value(j_min_power) == min_powers[t],
       "value at index %d of \"min-powers\" is %f", t, min_powers[t]);
    ok(json_number_value(j_max_power) == max_powers[t],
       "value at index %d of \"max-powers\" is %f", t, max_powers[t]);
  }
  ok(json_is_string(j_zone), "value associated with \"zone\" is string");
  is(json_string_value(j_zone), "Z", "value associated with \"zone\" is \"Z\"");

  // Teardown
  json_decref(j);
  plant_free(&plant);
  zone_free(&zone);
  timeline_free(&timeline);
}

/**
 * Tests the plant_from_json function
 */
void test_plant_from_json(void) {
  diag("Testing plant_from_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);
  mw min_powers[] = {2.0, 3.0, 4.0};
  mw max_powers[] = {7.0, 8.0, 9.0};
  struct Plant plant, json_plant;
  plant_initialize(&plant, "P", &timeline, &zone, min_powers, max_powers);
  json_t* j = plant_to_json(&plant);
  plant_from_json(&json_plant, &timeline, &zone, j);

  // Checks
  ok(plant_are_equal(&plant, &json_plant),
     "manually built plant and JSON plant are equal");

  // Teardown
  json_decref(j);
  plant_free(&plant);
  plant_free(&json_plant);
  zone_free(&zone);
  timeline_free(&timeline);
}

int main(void) {
  test_plant_initialize();
  test_plant_are_equal();
  test_plant_to_json();
  test_plant_from_json();
  done_testing();
}
