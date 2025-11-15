#include "egpoc_network.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int egpoc_dns_name_skip(uint8_t const* message, int message_size)
{
    bool found = false;
    int  index = 0U;

    while (!found && index < message_size) {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        switch (message[index] & 0xC0U) {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        case 0x00U: {  // label length
            found = (message[index] == 0U);
            index = index + 1 + message[index];
        } break;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        case 0xC0U: {  // label pointer
            found = true;
            index = index + 2;
        } break;
        default: {  // error
            found = false;
            index = message_size;
        } break;
        }
    }

    return (found && index <= message_size) ? index : 0;
}

int egpoc_dns_name_read(uint8_t const* message,  //
                        int            message_size,
                        int            message_index,
                        uint8_t*       name,
                        int            name_capacity)
{
    int name_size   = 0;
    int name_error  = 0;
    int name_found  = 0;
    int message_min = message_index;

    if (256 < name_capacity) {
        name_capacity = 256;
    }

    while (!name_error && !name_found && message_index < message_size) {
        switch (message[message_index] & 0xC0U) {
        case 0x00U: {  // label length
            int bytes_need = 1 + message[message_index];
            int bytes_have = name_capacity - name_size;
            if (bytes_have < bytes_need) {
                name_error = 1;
            } else {
                uint8_t*       name_ptr    = name + name_size;
                uint8_t const* message_ptr = message + message_index;
                memcpy(name_ptr, message_ptr, (size_t)bytes_need);
                name_found    = (message[message_index] == 0);
                name_size     = name_size + bytes_need;
                message_index = message_index + bytes_need;
            }
        } break;
        case 0xC0U: {  // label pointer
            int bytes_need = 2;
            int bytes_have = name_capacity - name_size;
            if (bytes_have < bytes_need) {
                name_error = 1;
            } else {
                uint16_t index_msb = message[message_index] & 0x3FU;
                uint16_t index_lsb = message[message_index + 1];
                message_index      = (uint16_t)(index_msb << 8U) | index_lsb;
                if (message_min <= message_index) {  // prevent loops or forward jumps
                    name_error = 1;
                } else {
                    message_min = message_index;
                }
            }
        } break;
        default: {  // error
            name_error = 1;
        } break;
        }
    }

    if (name_error || !name_found) {
        return -1;
    }

    return name_size;
}
