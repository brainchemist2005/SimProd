#include "link.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validation.h"
#include "zone.h"

// Creation
// --------

void link_initialize(struct Link* link,
                     const char* id,
                     const struct Zone* source,
                     const struct Zone* target) {
  strncpy(link->id, id, ID_MAX_LENGTH);
  link->source = source;
  link->target = target;
}

void link_copy(struct Link* dest, const struct Link* src) {
  link_initialize(dest, src->id, src->source, src->target);
}

void link_from_json(struct Link* link,
                     const struct Zone* source,
                     const struct Zone* target,
                    json_t* j) {
  ensure_json_is_object(j);
  ensure_json_object_has_size(j, 3);
  ensure_json_object_contains_key(j, JSON_LINK_ID);
  ensure_json_object_contains_key(j, JSON_LINK_SOURCE);
  ensure_json_object_contains_key(j, JSON_LINK_TARGET);
  const json_t* j_id = json_object_get(j, JSON_LINK_ID);
  ensure_json_is_string(j_id);
  const json_t* j_source = json_object_get(j, JSON_LINK_SOURCE);
  ensure_json_is_string(j_source);
  const json_t* j_target = json_object_get(j, JSON_LINK_TARGET);
  ensure_json_is_string(j_target);
  const char* source_id = json_string_value(j_source);
  ensure_zone_identifiers_are_the_same(source_id, source->id);
  const char* target_id = json_string_value(j_target);
  ensure_zone_identifiers_are_the_same(target_id, target->id);

  const char* id = json_string_value(j_id);
  link_initialize(link, id, source, target);
}

// Destruction
// -----------

void link_free(struct Link* link) {
}

// Accessors
// ---------

bool link_are_equal(const struct Link* link1, const struct Link* link2) {
  if (strcmp(link1->id, link2->id) != 0)
    return false;
  if (!zone_are_equal(link1->source, link2->source))
    return false;
  if (!zone_are_equal(link1->target, link2->target))
    return false;
  return true;
}

void link_print(const struct Link* link) {
  printf("A link with identifier \"%s\"\n", link->id);
  printf("  Source zone: %s\n", link->source->id);
  printf("  Target zone: %s\n", link->target->id);
}

// JSON serialization
// ------------------

json_t* link_to_json(const struct Link* link) {
  return json_pack("{s:s,s:s,s:s}",
                   "id", link->id,
                   "source", link->source->id,
                   "target", link->target->id);
}
