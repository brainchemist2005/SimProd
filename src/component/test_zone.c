#include "zone.h"

#include <stdio.h>
#include <stdlib.h>

#include <tap.h>

#include "timeline.h"

/**
 * Tests the zone_initialize function
 */
void test_zone_initialize(void) {
  diag("Testing zone_initialize");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);

  // Checks
  is(zone.id, "Z", "zone identifier is \"Z\"");
  cmp_ok(zone.expected_demands[0], "==", expected_demands[0],
         "first expected demand of zone is equal to provided demand");
  cmp_ok(zone.expected_demands[1], "==", expected_demands[1],
         "second expected demand of zone is equal to provided demand");
  cmp_ok(zone.expected_demands[2], "==", expected_demands[2],
         "third expected demand of zone is equal to provided demand");
  ok(timeline_are_equal(zone.timeline, &timeline),
     "zone timeline is equal to provided timeline");

  // Teardown
  zone_free(&zone);
  timeline_free(&timeline);
}

/**
 * Tests the zone_are_equal function
 */
void test_zone_are_equal(void) {
  diag("Testing zone_are_equal");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline1, timeline2;
  timeline_initialize(&timeline1, 2, durations);
  timeline_initialize(&timeline2, 3, durations);
  mw expected_demands1[] = {5.0, 10.0, 8.0},
     expected_demands2[] = {5.0, 6.0, 8.0};
  struct Zone zone1, zone2, zone3, zone4, zone5;
  zone_initialize(&zone1, "Z", &timeline1, expected_demands1);
  zone_initialize(&zone2, "Z", &timeline1, expected_demands1);
  zone_initialize(&zone3, "Z'", &timeline1, expected_demands1);
  zone_initialize(&zone4, "Z'", &timeline2, expected_demands1);
  zone_initialize(&zone5, "Z'", &timeline1, expected_demands2);

  // Test cases
  struct test_case {
    const struct Zone *zone1;
    const struct Zone *zone2;
    bool equal;
  };
  struct test_case test_cases[] = {
    {&zone1, &zone1, true},
    {&zone1, &zone2, true},
    {&zone1, &zone3, false},
    {&zone1, &zone4, false},
    {&zone1, &zone5, false},
  };
  int ntc = sizeof(test_cases) / sizeof(struct test_case);

  // Checks
  for (int tc = 0; tc < ntc; ++tc)
    cmp_ok(zone_are_equal(test_cases[tc].zone1, test_cases[tc].zone2), "==",
           test_cases[tc].equal,
           "zones with identifiers %s and %s are %s",
           test_cases[tc].zone1,
           test_cases[tc].zone2,
           test_cases[tc].equal ? "equal" : "not equal");

  // Teardown
  zone_free(&zone1);
  zone_free(&zone2);
  zone_free(&zone3);
  zone_free(&zone4);
  zone_free(&zone5);
  timeline_free(&timeline1);
  timeline_free(&timeline2);
}

/**
 * Tests the zone_copy function
 */
void test_zone_copy(void) {
  diag("Testing zone_copy");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone1, zone2;
  zone_initialize(&zone1, "Z", &timeline, expected_demands);
  zone_copy(&zone2, &zone1);

  // Checks
  ok(zone_are_equal(&zone1, &zone2), "copied zone is equal to original zone");

  // Teardown
  zone_free(&zone1);
  zone_free(&zone2);
  timeline_free(&timeline);
}

/**
 * Tests the zone_to_json function
 */
void test_zone_to_json(void) {
  diag("Testing zone_to_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);
  json_t* j = zone_to_json(&zone);

  // Checks
  ok(json_is_object(j), "json value is an object");
  cmp_ok(json_object_size(j), "==", 2, "json object has size 2");
  const json_t* j_id = json_object_get(j, JSON_ZONE_ID);
  const json_t* j_expected_demands = json_object_get(j, "expected-demands");
  ok(j_id != NULL, "json value has key \"id\"");
  ok(j_expected_demands != NULL, "json value has key \"expected-demands\"");
  ok(json_is_string(j_id), "value associated with \"id\" is string");
  is(json_string_value(j_id), "Z", "value associated with \"id\" is \"Z\"");
  ok(json_is_array(j_expected_demands),
     "value associated with \"expected-demands\" is array");
  cmp_ok(json_array_size(j_expected_demands), "==", 3,
         "size of array \"expected-demands\" is 3");
  for (int t = 0; t <= 2; ++t) {
    const json_t* j_demand = json_array_get(j_expected_demands, t);
    ok(json_is_real(j_demand),
       "value at index %d of \"expected demands\" is a real number", t);
    ok(json_number_value(j_demand) == expected_demands[t],
       "value at index %d of \"expected demands\" is %f",
       t, expected_demands[t]);
  }

  // Teardown
  json_decref(j);
  zone_free(&zone);
  timeline_free(&timeline);
}

/**
 * Tests the zone_from_json function
 */
void test_zone_from_json(void) {
  diag("Testing zone_from_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone, json_zone;
  zone_initialize(&zone, "Z", &timeline, expected_demands);
  json_t* j = zone_to_json(&zone);
  zone_from_json(&json_zone, &timeline, j);

  // Checks
  ok(zone_are_equal(&zone, &json_zone),
     "manually built zone and JSON zone are equal");

  // Teardown
  json_decref(j);
  zone_free(&zone);
  zone_free(&json_zone);
  timeline_free(&timeline);
}

int main(void) {
  test_zone_initialize();
  test_zone_are_equal();
  test_zone_copy();
  test_zone_to_json();
  test_zone_from_json();
  done_testing();
}
