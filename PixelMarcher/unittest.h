/**
 *                            unittest.h
 *                           -----------
 *          One-Header file simple testing framework for C code.
 *  ------------------------------------------------------------------------
 *
 *  Usage:
 *      - Add this header file to your project directory and import it
 *
 *      - Define a test case as follows:
 *            TEST(test_name) {
 *                  <code>
 *            }
 *
 *      - Each test case is expected to exit with non-zero status if it fails.
 *          returning from the test function counts as a success. The provided
 *          TEST_FAIL() function prints out a given message and exits with 1,
 *          but you can also use the functionality from `assert.h`
 *
 *      - In the main function, call `unit_main(argc, argv)` to run tests.
 *
 *      - Run `./program test_name` to run a single test or `./program`
 *          to run all tests. The latter is not ideal as no further tests
 *          are fun as soon as a single one fails.
 *
 * A default main() function is included in the header file, if you don't need
 * any other functionality then you can use it by simply putting
 *
 *        #define UNITTEST_DEFAULT_MAIN
 *
 * in the C file before importing the header. You don't need to define any
 * main function in this case.
 *
 *  ------------------------------------------------------------------------
 *                          Mustafa Quraish, 2020
 */
#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Main test case container */
typedef struct unit_test {
  char *name;              // User-defined name of test case
  void (*test_func)(void); // Function pointer to test handler
  struct unit_test *next;  // Next pointer for internal linked list
} unit_test;

/* Main linked list for test cases */
typedef struct unit_test_map {
  int num_tests;
  unit_test *list;
} unit_test_list;

/* Global list of all test cases */
unit_test_list ALL_TESTS = {0, NULL};

/* Log things to stdout if verbose mode is on */
void unit_log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

/* Log things to stdout if verbose mode is on */
void TEST_FAIL(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  exit(1);
}

/* Find the test case given the name, NULL if doesn't exist */
unit_test *unit_get_test(char *name) {
  for (unit_test *cur = ALL_TESTS.list; cur != NULL; cur = cur->next)
    if (strcmp(cur->name, name) == 0)
      return cur;
  return NULL;
}

/* Create and add test to head of linked list if it doesn't already exist */
void unit_insert_test(char *name, void (*test_func)(void)) {
  unit_test *node = (unit_test *)calloc(sizeof(unit_test), 1);
  node->name = name;
  node->test_func = test_func;

  if (ALL_TESTS.list == NULL) {
    ALL_TESTS.list = node;
    return;
  }

  unit_test *cur = ALL_TESTS.list;
  while (cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = node;
}

/* Run test */
void unit_run_test(unit_test *test) {
  unit_log("[+] Running test: %s ... ", test->name);
  test->test_func();
  unit_log("Passed!\n");
}

/* Run all the tests */
void unit_run_all_tests() {
  unit_test *cur = ALL_TESTS.list;
  while (cur != NULL) {
    unit_run_test(cur);
    cur = cur->next;
  }
}

/* Free all associated data */
void unit_free_lists() {
  unit_test *cur = (unit_test *)ALL_TESTS.list;
  while (cur != NULL) {
    unit_test *temp = cur->next;
    free(cur);
    cur = temp;
  }
}

/**
 * Main function to handle running all tests. Should take in argc and argv
 * from `main()`. If no arguments are passed in, all tests are run.
 */
void unit_main(int argc, char **argv) {
  if (argc == 1) {
    unit_run_all_tests();
  } else {
    unit_test *test = unit_get_test(argv[1]);
    if (test == NULL) {
      fprintf(stderr, "Test %s not found. Exiting.\n", argv[1]);
      exit(1);
    }
    unit_run_test(test);
  }
  unit_free_lists();
  unit_log("[+] Done.\n");
}

#define TEST(name)                                                             \
  void unit_testcase_##name(void);                                             \
  __attribute__((constructor)) void unit_constructor_##name() {                \
    unit_insert_test(#name, unit_testcase_##name);                             \
  }                                                                            \
  void unit_testcase_##name(void)

/** If this flag is defined, use the default basic main function **/
#ifdef UNITTEST_DEFAULT_MAIN
int main(int argc, char **argv) { unit_main(argc, argv); }
#endif