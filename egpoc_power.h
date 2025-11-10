#pragma once

#ifndef EGPOC_POWER_H_
#define EGPOC_POWER_H_

#include <stdint.h>

/// Function pointer type for copying values of arbitrary type.
/// Must copy the value from src to dst.
typedef void (*egpoc_power_copy_t)(void const* src, void* dst);

/// Function pointer type for setting a value to its multiplicative identity.
/// For example, for integers, this would set the value to 1.
typedef void (*egpoc_power_identity_t)(void* value);

/// Function pointer type for multiplying two values of arbitrary type.
/// The result is stored in the third argument.
typedef void (*egpoc_power_multiply_t)(void const* lhs, void const* rhs, void* product);

/// Computes the exponentiation of an arbitrary type using binary exponentiation.
///
/// This function is fully generic and type-agnostic. The caller must provide
/// type-specific implementations for copying, multiplying, and initializing
/// the multiplicative identity.
///
/// \param base Pointer to the base value. **Note:** this value may be modified during computation.
/// \param power Pointer to the output/result value.
/// \param extra Pointer to a temporary buffer of the same size as `base` and `power`.
///              Used to avoid in-place overwrites during multiplication.
/// \param exponent Non-negative integer exponent.
/// \param copy Function pointer to copy a value from source to destination.
/// \param multiply Function pointer to multiply two values of the given type.
/// \param identity Function pointer to set a value to the multiplicative identity.
///
/// \par Examples:
/// \code
/// #include <stdint.h>
/// #include "egpoc_power.h"
///
/// // Type-specific helpers for uint32_t
/// static inline void egpoc_power_identity_uint32(void* value) {
///     *(uint32_t*)value = 1;
/// }
///
/// static inline void egpoc_power_multiply_uint32(void const* lhs, void const* rhs, void* result) {
///     *(uint32_t*)result = (*(uint32_t const*)lhs) * (*(uint32_t const*)rhs);
/// }
///
/// static inline void egpoc_power_copy_uint32(void const* src, void* dst) {
///     *(uint32_t*)dst = *(uint32_t const*)src;
/// }
///
/// int main() {
///     uint32_t base = 2;
///     uint32_t power;
///     uint32_t extra;
///
///     egpoc_power(&base, &power, &extra, 10,
///                 egpoc_power_copy_uint32,
///                 egpoc_power_multiply_uint32,
///                 egpoc_power_identity_uint32);
///
///     // result == 1024
/// }
/// \endcode
///
/// \par Notes / Limitations:
/// - `base` is modified during computation; preserve it if needed.
/// - The caller must provide a temporary buffer (`extra`) large enough to hold one value.
/// - Function pointers introduce runtime overhead; the compiler cannot inline or optimize
///   across the function calls.
/// - Suitable for arbitrary types including structs, big integers, or matrices.
///   For primitive types where performance is critical, a macro-based version is preferable.
void egpoc_power(void*                  base,
                 void*                  power,
                 void*                  extra,
                 uintmax_t              exponent,
                 egpoc_power_copy_t     copy,
                 egpoc_power_multiply_t multiply,
                 egpoc_power_identity_t identity);

#endif  // EGPOC_POWER_H_
