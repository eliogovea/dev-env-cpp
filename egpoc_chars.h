#pragma once

#ifndef EGPOC_CHARS_H_
#define EGPOC_CHARS_H_

#include <stddef.h>

typedef struct {
    char const* data;
    size_t      size;
} egpoc_bytes_view_t;

typedef struct {
    char*  data;
    size_t size;
} egpoc_bytes_span_t;

#endif  // EGPOC_CHARS_H_
