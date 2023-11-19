#include "timeline.h"

#include <stdlib.h>

#include <tap.h>

/**
 * Tests the timeline_initialize function
 */
void test_timeline_initialize(void) {
  diag("Testing timeline_initialize");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);

  // Checks
  cmp_ok(timeline.future_durations[0], "==", 10,
         "first duration of timeline is as given");
  cmp_ok(timeline.future_durations[1], "==", 30,
         "second duration of timeline is as given");
  cmp_ok(timeline.future_durations[2], "==", 60,
         "third duration of timeline is as given");

  // Teardown
  timeline_free(&timeline);
}

/**
 * Tests the timeline_are_equal function
 */
void test_timeline_are_equal(void) {
  diag("Testing timeline_are_equal");

  // Setup
  int durations1[] = {10, 20, 30},
      durations2[] = {10, 30, 60};
  struct Timeline timeline1, timeline2, timeline3;
  timeline_initialize(&timeline1, 3, durations1);
  timeline_initialize(&timeline2, 3, durations1);
  timeline_initialize(&timeline3, 3, durations2);

  // Checks
  ok(timeline_are_equal(&timeline1, &timeline1),
     "timeline is equal to itself");
  ok(timeline_are_equal(&timeline1, &timeline2),
     "timelines with same durations are equal");
  ok(!timeline_are_equal(&timeline1, &timeline3),
     "timelines with different durations are not equal");

  // Teardown
  timeline_free(&timeline1);
  timeline_free(&timeline2);
  timeline_free(&timeline3);
}

/**
 * Tests the timeline_copy function
 */
void test_timeline_copy(void) {
  diag("Testing timeline_copy");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline1, timeline2;
  timeline_initialize(&timeline1, 3, durations);
  timeline_copy(&timeline2, &timeline1);

  // Checks
  ok(timeline_are_equal(&timeline1, &timeline2),
     "copied timeline is equal to original timeline");

  // Teardown
  timeline_free(&timeline1);
  timeline_free(&timeline2);
}

/**
 * Tests the timeline_to_json function
 */
void test_timeline_to_json(void) {
  diag("Testing timeline_to_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline;
  timeline_initialize(&timeline, 3, durations);

  // Checks
  json_t* j = timeline_to_json(&timeline);
  ok(json_is_object(j), "json value is an object");
  cmp_ok(json_object_size(j), "==", 1, "json object has size 1");
  const json_t* j_durations = json_object_get(j, "future-durations");
  ok(j_durations != NULL, "json value has key \"future-durations\"");
  ok(json_is_array(j_durations),
     "value associated with \"future-durations\" is array");
  cmp_ok(json_array_size(j_durations), "==", 3,
         "size of array \"future-durations\" is 3");
  for (int t = 0; t <= 2; ++t) {
    const json_t* j_duration = json_array_get(j_durations, t);
    ok(json_is_integer(j_duration),
       "value at index %d of \"future-durations\" is an integer", t);
    cmp_ok(json_integer_value(j_duration), "==", durations[t],
           "value at index %d of \"future-durations\" is %d", t, durations[t]);
  }

  // Teardown
  json_decref(j);
  timeline_free(&timeline);
}

/**
 * Tests the timeline_from_json function
 */
void test_timeline_from_json(void) {
  diag("Testing timeline_from_json");

  // Setup
  int durations[] = {10, 30, 60};
  struct Timeline timeline, json_timeline;
  timeline_initialize(&timeline, 3, durations);
  json_t* j = timeline_to_json(&timeline);
  timeline_from_json(&json_timeline, j);

  // Checks
  ok(timeline_are_equal(&timeline, &json_timeline),
     "manually built timeline and JSON timeline are equal");

  // Teardown
  json_decref(j);
  timeline_free(&timeline);
  timeline_free(&json_timeline);
}

int main(void) {
  test_timeline_initialize();
  test_timeline_are_equal();
  test_timeline_copy();
  test_timeline_to_json();
  test_timeline_from_json();
  done_testing();
}
