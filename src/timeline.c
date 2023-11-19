#include "timeline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validation.h"

// Creation
// --------

void timeline_initialize(struct Timeline* timeline,
                         unsigned int num_future_timesteps,
                         const int* future_durations) {
  timeline->num_future_timesteps = num_future_timesteps;
  timeline->future_durations = malloc(num_future_timesteps * sizeof(int));
  memcpy(timeline->future_durations,
         future_durations,
         num_future_timesteps * sizeof(int));
}

void timeline_copy(struct Timeline* dest, const struct Timeline* src) {
  timeline_initialize(dest, src->num_future_timesteps, src->future_durations);
}

void timeline_from_json(struct Timeline* timeline,
                        json_t* j) {
  ensure_json_is_object(j);
  ensure_json_object_has_size(j, 1);
  ensure_json_object_contains_key(j, JSON_TIMELINE_FUTURE_DURATIONS);
  const json_t* j_future_durations =
    json_object_get(j, JSON_TIMELINE_FUTURE_DURATIONS);
  ensure_json_is_array_of_integers(j_future_durations);
  unsigned int num_future_timesteps = json_array_size(j_future_durations);
  int future_durations[num_future_timesteps];
  for (int t = 0; t < num_future_timesteps; ++t)
    future_durations[t] =
      json_integer_value(json_array_get(j_future_durations, t));
  timeline_initialize(timeline, num_future_timesteps, future_durations);
}

// Destruction
// -----------

void timeline_free(struct Timeline* timeline) {
  free(timeline->future_durations);
}

// Accessors
// ---------

bool timeline_are_equal(const struct Timeline* timeline1,
                        const struct Timeline* timeline2) {
  if (timeline1->num_future_timesteps != timeline2->num_future_timesteps)
    return false;
  for (int t = 0; t < timeline1->num_future_timesteps; ++t)
    if (timeline1->future_durations[t] != timeline2->future_durations[t])
      return false;
  return true;
}

void timeline_print(const struct Timeline* timeline) {
  printf("A timeline of %d future timesteps with durations ",
         timeline->num_future_timesteps);
  for (int t = 0; t < timeline->num_future_timesteps; ++t) {
    if (t > 0) printf(", ");
    printf("%dmin", timeline->future_durations[t]);
  }
  printf("\n");
}

// JSON serialization
// ------------------

json_t* timeline_to_json(const struct Timeline* timeline) {
  json_t* j = json_array();
  for (int t = 0; t < timeline->num_future_timesteps; ++t)
    json_array_append_new(j, json_integer(timeline->future_durations[t]));
  return json_pack("{s:o}", "future-durations", j);
}
