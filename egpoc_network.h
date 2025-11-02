#pragma once

#ifndef EGPOC_NETWORK_H_
#define EGPOC_NETWORK_H_

#include <stdint.h>

#include "egpoc_bytes.h"

typedef struct {
    uint32_t data;
} egpoc_network_ipv4_address_t;

typedef struct {
    uint64_t data_upper;
    uint64_t data_lower;
} egpoc_network_ipv6_address_t;

static inline int egpoc_network_ipv4_bytes_to_address(egpoc_network_ipv4_address_t* address, egpoc_bytes_view_t bytes)
{
    if (bytes.size != 4) {
        return 0;
    }

    address->data = 0;

    address->data |= (uint32_t)bytes.data[0] << 24;
    address->data |= (uint32_t)bytes.data[1] << 16;
    address->data |= (uint32_t)bytes.data[2] << 8;
    address->data |= (uint32_t)bytes.data[3];

    return 1;
}

static inline int egpoc_network_ipv4_address_to_bytes(egpoc_network_ipv4_address_t address, egpoc_bytes_span_t bytes)
{
    if (bytes.size != 4) {
        return 0;
    }

    bytes.data[0] = (uint8_t)address.data >> 24;
    bytes.data[1] = (uint8_t)address.data >> 16;
    bytes.data[2] = (uint8_t)address.data >> 8;
    bytes.data[3] = (uint8_t)address.data;

    return 1;
}

#endif  // EGPOC_NETWORK_H_
