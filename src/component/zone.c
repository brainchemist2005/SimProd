#include "zone.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timeline.h"
#include "validation.h"

// Initialization
// --------------

void zone_initialize(struct Zone* zone,
                     const char* id,
                     const struct Timeline* timeline,
                     const mw* expected_demands) {
  strncpy(zone->id, id, ID_MAX_LENGTH);
  zone->timeline = timeline;
  zone->expected_demands = malloc(timeline->num_future_timesteps * sizeof(mw));
  memcpy(zone->expected_demands,
         expected_demands,
         timeline->num_future_timesteps * sizeof(mw));
}

void zone_copy(struct Zone* dest, const struct Zone* src) {
  zone_initialize(dest, src->id, src->timeline, src->expected_demands);
}

void zone_from_json(struct Zone* zone,
                    const struct Timeline* timeline,
                    json_t* j) {
  ensure_json_is_object(j);
  ensure_json_object_has_size(j, 2);
  ensure_json_object_contains_key(j, JSON_ZONE_ID);
  ensure_json_object_contains_key(j, JSON_ZONE_EXPECTED_DEMANDS);
  const json_t* j_id = json_object_get(j, JSON_ZONE_ID);
  ensure_json_is_string(j_id);
  const json_t* j_demands = json_object_get(j, JSON_ZONE_EXPECTED_DEMANDS);
  ensure_json_is_array(j_demands);
  ensure_json_array_has_size(j_demands, timeline->num_future_timesteps);
  ensure_json_is_array_of_numbers(j_demands);
  const char* id = json_string_value(json_object_get(j, JSON_ZONE_ID));
  mw expected_demands[timeline->num_future_timesteps];
  for (int t = 0; t < timeline->num_future_timesteps; ++t)
    expected_demands[t] = json_number_value(json_array_get(j_demands, t));
  zone_initialize(zone, id, timeline, expected_demands);
}

// Destruction
// -----------

void zone_free(struct Zone* zone) {
  free(zone->expected_demands);
}

// Accessors
// ---------

bool zone_are_equal(const struct Zone* zone1, const struct Zone* zone2) {
  if (strcmp(zone1->id, zone2->id) != 0)
    return false;
  if (!timeline_are_equal(zone1->timeline, zone2->timeline))
    return false;
  for (int t = 0; t < zone1->timeline->num_future_timesteps; ++t)
    if (zone1->expected_demands[t] != zone2->expected_demands[t])
      return false;
  return true;
}

void zone_print(const struct Zone* zone) {
  printf("A zone with identifier \"%s\"\n", zone->id);
  printf("  Expected demands: ");
  for (int t = 0; t < zone->timeline->num_future_timesteps; ++t) {
    if (t > 0) printf(", ");
    printf("%f", zone->expected_demands[t]);
  }
  printf("\n");
}

// JSON serialization
// ------------------

json_t* zone_to_json(const struct Zone* zone) {
  json_t* jdemands = json_array();
  for (int t = 0; t < zone->timeline->num_future_timesteps; ++t)
    json_array_append_new(jdemands, json_real(zone->expected_demands[t]));
  return json_pack("{s:o,s:s}",
                   JSON_ZONE_EXPECTED_DEMANDS, jdemands,
                   JSON_ZONE_ID, zone->id);
}
