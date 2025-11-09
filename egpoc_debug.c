#include "egpoc_debug.h"

#include <stdarg.h>
#include <stdio.h>

void egpoc_debug(char const* file, int line, char const* format, ...)
{
    fprintf(stderr, "%s:%d ", file, line);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fflush(stderr);
}
