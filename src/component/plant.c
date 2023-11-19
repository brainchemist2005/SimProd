#include "plant.h"

#include <stdlib.h>
#include <string.h>

#include "validation.h"

// Initialization
// --------------

void plant_initialize(struct Plant* plant,
                      const char* id,
                      const struct Timeline* timeline,
                      const struct Zone* zone,
                      const mw* min_powers,
                      const mw* max_powers) {
  strncpy(plant->id, id, ID_MAX_LENGTH);
  plant->timeline = timeline;
  plant->zone = zone;
  plant->min_powers = malloc(timeline->num_future_timesteps * sizeof(mw));
  plant->max_powers = malloc(timeline->num_future_timesteps * sizeof(mw));
  memcpy(plant->min_powers, min_powers,
         timeline->num_future_timesteps * sizeof(mw));
  memcpy(plant->max_powers, max_powers,
         timeline->num_future_timesteps * sizeof(mw));
}

void plant_copy(struct Plant* dest, const struct Plant* src) {
  plant_initialize(dest,
                   src->id,
                   src->timeline,
                   src->zone,
                   src->min_powers,
                   src->max_powers);
}

void plant_from_json(struct Plant* plant,
                     const struct Timeline* timeline,
                     const struct Zone* zone,
                     json_t* j) {
  ensure_json_is_object(j);
  ensure_json_object_has_size(j, 4);
  ensure_json_object_contains_key(j, JSON_PLANT_ID);
  ensure_json_object_contains_key(j, JSON_PLANT_ZONE);
  ensure_json_object_contains_key(j, JSON_PLANT_MIN_POWERS);
  ensure_json_object_contains_key(j, JSON_PLANT_MAX_POWERS);
  const json_t* j_id = json_object_get(j, JSON_PLANT_ID);
  ensure_json_is_string(j_id);
  const json_t* j_zone = json_object_get(j, JSON_PLANT_ZONE);
  ensure_json_is_string(j_zone);
  const char* zone_id = json_string_value(j_zone);
  ensure_zone_identifiers_are_the_same(zone_id, zone->id);
  const json_t* j_min_powers = json_object_get(j, JSON_PLANT_MIN_POWERS);
  ensure_json_is_array(j_min_powers);
  ensure_json_array_has_size(j_min_powers, timeline->num_future_timesteps);
  ensure_json_is_array_of_numbers(j_min_powers);
  const json_t* j_max_powers = json_object_get(j, JSON_PLANT_MAX_POWERS);
  ensure_json_is_array(j_max_powers);
  ensure_json_array_has_size(j_max_powers, timeline->num_future_timesteps);
  ensure_json_is_array_of_numbers(j_max_powers);

  const char* id = json_string_value(j_id);
  mw min_powers[timeline->num_future_timesteps];
  mw max_powers[timeline->num_future_timesteps];
  for (int t = 0; t < timeline->num_future_timesteps; ++t) {
    min_powers[t] = json_number_value(json_array_get(j_min_powers, t));
    max_powers[t] = json_number_value(json_array_get(j_max_powers, t));
  }
  plant_initialize(plant, id, timeline, zone, min_powers, max_powers);
}

// Destruction
// -----------

void plant_free(struct Plant* plant) {
  free(plant->min_powers);
  free(plant->max_powers);
}

// Accessors
// ---------

bool plant_are_equal(const struct Plant* plant1, const struct Plant* plant2) {
  if (strcmp(plant1->id, plant2->id) != 0)
    return false;
  if (!timeline_are_equal(plant1->timeline, plant2->timeline))
    return false;
  if (!zone_are_equal(plant1->zone, plant2->zone))
    return false;
  for (int t = 0; t < plant1->timeline->num_future_timesteps; ++t) {
    if (plant1->min_powers[t] != plant2->min_powers[t])
      return false;
    if (plant1->max_powers[t] != plant2->max_powers[t])
      return false;
  }
  return true;
}

void plant_print(const struct Plant* plant) {
  printf("A plant with identifier \"%s\"\n", plant->id);
  printf("  Zone: %s\n", plant->zone->id);
  printf("  Minimum powers: ");
  for (int t = 0; t < plant->timeline->num_future_timesteps; ++t) {
    if (t > 0) printf(", ");
    printf("%f", plant->min_powers[t]);
  }
  printf("\n");
  printf("  Maximum powers: ");
  for (int t = 0; t < plant->timeline->num_future_timesteps; ++t) {
    if (t > 0) printf(", ");
    printf("%f", plant->max_powers[t]);
  }
  printf("\n");
}

// JSON serialization
// ------------------

json_t* plant_to_json(const struct Plant* plant) {
  json_t* j_min_powers = json_array();
  json_t* j_max_powers = json_array();
  for (int t = 0; t < plant->timeline->num_future_timesteps; ++t) {
    json_array_append_new(j_min_powers, json_real(plant->min_powers[t]));
    json_array_append_new(j_max_powers, json_real(plant->max_powers[t]));
  }
  return json_pack("{s:s,s:o,s:o,s:s}",
                   "id", plant->id,
                   "max-powers", j_max_powers,
                   "min-powers", j_min_powers,
                   "zone", plant->zone->id);
}
