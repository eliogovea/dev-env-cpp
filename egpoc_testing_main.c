#include <stdlib.h>

#include "egpoc_testing.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    switch (egpoc_testing_run()) {
    case egpoc_testing_error_none:
        return EXIT_SUCCESS;
    case egpoc_testing_error_expect:
    case egpoc_testing_error_unknown:
        return EXIT_FAILURE;
    }

    // TODO: assert unreachable
    return EXIT_FAILURE;
}
