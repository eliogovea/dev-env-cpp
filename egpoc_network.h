#pragma once

#ifndef EGPOC_NETWORK_H_
#define EGPOC_NETWORK_H_

#include <stdbool.h>
#include <stdint.h>

/// @struct egpoc_network_ipv4_address_t
/// @brief Represents an IPv4 address stored in *network byte order*.
typedef struct {
    /// IPv4 address value in network byte order.
    uint32_t data;
} egpoc_network_ipv4_address_t;

/// @struct egpoc_network_ipv6_address_t
/// @brief Represents an IPv6 address stored in *network byte order*.
typedef struct {
    /// Upper 64 bits (first 8 bytes), network byte order.
    uint64_t data_upper;
    /// Lower 64 bits (last 8 bytes), network byte order.
    uint64_t data_lower;
} egpoc_network_ipv6_address_t;

/// @struct egpoc_dns_header_t
/// @brief DNS message header stored fully in *host byte order*.
///
/// All fields are converted from network order when parsing, and must be
/// converted back to network order when generating a DNS message.
typedef struct {
    uint16_t id;                ///< ID, host byte order
    uint16_t flags;             ///< Flags, host byte order
    uint16_t count_questions;   ///< Number of questions, host byte order
    uint16_t count_answer;      ///< Number of answer RRs, host byte order
    uint16_t count_authority;   ///< Number of authority RRs, host byte order
    uint16_t count_additional;  ///< Number of additional RRs, host byte order
} egpoc_dns_header_t;

/// @struct egpoc_dns_question_t
/// @brief DNS question stored in *host byte order*.
///
/// The `name` pointer references decoded (decompressed) bytes. The `type` and
/// `class_` values are provided in host byte order.
typedef struct {
    uint8_t const* name;       ///< Decoded domain name (labels, no compression)
    uint16_t       name_size;  ///< Size of the decoded name
    uint16_t       type;       ///< Record type, host byte order
    uint16_t       class_;     ///< Class, host byte order
} egpoc_dns_question_t;

/// @struct egpoc_dns_record_t
/// @brief DNS resource record stored in *host byte order*.
///
/// The RDATA pointer gives access to the raw record data. All integer fields
/// (type, class, TTL, size) are in host byte order.
typedef struct {
    uint8_t const* name;              ///< Decoded domain name
    uint16_t       name_size;         ///< Size of the decoded name
    uint16_t       type;              ///< Record type, host byte order
    uint16_t       class_;            ///< Record class, host byte order
    uint32_t       time_to_live;      ///< TTL in seconds, host byte order
    uint8_t const* record_data;       ///< Raw RDATA contents
    uint16_t       record_data_size;  ///< RDATA length, host byte order
} egpoc_dns_record_t;

/// @brief Error codes returned by DNS message parsing functions.
/// Success is 0; errors are negative.
typedef enum {
    egpoc_dns_error_none        = 0,   ///< No error.
    egpoc_dns_error_argument    = -1,  ///< Invalid argument (NULL, bad size, etc.).
    egpoc_dns_error_truncated   = -2,  ///< Message ended prematurely.
    egpoc_dns_error_label       = -3,  ///< Malformed DNS name label
    egpoc_dns_error_compression = -4,  ///< Malformed DNS compression pointer (potential loop or forward pointer)
    egpoc_dns_error_qdcount     = -5,  ///< Unsupported question count.
    egpoc_dns_error_callback    = -6,  ///< User callback aborted processing.
    egpoc_dns_error_internal    = -7,  ///< Internal inconsistency or impossible condition.
} egpoc_dns_error_t;

/// @brief Callback invoked when the DNS header has been fully parsed.
///
/// All fields inside @p header are provided in host byte order.
///
/// The callback must return one of the values of egpoc_dns_error_t:
/// - egpoc_dns_error_none   — Continue parsing the DNS message.
/// - egpoc_dns_error_callback — Stop parsing immediately as requested by
///   the callback/user code.
/// - Any other error code — Abort parsing and report the error upstream.
///
/// @note The callback must not store or retain pointers into @p header,
///       as the underlying storage may be transient.
///
/// @param user_data User-supplied context pointer.
/// @param header Pointer to the parsed DNS header (host byte order).
/// @return An egpoc_dns_error_t controlling parser continuation.
typedef int (*egpoc_dns_message_read_cb_header_t)(void* user_data, egpoc_dns_header_t const* header);

/// @brief Callback invoked for each parsed DNS question section.
///
/// All fields inside @p question are in host byte order.
///
/// The callback must return a value from egpoc_dns_error_t:
/// - egpoc_dns_error_none — Continue parsing the next question or record.
/// - egpoc_dns_error_callback — Stop parsing immediately as requested
///   by the callback.
/// - Any other error code — Abort parsing and report an error.
///
/// @note The callback must not keep pointers into @p question, as the
///       backing storage may be temporary.
///
/// @param user_data User-supplied context pointer.
/// @param question Pointer to the parsed DNS question (host byte order).
/// @return An egpoc_dns_error_t controlling the parser’s behavior.
typedef int (*egpoc_dns_message_read_cb_question_t)(void* user_data, egpoc_dns_question_t const* question);

