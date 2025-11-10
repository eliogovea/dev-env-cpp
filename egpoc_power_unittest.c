#include <stdint.h>
#include <string.h>

#include "egpoc_power.h"
#include "egpoc_testing.h"

static inline void egpoc_power_identity_uint32(void* value)
{
    *(uint32_t*)value = 1;
}

static inline void egpoc_power_multiply_uint32(void const* lhs, void const* rhs, void* result)
{
    *(uint32_t*)result = (*(uint32_t const*)lhs) * (*(uint32_t const*)rhs);
}

static inline void egpoc_power_copy_uin32(void const* src, void* dst)
{
    *(uint32_t*)dst = *(uint32_t const*)src;
}

typedef struct matrix_2x2_uint32_t {
    uint32_t data[2][2];
} matrix_2x2_uint32_t;

static inline void egpoc_power_copy_matrix_2x2_uint32(void const* src, void* dst)
{
    memcpy(dst, src, sizeof(matrix_2x2_uint32_t));
}

static inline void egpoc_power_identity_matrix_2x2_uint32(void* value)
{
    matrix_2x2_uint32_t* matrix = (matrix_2x2_uint32_t*)value;

    matrix->data[0][0] = 1;
    matrix->data[0][1] = 0;
    matrix->data[1][0] = 0;
    matrix->data[1][1] = 1;
}

static inline void egpoc_power_multiply_matrix_2x2_uint32(void const* lhs, void const* rhs, void* product)
{
    matrix_2x2_uint32_t const* matrix_lhs     = (matrix_2x2_uint32_t const*)lhs;
    matrix_2x2_uint32_t const* matrix_rhs     = (matrix_2x2_uint32_t const*)rhs;
    matrix_2x2_uint32_t*       matrix_product = (matrix_2x2_uint32_t*)product;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            matrix_product->data[i][j] = 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < 2; j++) {
                matrix_product->data[i][j] += matrix_lhs->data[i][k] * matrix_rhs->data[k][j];
            }
        }
    }
}

EGPOC_TEST(power, uint32)
{
    uint32_t base  = 2;
    uint32_t power = 0;
    uint32_t extra = 0;
    egpoc_power(&base,
                &power,  //
                &extra,
                5,
                &egpoc_power_copy_uin32,
                &egpoc_power_multiply_uint32,
                egpoc_power_identity_uint32);
    EGPOC_EXPECT(power == 32);
    return egpoc_testing_error_none;
}

EGPOC_TEST(power, matrix_2x2_uint32_identity)
{
    matrix_2x2_uint32_t base;
    matrix_2x2_uint32_t power;
    matrix_2x2_uint32_t extra;

    egpoc_power_identity_matrix_2x2_uint32(&base);

    EGPOC_EXPECT(base.data[0][0] == 1);
    EGPOC_EXPECT(base.data[0][1] == 0);
    EGPOC_EXPECT(base.data[1][0] == 0);
    EGPOC_EXPECT(base.data[1][1] == 1);

    egpoc_power(&base,
                &power,
                &extra,
                5,
                &egpoc_power_copy_matrix_2x2_uint32,
                &egpoc_power_multiply_matrix_2x2_uint32,
                egpoc_power_identity_matrix_2x2_uint32);

    EGPOC_EXPECT(power.data[0][0] == 1);
    EGPOC_EXPECT(power.data[0][1] == 0);
    EGPOC_EXPECT(power.data[1][0] == 0);
    EGPOC_EXPECT(power.data[1][1] == 1);

    return egpoc_testing_error_none;
}

EGPOC_TEST(power, matrix_2x2_uint32_ones)
{
    matrix_2x2_uint32_t base;
    matrix_2x2_uint32_t power;
    matrix_2x2_uint32_t extra;

    base.data[0][0] = 1;
    base.data[0][1] = 1;
    base.data[1][0] = 1;
    base.data[1][1] = 1;

    egpoc_power(&base,
                &power,
                &extra,
                2,
                &egpoc_power_copy_matrix_2x2_uint32,
                &egpoc_power_multiply_matrix_2x2_uint32,
                egpoc_power_identity_matrix_2x2_uint32);

    EGPOC_EXPECT(power.data[0][0] == 2);
    EGPOC_EXPECT(power.data[0][1] == 2);
    EGPOC_EXPECT(power.data[1][0] == 2);
    EGPOC_EXPECT(power.data[1][1] == 2);

    return egpoc_testing_error_none;
}
