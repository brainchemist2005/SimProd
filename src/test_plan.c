#include "plan.h"

#include <string.h>

#include <tap.h>

#include "scenario.h"

// JSON verification helpers
// =========================

/**
 * Checks that the JSON shape of a JSON plan is as expected
 *
 * @param j_plan  The JSON value of a plan
 */
void test_plan_json_shape(const json_t* j_plan) {
  ok(json_is_object(j_plan), "json value is an object");
  cmp_ok(json_object_size(j_plan), "==", 2, "json object has size 2");
  const json_t* j_productions = json_object_get(j_plan, "productions");
  const json_t* j_timeline = json_object_get(j_plan, "timeline");
  ok(j_productions != NULL, "json value has key \"productions\"");
  ok(j_timeline != NULL, "json value has key \"timeline\"");
  ok(json_is_object(j_productions), "value associated with \"productions\" is object");
  ok(json_is_object(j_timeline), "value associated with \"timeline\" is object");
}

// Empty plan
// ==========

// An example of an empty plan
struct EmptyPlanExample {
  struct Plan plan;         // The scenario
  struct Timeline timeline; // The timeline
};

/**
 * Initializes an example of an empty plan
 *
 * @param example  The example to initialize
 */
void empty_plan_example_initialize(struct EmptyPlanExample* example) {
  diag("Building an empty plan");
  int durations[] = {10, 30, 60};
  timeline_initialize(&example->timeline, 3, durations);
  plan_initialize(&example->plan, &example->timeline);
}

/**
 * Frees an example of an empty plan
 *
 * @param example  The example to free
 */
void empty_plan_example_free(struct EmptyPlanExample* example) {
  plan_free(&example->plan);
  timeline_free(&example->timeline);
}

/**
 * Tests the plan_initialize function on an example with an empty plan
 */
void test_empty_plan_initialize(void) {
  diag("Testing plan_initialize");
  struct EmptyPlanExample example;
  empty_plan_example_initialize(&example);
  const struct Plan* plan = &example.plan;
  const struct Timeline* timeline = &example.timeline;

  ok(timeline_are_equal(&plan->timeline, timeline),
     "plan timeline is equal to the provided timeline");

  empty_plan_example_free(&example);
}

/**
 * Tests the plan_to_json function on an example with an empty plan
 */
void test_empty_plan_to_json(void) {
  diag("Testing plan_to_json");
  struct EmptyPlanExample example;
  empty_plan_example_initialize(&example);
  const struct Plan* plan = &example.plan;
  const struct Timeline* timeline = &example.timeline;

  // Checks
  json_t* j = plan_to_json(plan);
  test_plan_json_shape(j);
  const json_t* j_timeline = json_object_get(j, "timeline");
  json_t* timeline_to_j = timeline_to_json(timeline);
  ok(json_equal(j_timeline, timeline_to_j),
     "json value of timeline equals json timeline value in plan");
  const json_t* j_productions = json_object_get(j, "productions");
  cmp_ok(json_object_size(j_productions), "==", 0,
         "size of array \"productions\" is 0");

  // Teardown
  json_decref(j);
  json_decref(timeline_to_j);
  empty_plan_example_free(&example);
}

/**
 * Tests the plan_from_json function on an example with an empty plan
 */
void test_empty_plan_from_json(void) {
  diag("Testing plan_from_json");
  struct EmptyPlanExample example;
  empty_plan_example_initialize(&example);
  const struct Plan* plan = &example.plan;
  const struct Timeline* timeline = &example.timeline;
  struct Plan json_plan;
  json_t* j = plan_to_json(plan);
  plan_from_json(&json_plan, j);

  // Checks
  ok(plan_are_equal(plan, &json_plan),
     "manually built plan and JSON plan are equal");

  // Teardown
  json_decref(j);
  plan_free(&json_plan);
  empty_plan_example_free(&example);
}

/**
 * Tests an example with an empty plan
 */
void test_empty_plan(void) {
  test_empty_plan_initialize();
  test_empty_plan_to_json();
  test_empty_plan_from_json();
}

// Plan with productions
// =====================

// An example of a plan with productions
struct PlanWithProductionsExample {
  struct Plan plan;         // The plan
  char plant1_id[5];        // The identifier of the first plant
  char plant2_id[5];        // The identifier of the second plant
  mw productions_plant1[3]; // The productions of the first plant
  mw productions_plant2[3]; // The productions of the second plant
  struct Timeline timeline; // The timeline
};

/**
 * Initializes an example of a plan with productions
 *
 * @param example  The example to initialize
 */
