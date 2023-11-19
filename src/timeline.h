#ifndef TIMELINE_H
#define TIMELINE_H

#include <stdbool.h>

#include <jansson.h>

// JSON keys
// ---------

#define JSON_TIMELINE_FUTURE_DURATIONS "future-durations"

// Type
// ----

struct Timeline {
  // The number of future timesteps
  unsigned int num_future_timesteps;
  // The future timesteps durations in minutes
  int* future_durations;
};

// Initialization
// --------------

/**
 * Initializes a timeline
 *
 * @param timeline              The timeline to initialize
 * @param num_future_timesteps  The number of future timesteps
 * @param future_durations      The duration of each future timesteps
 */
void timeline_initialize(struct Timeline* timeline,
                         unsigned int num_future_timesteps,
                         const int* future_durations);

/**
 * Initializes a timeline from another one
 *
 * @param dest  The destination timeline
 * @param src   The source timeline
 */
void timeline_copy(struct Timeline* dest, const struct Timeline* src);

/**
 * Initializes a timeline from a JSON value
 *
 * @param timeline  The timeline to initialize
 * @param j         The JSON value
 */
void timeline_from_json(struct Timeline* timeline,
                        json_t* j);

// Destruction
// -----------

/**
 * Frees a timeline
 *
 * @param timeline  The timeline to delete
 */
void timeline_free(struct Timeline* timeline);

// Accessors
// ---------

/**
 * Indicates if two timelines are equal
 *
 * @param timeline1  The first timeline
 * @param timeline2  The second timeline
 * @return           true if and only if the timelines are equal
 */
bool timeline_are_equal(const struct Timeline* timeline1,
                        const struct Timeline* timeline2);

/**
 * Prints a timeline to stdout
 *
 * @param timeline  The timeline to print
 */
void timeline_print(const struct Timeline* timeline);

// JSON serialization
// ------------------

/**
 * Converts a timeline to a JSON value
 *
 * @param timeline  The timeline to convert
 * @return          The JSON value
 */
json_t* timeline_to_json(const struct Timeline* timeline);

#endif
