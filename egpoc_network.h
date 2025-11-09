#pragma once

#ifndef EGPOC_NETWORK_H_
#define EGPOC_NETWORK_H_

#include <stdbool.h>
#include <stdint.h>

#include "egpoc_bytes.h"

/// @file egpoc_network.h
/// @brief Utility functions for handling IPv4 and IPv6 network address conversions.
///
/// Provides conversion utilities between raw byte representations and structured
/// IPv4/IPv6 address types, following network byte order (big-endian) conventions.

/// @struct egpoc_network_ipv4_address_t
/// @brief Represents an IPv4 address in network byte order.
typedef struct {
    /// IPv4 address value in 32-bit big-endian format.
    uint32_t data;
} egpoc_network_ipv4_address_t;

/// @struct egpoc_network_ipv6_address_t
/// @brief Represents an IPv6 address in network byte order.
///
/// The address is split into two 64-bit segments for convenience:
/// - `data_upper` contains the first 8 bytes (most significant bits)
/// - `data_lower` contains the last 8 bytes (least significant bits)
typedef struct {
    /// Upper 64 bits (first 8 bytes) of the IPv6 address.
    uint64_t data_upper;
    /// Lower 64 bits (last 8 bytes) of the IPv6 address.
    uint64_t data_lower;
} egpoc_network_ipv6_address_t;

/// @brief Converts a 4-byte sequence to an IPv4 address structure.
/// @param[out] address Pointer to the destination IPv4 address structure.
/// @param[in] bytes Byte view containing exactly 4 bytes.
/// @return `true` on success, `false` if input is invalid (e.g., null or wrong size).
static inline bool egpoc_network_ipv4_bytes_to_address(egpoc_network_ipv4_address_t* address, egpoc_bytes_view_t bytes)
{
    if (!address || !bytes.data || bytes.size != 4) {
        return false;
    }

    address->data = ((uint32_t)bytes.data[0] << 24)  //
                  | ((uint32_t)bytes.data[1] << 16)  //
                  | ((uint32_t)bytes.data[2] << 8)   //
                  | ((uint32_t)bytes.data[3]);

    return true;
}

/// @brief Converts an IPv4 address structure to a 4-byte sequence.
/// @param[in] address IPv4 address structure in network byte order.
/// @param[out] bytes Byte span to receive the 4 bytes.
/// @return `true` on success, `false` if output buffer is invalid or wrong size.
static inline bool egpoc_network_ipv4_address_to_bytes(egpoc_network_ipv4_address_t address, egpoc_bytes_span_t bytes)
{
    if (!bytes.data || bytes.size != 4) {
        return false;
    }

    bytes.data[0] = (uint8_t)(address.data >> 24);
    bytes.data[1] = (uint8_t)(address.data >> 16);
    bytes.data[2] = (uint8_t)(address.data >> 8);
    bytes.data[3] = (uint8_t)(address.data);

    return true;
}

/// @brief Converts a 16-byte sequence to an IPv6 address structure.
///
/// The first 8 bytes populate `data_upper` and the last 8 bytes populate `data_lower`,
/// following network byte order (big-endian).
///
/// @param[out] address Pointer to the destination IPv6 address structure.
/// @param[in] bytes Byte view containing exactly 16 bytes.
/// @return `true` on success, `false` if input is invalid (e.g., null or wrong size).
static inline bool egpoc_network_ipv6_bytes_to_address(egpoc_network_ipv6_address_t* address, egpoc_bytes_view_t bytes)
{
    if (!address || !bytes.data || bytes.size != 16) {
        return false;
    }

    address->data_upper = ((uint64_t)bytes.data[0] << 56) | ((uint64_t)bytes.data[1] << 48)
                        | ((uint64_t)bytes.data[2] << 40) | ((uint64_t)bytes.data[3] << 32)
                        | ((uint64_t)bytes.data[4] << 24) | ((uint64_t)bytes.data[5] << 16)
                        | ((uint64_t)bytes.data[6] << 8) | ((uint64_t)bytes.data[7]);

    address->data_lower = ((uint64_t)bytes.data[8] << 56) | ((uint64_t)bytes.data[9] << 48)
                        | ((uint64_t)bytes.data[10] << 40) | ((uint64_t)bytes.data[11] << 32)
                        | ((uint64_t)bytes.data[12] << 24) | ((uint64_t)bytes.data[13] << 16)
                        | ((uint64_t)bytes.data[14] << 8) | ((uint64_t)bytes.data[15]);

    return true;
}

/// @brief Converts an IPv6 address structure to a 16-byte sequence.
///
/// The output is written in network byte order (big-endian).
/// The first 8 bytes come from `data_upper`, followed by 8 bytes from `data_lower`.
///
/// @param[in] address IPv6 address structure in network byte order.
/// @param[out] bytes Byte span to receive the 16 bytes.
/// @return `true` on success, `false` if output buffer is invalid or wrong size.
static inline bool egpoc_network_ipv6_address_to_bytes(egpoc_network_ipv6_address_t address, egpoc_bytes_span_t bytes)
{
    if (!bytes.data || bytes.size != 16) {
        return false;
    }

    bytes.data[0] = (uint8_t)(address.data_upper >> 56);
    bytes.data[1] = (uint8_t)(address.data_upper >> 48);
    bytes.data[2] = (uint8_t)(address.data_upper >> 40);
    bytes.data[3] = (uint8_t)(address.data_upper >> 32);
    bytes.data[4] = (uint8_t)(address.data_upper >> 24);
    bytes.data[5] = (uint8_t)(address.data_upper >> 16);
    bytes.data[6] = (uint8_t)(address.data_upper >> 8);
    bytes.data[7] = (uint8_t)(address.data_upper);

    bytes.data[8]  = (uint8_t)(address.data_lower >> 56);
    bytes.data[9]  = (uint8_t)(address.data_lower >> 48);
    bytes.data[10] = (uint8_t)(address.data_lower >> 40);
    bytes.data[11] = (uint8_t)(address.data_lower >> 32);
    bytes.data[12] = (uint8_t)(address.data_lower >> 24);
    bytes.data[13] = (uint8_t)(address.data_lower >> 16);
    bytes.data[14] = (uint8_t)(address.data_lower >> 8);
    bytes.data[15] = (uint8_t)(address.data_lower);

    return true;
}

#endif  // EGPOC_NETWORK_H_