void plan_with_productions_example_initialize(
  struct PlanWithProductionsExample* example) {
  diag("Building a plan with productions");
  int durations[] = {10, 30, 60};
  timeline_initialize(&example->timeline, 3, durations);
  plan_initialize(&example->plan, &example->timeline);
  strcpy(example->plant1_id, "P1");
  strcpy(example->plant2_id, "P2");
  example->productions_plant1[0] = 1.0;
  example->productions_plant1[1] = 2.0;
  example->productions_plant1[2] = 3.0;
  example->productions_plant2[0] = 4.0;
  example->productions_plant2[1] = 5.0;
  example->productions_plant2[2] = 6.0;
  plan_set_production(&example->plan, 0, "P1", example->productions_plant1[0]);
  plan_set_production(&example->plan, 1, "P1", example->productions_plant1[1]);
  plan_set_production(&example->plan, 2, "P1", example->productions_plant1[2]);
  plan_set_production(&example->plan, 0, "P2", example->productions_plant2[0]);
  plan_set_production(&example->plan, 1, "P2", example->productions_plant2[1]);
  plan_set_production(&example->plan, 2, "P2", example->productions_plant2[2]);
}

/**
 * Frees an example of a plan with productions
 *
 * @param example  The example to free
 */
void plan_with_productions_example_free(
  struct PlanWithProductionsExample* example) {
  plan_free(&example->plan);
  timeline_free(&example->timeline);
}

/**
 * Tests the plan_initialize function on an example of a plan with productions
 */
void test_plan_with_productions_initialize(void) {
  struct PlanWithProductionsExample example;
  plan_with_productions_example_initialize(&example);

  ok(timeline_are_equal(&example.plan.timeline, &example.timeline),
     "plan timeline is equal to the provided timeline");
  for (int t = 0; t < 3; ++t) {
    cmp_ok(plan_get_production(&example.plan, t, "P1"),
           "==",
           example.productions_plant1[t],
           "plan_get_production(P1) == %f",
           example.productions_plant1[t]);
  }
  for (int t = 0; t < 3; ++t) {
    cmp_ok(plan_get_production(&example.plan, t, "P2"),
           "==",
           example.productions_plant2[t],
           "plan_get_production(P2) == %f",
           example.productions_plant2[t]);
  }

  plan_with_productions_example_free(&example);
}

/**
 * Tests the plan_to_json function on an example of plan with productions
 */
void test_plan_with_productions_to_json(void) {
  diag("Testing plan_to_json");
  struct PlanWithProductionsExample example;
  plan_with_productions_example_initialize(&example);
  const struct Plan* plan = &example.plan;
  const struct Timeline* timeline = &example.timeline;

  // Checks
  json_t* j = plan_to_json(plan);
  test_plan_json_shape(j);
  const json_t* j_timeline = json_object_get(j, "timeline");
  json_t* timeline_to_j = timeline_to_json(timeline);
  ok(json_equal(j_timeline, timeline_to_j),
     "json value of timeline equals json timeline value in plan");
  const json_t* j_productions = json_object_get(j, "productions");
  cmp_ok(json_object_size(j_productions), "==", 2,
         "size of j[productions] is 2");
  const json_t* j_plant1 = json_object_get(j_productions, "P1");
  const json_t* j_plant2 = json_object_get(j_productions, "P2");
  ok(j_plant1 != NULL, "j[productions] has key \"P1\"");
  ok(j_plant2 != NULL, "j[productions] has key \"P2\"");
  ok(json_is_array(j_plant1), "j[productions][P1] is array");
  ok(json_is_array(j_plant2), "j[productions][P2] is array");
  cmp_ok(json_array_size(j_plant1), "==", 3, "size of j[productions][P1] is 3");
  cmp_ok(json_array_size(j_plant2), "==", 3, "size of j[productions][P2] is 3");
  for (int t = 0; t < 3; ++t)
    cmp_ok(json_real_value(json_array_get(j_plant1, t)),
           "==",
           example.productions_plant1[t],
           "j[productions][P1] == %f",
           example.productions_plant1[t]);
  for (int t = 0; t < 3; ++t)
    cmp_ok(json_real_value(json_array_get(j_plant2, t)),
           "==",
           example.productions_plant2[t],
           "j[productions][P2] == %f",
           example.productions_plant2[t]);

  // Teardown
  json_decref(j);
  json_decref(timeline_to_j);
  plan_with_productions_example_free(&example);
}

/**
 * Tests the plan_from_json function on an example of plan with productions
 */
void test_plan_with_productions_from_json(void) {
  diag("Testing plan_from_json");
  struct PlanWithProductionsExample example;
  plan_with_productions_example_initialize(&example);
  const struct Plan* plan = &example.plan;
  const struct Timeline* timeline = &example.timeline;
  struct Plan json_plan;
  json_t* j = plan_to_json(plan);
  plan_from_json(&json_plan, j);

  // Checks
  ok(plan_are_equal(plan, &json_plan),
     "manually built plan and JSON plan are equal");

  // Teardown
  json_decref(j);
  plan_free(&json_plan);
  plan_with_productions_example_free(&example);
}

/**
 * Tests an example of plan with productions
 */
void test_plan_with_productions(void) {
  test_plan_with_productions_initialize();
  test_plan_with_productions_to_json();
  test_plan_with_productions_from_json();
}

// Main
// ====

int main(void) {
  test_empty_plan();
  test_plan_with_productions();
  done_testing();
}
