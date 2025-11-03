#ifndef EGPOC_TESTING_H_
#define EGPOC_TESTING_H_

#include <stdio.h>

/// @brief Error codes returned by tests.
typedef enum {
    egpoc_testing_error_none,     ///< No error, test passed.
    egpoc_testing_error_expect,   ///< Test failed due to failed EGPOC_EXPECT().
    egpoc_testing_error_unknown,  ///< Unspecified or unexpected failure.
} egpoc_testing_error_t;

/// @brief Type for a test function.
/// Each test function returns an `egpoc_testing_error_t` value.
typedef egpoc_testing_error_t (*egpoc_testing_func_t)(void);

/// @brief Represents a single test case (suite + name + function pointer).
typedef struct egpoc_testing_test_t {
    char const*                  suite;  ///< Suite name (e.g. "Math").
    char const*                  name;   ///< Test name (e.g. "Addition").
    egpoc_testing_func_t         func;   ///< Function implementing the test.
    struct egpoc_testing_test_t* next;   ///< Pointer to next registered test.
} egpoc_testing_test_t;

/// @brief Adds a test to the global registry.
/// Normally called automatically by EGPOC_TEST.
/// @param test Pointer to a test structure.
/// @return egpoc_testing_error_none if added successfully.
egpoc_testing_error_t egpoc_testing_add(egpoc_testing_test_t* test);

/// @brief Runs all registered tests and prints results to stdout.
/// @return egpoc_testing_error_none if all tests passed,
///         egpoc_testing_error_unknown if one or more failed.
egpoc_testing_error_t egpoc_testing_run(void);

/// @brief Fails the current test if `expr` evaluates to false.
/// Prints the expression, filename, and line number, then returns
/// `egpoc_testing_error_expect`.
/// Example:
/// ```c
/// EGPOC_EXPECT(x == 10);
/// ```
#define EGPOC_EXPECT(expr)                                                              \
    do {                                                                                \
        if (!(expr)) {                                                                  \
            printf("  Expectation failed: %s (at %s:%d)\n", #expr, __FILE__, __LINE__); \
            return (int)egpoc_testing_error_expect;                                     \
        }                                                                               \
    } while (0)

/// @brief Defines and registers a new test.
/// Each test must return an `egpoc_testing_error_t` value.
/// Example:
/// ```c
/// EGPOC_TEST(Math, Addition)
/// {
///     EGPOC_EXPECT(1 + 1 == 2);
///     return egpoc_testing_error_none;
/// }
/// ```
/// @param SUITE Logical group name for the test (e.g., Math)
/// @param NAME  Individual test name (e.g., Addition)
#define EGPOC_TEST(SUITE, NAME)                                                       \
    /* declare testing functions */                                                   \
    static egpoc_testing_error_t egpoc_testing_function_##SUITE##_##NAME(void);       \
                                                                                      \
    static egpoc_testing_test_t egpoc_testing_test_##SUITE##_##NAME = {               \
        #SUITE,                                                                       \
        #NAME,                                                                        \
        egpoc_testing_function_##SUITE##_##NAME,                                      \
        NULL,                                                                         \
    };                                                                                \
                                                                                      \
    /* register this test before main() executes (GCC/Clang only) */                  \
    __attribute__((constructor)) static void egpoc_testing_add_##SUITE##_##NAME(void) \
    {                                                                                 \
        egpoc_testing_add(&egpoc_testing_test_##SUITE##_##NAME);                      \
    }                                                                                 \
                                                                                      \
    static egpoc_testing_error_t egpoc_testing_function_##SUITE##_##NAME(void)

#endif /* EGPOC_TESTING_H_ */
