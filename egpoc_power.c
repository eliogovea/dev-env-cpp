#include "egpoc_power.h"

void egpoc_power(void*                  base,
                 void*                  power,
                 void*                  extra,
                 uintmax_t              exponent,
                 egpoc_power_copy_t     copy,
                 egpoc_power_multiply_t multiply,
                 egpoc_power_identity_t identity)
{
    identity(power);
    while (exponent > 0) {
        if (exponent & 1) {
            multiply(power, base, extra);
            copy(extra, power);
        }
        multiply(base, base, extra);
        copy(extra, base);
        exponent >>= 1;
    }
}
