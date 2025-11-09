#include "egpoc_system.h"

char const* egpoc_system_error_to_cstring(egpoc_system_error_t error)
{
    switch (error) {
    case egpoc_system_error_none:
        return "Ok";
    case egpoc_system_error_unknown:
        return "Unknown";
    case egpoc_system_error_argument:
        return "Argument";
    case egpoc_system_error_acquire:
        return "Acquire";
    case egpoc_system_error_release:
        return "Release";
    case egpoc_system_error_timeout:
        return "Timeout";
    case egpoc_system_error_invalid:
        return "Invalid";
    case egpoc_system_error_missing:
        return "Unimplemented";
    case egpoc_system_error_oom:
        return "Memory";
    case egpoc_system_error_mmap:
        return "Mapping";
    }

    return "Unrecognized";
}
