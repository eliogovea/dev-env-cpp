#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "egpoc_debug.h"
#include "egpoc_network.h"
#include "egpoc_testing.h"

EGPOC_TEST(egpoc_network, egpoc_dns_name_read_simple)
{
    // clang-format off
    static uint8_t const message[] = {
        3, 'w', 'w', 'w',
        7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        3, 'c', 'o', 'm',
        0,
    };
    // clang-format on

    // clang-format off
    static uint8_t const name_expected[] = {
        3, 'w', 'w', 'w',
        7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        3, 'c', 'o', 'm',
        0,
    };
    // clang-format on

    uint8_t name[256];

    int name_size = egpoc_dns_name_read(message,  //
                                        sizeof(message),
                                        0,
                                        name,
                                        sizeof(name));

    EGPOC_EXPECT(name_size == sizeof(name_expected));
    EGPOC_EXPECT(memcmp(name, name_expected, sizeof(name_expected)) == 0);

    return egpoc_testing_error_none;
}

EGPOC_TEST(egpoc_network, egpoc_dns_name_read_pointer)
{
    // clang-format off
    static uint8_t const message[] = {
        3, 'c', 'o', 'm',
        0,
        3, 'w', 'w', 'w',
        7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        0xC0, 0x00 // pointer to the start
    };
    // clang-format on

    // clang-format off
    static uint8_t const name_expected[] = {
        3, 'w', 'w', 'w',
        7, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        3, 'c', 'o', 'm',
        0,
    };
    // clang-format on

    uint8_t name[256];

    int name_size = egpoc_dns_name_read(message,  //
                                        sizeof(message),
                                        5,
                                        name,
                                        sizeof(name));

    EGPOC_EXPECT(name_size == sizeof(name_expected));
    EGPOC_EXPECT(memcmp(name, name_expected, sizeof(name_expected)) == 0);

    return egpoc_testing_error_none;
}

EGPOC_TEST(egpoc_network, egpoc_dns_name_read_pointer_loop)
{
    static uint8_t const message[] = {0xC0, 0x00};

    uint8_t name[256];

    int name_size = egpoc_dns_name_read(message,  //
                                        sizeof(message),
                                        0,
                                        name,
                                        sizeof(name));

    EGPOC_EXPECT(name_size == -1);

    return egpoc_testing_error_none;
}

EGPOC_TEST(egpoc_network, egpoc_dns_name_read_pointer_forward)
{
    static uint8_t const message[] = {
        3,
        'a',
        'b',
        'c',  // abc
        0,    // end
        0xC0,
        0x05  // pointer forward past end
    };

    uint8_t name[256];

    int name_size = egpoc_dns_name_read(message,  //
                                        sizeof(message),
                                        5,
                                        name,
                                        sizeof(name));

    EGPOC_EXPECT(name_size == -1);

    return egpoc_testing_error_none;
}

EGPOC_TEST(egpoc_network, egpoc_dns_name_read_label_too_long)
{
    static uint8_t const message[]
        = {70,  // invalid label length >63
           'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
           'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
           'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};

    uint8_t name[256];

    int name_size = egpoc_dns_name_read(message,  //
                                        sizeof(message),
                                        0,
                                        name,
                                        sizeof(name));

    EGPOC_EXPECT(name_size == -1);

    return egpoc_testing_error_none;
}

EGPOC_TEST(egpoc_network, egpoc_dns_name_read_buffer_too_small)
{
    static uint8_t const message[] = {3, 'w', 'w', 'w', 7, 'e', 'x', 'a', 'm', 'p', 'l', 'e', 3, 'c', 'o', 'm', 0};

    uint8_t name[4];  // deliberately too small

    int name_size = egpoc_dns_name_read(message,  //
                                        sizeof(message),
                                        0,
                                        name,
                                        sizeof(name));

    EGPOC_EXPECT(name_size == -1);

    return egpoc_testing_error_none;
}
