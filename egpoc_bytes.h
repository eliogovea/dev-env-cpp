#pragma once

#ifndef EGPOC_BYTES_H_
#define EGPOC_BYTES_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t const* data;
    size_t         size;
} egpoc_bytes_view_t;

typedef struct {
    uint8_t* data;
    size_t   size;
} egpoc_bytes_span_t;

typedef enum {
    egpoc_bytes_order_le,
    egpoc_bytes_order_be,
    egpoc_bytes_order_unknown,
} egpoc_bytes_order_t;

static inline egpoc_bytes_order_t egpoc_bytes_order_on_host(void)
{
    uint32_t value = 0x01234567;
    uint8_t* bytes = (uint8_t*)&value;

    if (bytes[0] == 0x01 && bytes[1] == 0x23 && bytes[2] == 0x45 && bytes[3] == 0x67) {
        return egpoc_bytes_order_be;
    }

    if (bytes[0] == 0x67 && bytes[1] == 0x45 && bytes[2] == 0x23 && bytes[3] == 0x01) {
        return egpoc_bytes_order_le;
    }

    return egpoc_bytes_order_unknown;
}

static inline uint16_t egpoc_bytes_order_swap_u16(uint16_t value)
{
    return __builtin_bswap16(value);
}

static inline uint16_t egpoc_bytes_order_network_to_host_u16(uint16_t value)
{
    if (egpoc_bytes_order_on_host() == egpoc_bytes_order_le) {
        value = egpoc_bytes_order_swap_u16(value);
    }

    return value;
}

static inline uint16_t egpoc_bytes_order_host_to_network_u16(uint16_t value)
{
    if (egpoc_bytes_order_on_host() == egpoc_bytes_order_le) {
        value = egpoc_bytes_order_swap_u16(value);
    }

    return value;
}

#endif  // EGPOC_BYTES_H_
