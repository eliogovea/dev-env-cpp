#include "egpoc_testing.h"

#include <stdlib.h>

static egpoc_testing_test_t* egpoc_testing_head = NULL;

egpoc_testing_error_t egpoc_testing_add(egpoc_testing_test_t* test)
{
    test->next         = egpoc_testing_head;
    egpoc_testing_head = test;
    return 1;
}

egpoc_testing_error_t egpoc_testing_run(void)
{
    int failed = 0;
    for (egpoc_testing_test_t* t = egpoc_testing_head; t; t = t->next) {
        fprintf(stdout, "[ RUN      ] %s.%s\n", t->suite, t->name);
        egpoc_testing_error_t error = t->func();
        switch (error) {
        case egpoc_testing_error_none: {
            fprintf(stdout, "[       OK ] %s.%s\n", t->suite, t->name);
        } break;
        case egpoc_testing_error_expect:
        case egpoc_testing_error_unknown: {
            fprintf(stdout, "[   FAILED ] %s.%s\n", t->suite, t->name);
            failed++;
        } break;
        default: {
            // TODO: assert unreachable
            fprintf(stderr, "%s:%d Expected unreachable\n", __FILE__, __LINE__);
            abort();
        } break;
        }
    }
    fprintf(stdout, "\nTotal failed: %d\n", failed);
    return failed > 0 ? egpoc_testing_error_unknown : egpoc_testing_error_none;
}
