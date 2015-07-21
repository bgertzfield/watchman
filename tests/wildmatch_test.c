/* Copyright 2015-present Facebook, Inc.
 * Licensed under the Apache License, Version 2.0 */

#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thirdparty/tap.h"
#include "thirdparty/jansson/jansson.h"
#include "thirdparty/wildmatch/wildmatch.h"

static void run_test(json_t *test_case_data)
{
  int wildmatch_should_succeed;
  char *text_to_match;
  char *pattern_to_use;

  json_error_t error;
  if (json_unpack_ex(
        test_case_data,
        &error,
        0,
        "[b,s,s]",
        &wildmatch_should_succeed,
        &text_to_match,
        &pattern_to_use) == -1) {
    fail(
      "Error decoding JSON: %s (source=%s, line=%d, col=%d)\n",
      error.text,
      error.source,
      error.line,
      error.column);
    return;
  }

  int wildmatch_succeeded = wildmatch(pattern_to_use, text_to_match) == 1;
  if (wildmatch_should_succeed) {
    ok(
      wildmatch_succeeded,
      "Pattern [%s] should match text [%s]", pattern_to_use, text_to_match);
  } else {
    ok(
      !wildmatch_succeeded,
      "Pattern [%s] should not match text [%s]", pattern_to_use, text_to_match);
  }
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  FILE *test_cases_file = fopen(WILDMATCH_TEST_JSON_FILE, "r");
  if (!test_cases_file) {
    diag("Couldn't open %s: %s\n", WILDMATCH_TEST_JSON_FILE, strerror(errno));
    abort();
  }
  json_error_t error;
  json_t *test_cases = json_loadf(test_cases_file, 0, &error);
  if (!test_cases) {
    diag(
      "Error decoding JSON: %s (source=%s, line=%d, col=%d)\n",
      error.text,
      error.source,
      error.line,
      error.column);
    abort();
  }
  if (fclose(test_cases_file) != 0) {
    diag("Error closing %s: %s\n", WILDMATCH_TEST_JSON_FILE, strerror(errno));
    abort();
  }
  if (!json_is_array(test_cases)) {
    diag("Expected JSON in %s to be an array\n", WILDMATCH_TEST_JSON_FILE);
    abort();
  }
  size_t num_tests = json_array_size(test_cases);
  plan_tests(num_tests);
  size_t index;
  for (index = 0; index < num_tests; index++) {
    json_t *test_case_data = json_array_get(test_cases, index);
    run_test(test_case_data);
  }
  json_decref(test_cases);
  return exit_status();
}
