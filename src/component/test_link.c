#include "link.h"

#include <stdlib.h>

#include <tap.h>

#include "component/zone.h"
#include "timeline.h"

/**
 * Tests the link_initialize function
 */
void test_link_initialize(void) {
  diag("Testing link_initialize");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone1, zone2;
  zone_initialize(&zone1, "Z1", &timeline, expected_demands);
  zone_initialize(&zone2, "Z2", &timeline, expected_demands);
  struct Link link;
  link_initialize(&link, "Z1->Z2", &zone1, &zone2);

  // Checks
  is(link.id, "Z1->Z2", "zone identifier is \"Z1->Z2\"");
  ok(zone_are_equal(link.source, &zone1),
     "source zone of link is equal to zone Z1");
  ok(zone_are_equal(link.target, &zone2),
     "target zone of link is equal to zone Z2");

  // Teardown
  link_free(&link);
  zone_free(&zone1);
  zone_free(&zone2);
  timeline_free(&timeline);
}

/**
 * Tests the link_are_equal function
 */
void test_link_are_equal(void) {
  diag("Testing link_are_equal");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone1, zone2, zone3;
  zone_initialize(&zone1, "Z1", &timeline, expected_demands);
  zone_initialize(&zone2, "Z2", &timeline, expected_demands);
  zone_initialize(&zone3, "Z3", &timeline, expected_demands);
  struct Link link1, link2, link3, link4;
  link_initialize(&link1, "L", &zone1, &zone2);
  link_initialize(&link2, "L'", &zone1, &zone2);
  link_initialize(&link3, "L", &zone1, &zone3);
  link_initialize(&link4, "L", &zone3, &zone1);

  // Test cases
  struct test_case {
    const struct Link *link1;
    const struct Link *link2;
    bool equal;
  };
  struct test_case test_cases[] = {
    {&link1, &link1, true},
    {&link1, &link2, false},
    {&link1, &link3, false},
    {&link1, &link4, false},
  };
  int ntc = sizeof(test_cases) / sizeof(struct test_case);

  // Checks
  for (int tc = 0; tc < ntc; ++tc)
    cmp_ok(link_are_equal(test_cases[tc].link1, test_cases[tc].link2), "==",
           test_cases[tc].equal,
           "links with identifiers %s and %s are %s",
           test_cases[tc].link1,
           test_cases[tc].link2,
           test_cases[tc].equal ? "equal" : "not equal");

  // Teardown
  zone_free(&zone1);
  zone_free(&zone2);
  zone_free(&zone3);
  link_free(&link1);
  link_free(&link2);
  timeline_free(&timeline);
}

/**
 * Tests the link_to_json function
 */
void test_link_to_json(void) {
  diag("Testing link_to_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone1, zone2;
  zone_initialize(&zone1, "Z1", &timeline, expected_demands);
  zone_initialize(&zone2, "Z2", &timeline, expected_demands);
  struct Link link;
  link_initialize(&link, "Z1->Z2", &zone1, &zone2);
  json_t* j = link_to_json(&link);

  // Checks
  ok(json_is_object(j), "json value is an object");
  cmp_ok(json_object_size(j), "==", 3, "json object has size 3");
  const json_t* j_id = json_object_get(j, "id");
  const json_t* j_source = json_object_get(j, "source");
  const json_t* j_target = json_object_get(j, "target");
  ok(j_id != NULL, "json value has key \"id\"");
  ok(j_source != NULL, "json value has key \"source\"");
  ok(j_target != NULL, "json value has key \"target\"");
  is(json_string_value(j_id), "Z1->Z2",
     "value associated with \"id\" is \"Z1->Z2\"");
  is(json_string_value(j_source), "Z1",
     "value associated with \"source\" is \"Z1\"");
  is(json_string_value(j_target), "Z2",
     "value associated with \"target\" is \"Z2\"");

  // Teardown
  json_decref(j);
  link_free(&link);
  zone_free(&zone1);
  zone_free(&zone2);
  timeline_free(&timeline);
}

/**
 * Tests the link_from_json function
 */
void test_link_from_json(void) {
  diag("Testing link_from_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);
  mw expected_demands[] = {5.0, 10.0, 8.0};
  struct Zone zone1, zone2;
  zone_initialize(&zone1, "Z1", &timeline, expected_demands);
  zone_initialize(&zone2, "Z2", &timeline, expected_demands);
  struct Link link, json_link;
  link_initialize(&link, "Z1->Z2", &zone1, &zone2);
  json_t* j = link_to_json(&link);
  link_from_json(&json_link, &zone1, &zone2, j);

  // Checks
  ok(link_are_equal(&link, &json_link),
     "manually built link and JSON link are equal");

  // Teardown
  json_decref(j);
  link_free(&link);
  link_free(&json_link);
  zone_free(&zone1);
  zone_free(&zone2);
  timeline_free(&timeline);
}

int main(void) {
  test_link_initialize();
  test_link_are_equal();
  test_link_to_json();
  test_link_from_json();
  done_testing();
}
