#include "egpoc_network.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int egpoc_dns_message_read(uint8_t const*                       message,
                           int                                  message_size,
                           egpoc_dns_message_read_cb_header_t   handle_header,
                           void*                                handle_header_user,
                           egpoc_dns_message_read_cb_question_t handle_question,
                           void*                                handle_question_user,
                           egpoc_dns_message_read_cb_record_t   handle_record,
                           void*                                handle_record_user)
{
    if (!message || message_size <= 0 || !handle_header || !handle_question || !handle_record) {
        return egpoc_dns_error_argument;
    }

    if (message_size < 12) {
        return egpoc_dns_error_truncated;
    }

    egpoc_dns_header_t   header   = {0};
    egpoc_dns_question_t question = {0};
    egpoc_dns_record_t   record   = {0};

    uint8_t name[256];
    memset(name, 0, sizeof(name));

    // header (host endian)
    header.id               = (uint16_t)(message[0] << 8) | message[1];
    header.flags            = (uint16_t)(message[2] << 8) | message[3];
    header.count_questions  = (uint16_t)(message[4] << 8) | message[5];
    header.count_answer     = (uint16_t)(message[6] << 8) | message[7];
    header.count_authority  = (uint16_t)(message[8] << 8) | message[9];
    header.count_additional = (uint16_t)(message[10] << 8) | message[11];

    int handle_header_error = handle_header(handle_header_user, &header);
    if (handle_header_error != egpoc_dns_error_none) {
        return handle_header_error;
    }

    // Only 1 question supported
    if (header.count_questions != 1) {
        return egpoc_dns_error_qdcount;
    }

    int message_index = 12;

    // question

    // question: name
    int name_size = egpoc_dns_name_read(message, message_size, message_index, name, sizeof(name));
    if (name_size < 0) {
        return egpoc_dns_error_compression;
    }

    message_index = egpoc_dns_name_skip(message, message_size, message_index);
    if (message_index < 0) {
        return message_index;
    }

    question.name      = name;
    question.name_size = (uint16_t)name_size;

    // question: type
    if (message_size - message_index < 2) {
        return egpoc_dns_error_truncated;
    }

    question.type = (uint16_t)(message[message_index] << 8 | message[message_index + 1]);
    message_index += 2;

    // question: class
    if (message_size - message_index < 2) {
        return egpoc_dns_error_truncated;
    }

    question.class_ = (uint16_t)(message[message_index] << 8 | message[message_index + 1]);
    message_index += 2;

    int handle_question_error = handle_question(handle_question_user, &question);
    if (handle_question_error != egpoc_dns_error_none) {
        return handle_question_error;
    }

    // answers

    for (int i = 0; i < header.count_answer; i++) {
        // record: name
        name_size = egpoc_dns_name_read(message, message_size, message_index, name, sizeof(name));
        if (name_size < 0) {
            return egpoc_dns_error_compression;
        }

        message_index = egpoc_dns_name_skip(message, message_size, message_index);
        if (message_index < 0) {
            return message_index;
        }

        record.name      = name;
        record.name_size = (uint16_t)name_size;

        // record: type
        if (message_size - message_index < 2) {
            return egpoc_dns_error_truncated;
        }

        record.type = (uint16_t)(message[message_index] << 8 | message[message_index + 1]);
        message_index += 2;

        // record: class
        if (message_size - message_index < 2) {
            return egpoc_dns_error_truncated;
        }

        record.class_ = (uint16_t)(message[message_index] << 8 | message[message_index + 1]);
        message_index += 2;

        // record: time to live
        if (message_size - message_index < 4) {
            return egpoc_dns_error_truncated;
        }

        record.time_to_live = (uint32_t)message[message_index + 0] << 24  //
                            | (uint32_t)message[message_index + 1] << 16  //
                            | (uint32_t)message[message_index + 2] << 8   //
                            | (uint32_t)message[message_index + 3] << 0;
        message_index += 4;

        // record: RDLENGTH
        if (message_size - message_index < 2) {
            return egpoc_dns_error_truncated;
        }

        record.record_data_size = (uint16_t)(message[message_index] << 8 | message[message_index + 1]);
        message_index += 2;

        // record: RDATA
        if (message_size - message_index < record.record_data_size) {
            return egpoc_dns_error_truncated;
        }

        record.record_data = message + message_index;
        message_index += record.record_data_size;

        int handle_record_error = handle_record(handle_record_user, &record);
        if (handle_record_error != egpoc_dns_error_none) {
            return handle_record_error;
        }
    }

    return egpoc_dns_error_none;
}

int egpoc_dns_name_skip(uint8_t const* message,  //
                        int            message_size,
                        int            message_index)
{
    int found = 0;
    int error = egpoc_dns_error_none;

    while (found != 0 && message_index < message_size) {
        switch (message[message_index] & 0xC0U) {
        case 0x00U: {  // label length
            found         = (message[message_index] == 0U);
            message_index = message_index + 1 + message[message_index];
        } break;
        case 0xC0U: {  // label pointer
            found         = true;
            message_index = message_index + 2;
        } break;
        default: {  // error
            found         = false;
            error         = egpoc_dns_error_label;
            message_index = message_size;
        } break;
        }
    }

    if (error != egpoc_dns_error_none) {
        return error;
    }

    if (!found || message_index <= message_size) {
        return egpoc_dns_error_truncated;
    }

    return message_index;
}

int egpoc_dns_name_read(uint8_t const* message,  //
                        int            message_size,
                        int            message_index,
                        uint8_t*       name,
                        int            name_capacity)
{
    int name_size   = 0;
    int name_error  = egpoc_dns_error_none;
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
                name_error = egpoc_dns_error_truncated;
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
                name_error = egpoc_dns_error_truncated;
            } else {
                uint16_t index_msb = message[message_index + 0] & 0x3FU;
                uint16_t index_lsb = message[message_index + 1];
                message_index      = (uint16_t)(index_msb << 8U) | index_lsb;
                if (message_min <= message_index) {  // prevent loops or forward jumps
                    name_error = egpoc_dns_error_compression;
                } else {
                    message_min = message_index;
                }
            }
        } break;
        default: {  // error
            name_error = egpoc_dns_error_label;
        } break;
        }
    }

    if (name_error != egpoc_dns_error_none) {
        return name_error;
    }

    if (!name_found) {
        return egpoc_dns_error_internal;
    }

    return name_size;
}
