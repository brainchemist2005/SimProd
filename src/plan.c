#include "plan.h"

#include "validation.h"

// Initialization
// --------------

void plan_initialize(struct Plan* plan,
                     const struct Timeline* timeline) {
  timeline_copy(&plan->timeline, timeline);
  plan->productions =
    malloc(timeline->num_future_timesteps * sizeof(struct Treemap));
  for (int t = 0; t < timeline->num_future_timesteps; ++t)
    treemap_initialize(plan->productions + t);
}

void plan_from_json(struct Plan* plan, json_t* j) {
  ensure_json_is_object(j);
  ensure_json_object_contains_key(j, JSON_PLAN_TIMELINE);
  json_t* j_timeline = json_object_get(j, JSON_PLAN_TIMELINE);
  struct Timeline timeline;
  timeline_from_json(&timeline, j_timeline);
  plan_initialize(plan, &timeline);
  timeline_free(&timeline);
  json_t* j_productions = json_object_get(j, JSON_PLAN_PRODUCTIONS);
  ensure_json_is_object(j_productions);
  const char* plant_id;
  json_t* j_plant_productions;
  json_object_foreach(j_productions, plant_id, j_plant_productions) {
    ensure_json_is_array(j_plant_productions);
    for (int t = 0; t < json_array_size(j_plant_productions); ++t)
      plan_set_production(plan, t, plant_id,
                          json_real_value(json_array_get(j_plant_productions, t)));
  }
}

// Destruction
// -----------

void plan_free(struct Plan* plan) {
  for (int t = 0; t < plan->timeline.num_future_timesteps; ++t)
    treemap_delete(plan->productions + t);
  free(plan->productions);
  timeline_free(&plan->timeline);
}

// Modifiers
// ---------

void plan_set_production(struct Plan* plan,
                         int t,
                         const char* id,
                         mw production) {
  treemap_set(plan->productions + t, id, production);
}

// Accessors
// ---------

mw plan_get_production(struct Plan* plan,
                       int t,
                       const char* id) {
  return treemap_get(plan->productions + t, id);
}

bool plan_are_equal(const struct Plan* plan1, const struct Plan* plan2) {
  if (!timeline_are_equal(&plan1->timeline, &plan2->timeline))
    return false;
  for (int t = 0; t < plan1->timeline.num_future_timesteps; ++t)
    if (!treemap_are_equal(plan1->productions + t, plan2->productions + t))
      return false;
  return true;
}

// JSON serialization
// ------------------

json_t* plan_to_json(const struct Plan* plan) {
  json_t* j_productions = json_object();
  if (plan->timeline.num_future_timesteps > 0) {
    struct StringArray sa;
    treemap_compute_keys(plan->productions, &sa);
    for (int p = 0; p < sa.size; ++p) {
      json_t* j_plant_productions = json_array();
      for (int t = 0; t < plan->timeline.num_future_timesteps; ++t) {
        double production = 0.0;
        if (treemap_has_key(plan->productions + t, sa.strings[p]))
          production = treemap_get(plan->productions + t, sa.strings[p]);
        json_array_append(j_plant_productions, json_real(production));
      }
      json_object_set(j_productions, sa.strings[p], j_plant_productions);
      json_decref(j_plant_productions);
    }
    string_array_delete(&sa);
  }
  return json_pack("{s:o,s:o}",
                   "productions",
                   j_productions,
                   "timeline",
                   timeline_to_json(&plan->timeline));
}