/// @brief Callback invoked for each parsed DNS resource record.
///
/// All fields inside @p record are in host byte order.
/// RDATA is supplied as raw bytes; interpretation depends on the record type.
///
/// The callback must return a value from egpoc_dns_error_t:
/// - egpoc_dns_error_none — Continue parsing subsequent records.
/// - egpoc_dns_error_callback — Stop parsing immediately.
/// - Any other error code — Abort parsing and propagate the error.
///
/// @note The callback must not retain pointers into @p record or its
///       internal buffers, as these may be temporary.
///
/// @param user_data User-supplied context pointer.
/// @param record Pointer to the parsed DNS resource record (host byte order).
/// @return An egpoc_dns_error_t directing parser control flow.
typedef int (*egpoc_dns_message_read_cb_record_t)(void* user_data, egpoc_dns_record_t const* record);

/// @brief Skip past a DNS name in a raw DNS message.
///
/// Advances over a domain name encoded in DNS label format, including
/// support for compression pointers (RFC 1035 section 4.1.4).
///
/// This function does not allocate or copy any name data; it simply
/// determines how many bytes must be skipped to move past the encoded name.
///
/// @param message        Pointer to the raw DNS message buffer.
/// @param message_size   Size of the DNS message buffer in bytes.
/// @param message_index  Offset in the buffer where the name begins.
/// @return The index immediately after the encoded name on success,
///         or a negative egpoc_dns_error_t value on failure
int egpoc_dns_name_skip(uint8_t const* message, int message_size, int message_index);

/// @brief Read and fully decompress a DNS name into a caller-supplied buffer.
///
/// Follows compression pointers and reconstructs the name as a flat sequence
/// of raw label bytes without zero-termination. The output format is the
/// raw DNS label representation:
///     [len][label][len][label]...[0]
///
/// The caller must supply a buffer large enough to hold the resulting name.
/// No byte-order conversion is performed because domain names are sequences
/// of bytes, not multi-byte integer fields.
///
/// @param message         Pointer to the raw DNS message buffer.
/// @param message_size    Size of the DNS message buffer.
/// @param message_index   Offset where the encoded name begins.
/// @param name            Output buffer for the decompressed name.
/// @param name_capacity   Capacity of @p name in bytes.
/// @return The number of bytes written to @p name on success,
///         or a negative egpoc_dns_error_t value on error
int egpoc_dns_name_read(uint8_t const* message, int message_size, int message_index, uint8_t* name, int name_capacity);

/// @brief Parse an entire DNS message and emit host-byte-order structures.
///
/// This function parses a complete DNS wire-format message and emits:
/// - The DNS header (once)
/// - The DNS question (once)
/// - All resource records (answers, authority, additional)
///
/// All multi-byte numeric fields (ID, flags, counts, type, class, TTL, RDLENGTH, etc.)
/// are converted to host byte order before being passed to callbacks.
///
/// Name fields are always fully decompressed before being provided.
///
/// The parser calls the provided callbacks in strict wire order. The return
/// value of each callback must be an egpoc_dns_error_t:
/// - egpoc_dns_error_none   — parsing continues
/// - egpoc_dns_err_callback — parsing stops immediately as requested
/// - any other error        — parsing aborts and the same error code is returned
///
/// @param message               Pointer to the raw DNS message.
/// @param message_size          Size of @p message in bytes.
/// @param handle_header         Callback for the parsed header (may be NULL).
/// @param handle_header_user    User pointer passed to @p handle_header.
/// @param handle_question       Callback for each parsed question (may be NULL).
/// @param handle_question_user  User pointer passed to @p handle_question.
/// @param handle_record         Callback for each parsed resource record (may be NULL).
/// @param handle_record_user    User pointer passed to @p handle_record.
/// @return egpoc_dns_ok on full success,
///         egpoc_dns_err_callback if a callback halted parsing,
///         or another negative egpoc_dns_error_t on parse failure.
///
/// @note Classic DNS over UDP is limited to 512 bytes unless EDNS(0)
///       provides a larger acceptable message size.
int egpoc_dns_message_read(uint8_t const*                       message,
                           int                                  message_size,
                           egpoc_dns_message_read_cb_header_t   handle_header,
                           void*                                handle_header_user,
                           egpoc_dns_message_read_cb_question_t handle_question,
                           void*                                handle_question_user,
                           egpoc_dns_message_read_cb_record_t   handle_record,
                           void*                                handle_record_user);

#endif  // EGPOC_NETWORK_H_
