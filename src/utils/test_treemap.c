#include "treemap.h"

#include <tap.h>

// Main
// ----
int main() {
  struct Treemap t;
  treemap_initialize(&t);
  diag("Initializing a treemap with three pairs of key-value:");
  diag("  \"alpha\": 1.0");
  diag("  \"gamma\": 3.0");
  diag("  \"beta\": 2.0");
  treemap_set(&t, "alpha", 1.0);
  treemap_set(&t, "gamma", 3.0);
  treemap_set(&t, "beta", 2.0);
  cmp_ok(treemap_get(&t, "alpha"), "==", 1.0, "treemap_get(alpha) == 1.0");
  cmp_ok(treemap_get(&t, "beta"), "==", 2.0, "treemap_get(beta) == 2.0");
  cmp_ok(treemap_get(&t, "gamma"), "==", 3.0, "treemap_get(gamma) == 3.0");
  diag("Changing value of \"alpha\" to 4.0");
  treemap_set(&t, "alpha", 4.0);
  cmp_ok(treemap_get(&t, "alpha"), "==", 4.0, "treemap_get(alpha) == 4.0");
  ok(treemap_are_equal(&t, &t), "a treemap is equal to itself");
  struct StringArray sa;
  treemap_compute_keys(&t, &sa);
  diag("Retrieving the treemap's keys"); 
  printf("# ");
  for (int i = 0; i < sa.size; ++i)
    printf("%s ", sa.strings[i]);
  printf("\n");
  string_array_delete(&sa);
  treemap_delete(&t);
  done_testing();
}
