#pragma once

#ifndef EGPOC_DEBUG_H_
#define EGPOC_DEBUG_H_

#if defined(__clang__) || defined(__GNUC__)
    #define EGPOC_ATTRIBUTE_FORMAT(index_format, index_args) \
        __attribute__((format(printf, index_format, index_args)))
#else
    #define EGPOC_ATTRIBUTE_FORMAT(index_format, index_args)
#endif

void egpoc_debug(char const* file, int line, char const* format, ...) EGPOC_ATTRIBUTE_FORMAT(3, 4);

#define EGPOC_DEBUG(...) egpoc_debug(__FILE__, __LINE__, __VA_ARGS__)

#endif  // EGPOC_DEBUG_H_
