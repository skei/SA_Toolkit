
/*
    A small header-only json library for C. The key unique feature is that it
    allows use of the same code to serialize as well as deserialize, greatly
    reducing boiler-plate code required. The library also does not make any
    allocations.

    The library API was inspired by the kv implementation found in the
    cute framework (https://github.com/RandyGaul/cute_framework).

    For usage example see: https://github.com/stbachmann/xjson

    LICENSE
    =======
    zlib/libpng license

    Copyright (c) 2021 Stefan Bachmann

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/
#ifndef XJSON_H
#define XJSON_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__MACH__)
#include <stdlib.h>
#else 
#include <malloc.h>
#endif

#include <inttypes.h> // for PRIi64 macros

#include <assert.h>
#define XJSON_ASSERT(c) assert(c)

#define XJSON_LOG(s) puts(s)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xjson xjson;
typedef enum xjson_state
{
    XJSON_STATE_UNITIALIZED = 0,
    XJSON_STATE_READ,
    XJSON_STATE_WRITE
} xjson_state;

/* Sets xjson to read-mode using the string pointed to by json_str up to length len */
void xjson_setup_read(xjson* json, const char* json_str, size_t len);
/* Sets xjson to write-mode and the json is written to buffer. If pretty_print is set to true, it'll produce a more readable output */
void xjson_setup_write(xjson* json, bool pretty_print, char* buffer, size_t len);
/* Sets a custom string allocator method. Expects that the returned char* is zero-terminated! */
void xjson_set_string_allocator(xjson* json, char* (*string_allocator)(const char* str, size_t size, void* mem_ctx));

/* Returns either XJSON_STATE_READ or XJSON_STATE_WRITE */
xjson_state xjson_get_state(xjson* json);

/* Begins a json object scope, all future value calls will use this object until a new scope is introduced */
void xjson_object_begin(xjson* json, const char* key);
/* Ends a json object scope */
void xjson_object_end(xjson* json);
/* Begins an array, all future value calls will use this array until a new scope is introduced */
void xjson_array_begin(xjson* json, const char* key);
/* Ends an array */
void xjson_array_end(xjson* json);
/* Will return true if an array end has been reached. Use this to parse/write an array with loops */
bool xjson_array_reached_end(xjson* json, int counter, int size);
/* Reads/Writes just the key, it means next value call should not supply a key (nullptr). Useful for hashmaps */
void xjson_key(xjson* json, const char** key);

/* Read/write integer types */
void xjson_u8(xjson* json, const char* key, uint8_t* val);
void xjson_u16(xjson* json, const char* key, uint16_t* val);
void xjson_u32(xjson* json, const char* key, uint32_t* val);
void xjson_u64(xjson* json, const char* key, uint64_t* val);
void xjson_i8(xjson* json, const char* key, int8_t* val);
void xjson_i16(xjson* json, const char* key, int16_t* val);
void xjson_i32(xjson* json, const char* key, int32_t* val);
void xjson_i64(xjson* json, const char* key, int64_t* val);

/* Read/write floating-point types */
void xjson_float(xjson* json, const char* key, float* val);
void xjson_double(xjson* json, const char* key, double* val);

/* Read/write bool */
void xjson_bool(xjson* json, const char* key, bool* val);

/* Read/write a string */
void xjson_string(xjson* json, const char* key, const char** str);

typedef enum xjson_int_type
{
    XJSON_INT_TYPE_U8,
    XJSON_INT_TYPE_U16,
    XJSON_INT_TYPE_U32,
    XJSON_INT_TYPE_U64,
    XJSON_INT_TYPE_I8,
    XJSON_INT_TYPE_I16,
    XJSON_INT_TYPE_I32,
    XJSON_INT_TYPE_I64
} xjson_int_type;

typedef struct xjson
{
    // Will be passed to the string allocator function
    void* mem_ctx;
    // Current state of this xjson object. Is XJSON_STATE_UNITIALIZED by default
    xjson_state mode;
    // Will output json with newline/tab.
    bool pretty_print;
    int intendation;

    // These point to the beginning/end and current location in either the write or read buffer
    uint8_t* current;
    uint8_t* end;
    uint8_t* start;

    // The custom string allocator function
    char* (*string_allocator)(const char* str, size_t size, void* mem_ctx);

    // Error handling. Set to true on error + appropriate message in error_message.
    bool error;
    char error_message[256];
} xjson;

#ifdef __cplusplus
}
#endif

#endif // XJSON_H

#ifdef XJSON_H_IMPLEMENTATION
//----------------------------------------------------------------------------------
// API implementation
//----------------------------------------------------------------------------------
void xjson_error(xjson* json, const char* message)
{
    json->error = true;
    if(json->mode == XJSON_STATE_READ)
    {
        // get line number and line index
        int line = 0;
        int line_begin = 0;

        for (int i=0; i < json->current-json->start; i++)
        {
            if (*(json->start+i) == '\n')
            {
                line++;
                line_begin = i + 1;
            }
        }

        int character_offset = json->current-(json->start+line_begin);
        uint8_t* sample_range_start = json->current - 8 < json->start ? json->start : json->current - 8;
        uint8_t* sample_range_end = json->current + 8 > json->end ? json->end : json->current + 8;
        // print formatted line data
        sprintf(json->error_message, "Error (%i, %i): %s\n\t%.*s\n\t%.*s^", line, character_offset, message, sample_range_end-sample_range_start, sample_range_start, json->current-sample_range_start, "-------------");
    }
    else
    {
        strcpy(json->error_message, message);
    }
}

bool xjson_is_white_space(char c)
{
    return (c == ' ') ||
		(c == '\t') ||
		(c == '\n') ||
		(c == '\v') ||
		(c == '\f') ||
		(c == '\r');
}

char xjson_lookback(xjson* json)
{
    if(json->error) return 0;
    if(json->current == json->start) return 0;

    uint8_t* ptr = json->current - 1;
    while(ptr > json->start && xjson_is_white_space(*ptr))
    {
        ptr--;
    }

    return ptr > json->start ? *ptr : 0;
}

char xjson_consume(xjson* json)
{
    if(json->error) return 0;

    if(json->current == json->end)
        return 0;

    // Consume any white space that might be there
    char c = *(++json->current);
    while(xjson_is_white_space(c))
    {
        if(json->current == json->end)
            return 0;
        c = *(++json->current);
    }

    return c;
}

void xjson_try(xjson* json, char expected_character)
{
    if(json->error) return;

    if(*json->current == expected_character)
        xjson_consume(json);
}

void xjson_expect(xjson* json, char expected_character)
{
    if(json->error) return;

    if(*json->current != expected_character){
        xjson_error(json, "Unexpected token found.");
        return;
    }

    xjson_consume(json);
}

void xjson_expect_token(xjson* json, const char* token, size_t len)
{
    if(json->error) return;

    for(int i=0; i<len; i++){
        if(*(json->current+i) != token[i]) {
           xjson_error(json, "Unexpected token found.");
           return;
        }
    }

    json->current += len;
    if(xjson_is_white_space(*json->current)){
        xjson_consume(json);
    }
}

void xjson_expect_key(xjson* json, const char* key)
{
    xjson_expect(json, '\"');
    if(json->error) return;

    // We find the size of the key and advance the current pointer to the end of key
    const uint8_t* key_start = json->current;
    while(*json->current != '\"') {
        xjson_consume(json);
        if(json->error) return;
    }
    size_t key_len = json->current - key_start;

    // Then we compare the two to make sure they match
    if(memcmp(key, key_start, key_len) != 0){
        xjson_error(json, "Expected key does not match.");
        return;
    }

    xjson_expect(json, '\"');
    xjson_expect(json, ':');
}

void xjson_expect_and_parse_int(xjson* json, int64_t* out_value)
{
    if(json->error) return;

    uint8_t* end_ptr;
    int64_t value = strtoll((char*)json->current, (char**)&end_ptr, 10);
    if(json->current == end_ptr)
    {
        // This means we couldn't parse the integer properly.
        xjson_error(json, "Invalid integer found. Couldn't parse value.");
        return;
    }
    *out_value = value;

    // Move pointer to end of number value + ensure all white space is consumed
    json->current = end_ptr;
    if(xjson_is_white_space(*json->current)){
        xjson_consume(json);
    }
}

void xjson_expect_and_parse_double(xjson* json, double* out_value)
{
    if(json->error) return;

    uint8_t* end_ptr;
    double value = strtod((char*)json->current, (char**)&end_ptr);
    if(json->current == end_ptr)
    {
        // This means we couldn't parse the integer properly.
        xjson_error(json, "Invalid double found. Couldn't parse value.");
        return;
    }
    *out_value = value;

    // Move pointer to end of number value
    json->current = end_ptr;
    if(xjson_is_white_space(*json->current)){
        xjson_consume(json);
    }
}

void xjson_expect_and_parse_string(xjson* json, const char** str)
{
    xjson_expect(json, '\"');
    if(json->error) return;

    uint8_t* str_start = json->current;

    while(*json->current != '\"')
    {
        xjson_consume(json);
        if(json->error) return;
    }

    size_t str_len = json->current - str_start;
    *str = json->string_allocator((char*)str_start, str_len, json->mem_ctx);

    xjson_expect(json, '\"');
}

void xjson_expect_and_parse_bool(xjson* json, bool* val)
{
    if(json->error) return;

    if(*json->current == 't')
    {
        xjson_expect_token(json, "true", 4);
        if(!json->error)
            *val = true;
    }
    else if(*json->current == 'f')
    {
        xjson_expect_token(json, "false", 5);
        if(!json->error)
            *val = false;
    }
    else
    {
        xjson_error(json, "Unexpected token whilst parsing bool.");
    }
}

void xjson_print_token(xjson* json, const char* token, size_t len)
{
    if(json->error) return;

    if(json->current + len > json->end)
    {
       xjson_error(json, "Write buffer is too small to write to. Abort.");
    }

    strncpy((char*)json->current, token, len);
    json->current += len;
}

void xjson_print_key(xjson* json, const char* key)
{
    if(json->error) return;
    xjson_print_token(json, "\"", 1);
    xjson_print_token(json, key, strlen(key));
    xjson_print_token(json, "\"", 1);
    xjson_print_token(json, ":", 1);
}

void xjson_print_new_line(xjson* json)
{
    if(json->error) return;
    xjson_print_token(json, "\n", 1);
    for(int i=0; i<json->intendation; i++)
    {
        xjson_print_token(json, "\t", 1);
    }
}

void xjson_setup_read(xjson* json, const char* str, size_t len)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(str);

    json->start = (uint8_t*)str;
    json->current = (uint8_t*)str;
    json->end = (uint8_t*)(str+len);
    json->mode = XJSON_STATE_READ;
}

void xjson_setup_write(xjson* json, bool pretty_print, char* buffer, size_t len)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(buffer);

    json->pretty_print = pretty_print;
    json->start = (uint8_t*)buffer;
    json->current = (uint8_t*)buffer;
    json->end = (uint8_t*)(buffer+len);
    json->mode = XJSON_STATE_WRITE;
}

void xjson_set_string_allocator(xjson* json, char* (*string_allocator)(const char* str, size_t size, void* mem_ctx))
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(string_allocator);

    json->string_allocator = string_allocator;
}

xjson_state xjson_get_state(xjson* json)
{
    XJSON_ASSERT(json);

    return json->mode;
}

void xjson_object_begin(xjson* json, const char* key)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->error) return;

    if(json->mode == XJSON_STATE_READ){
        if(key != NULL){
            xjson_expect_key(json, key);
        }

        xjson_expect(json, '{');
    }
    else {
        if(key != NULL){
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':' && json->intendation != 0)
        {
            xjson_print_new_line(json);
        }

        xjson_print_token(json, "{", 1);
    }
    json->intendation += 1;
}

void xjson_object_end(xjson* json)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);
    XJSON_ASSERT(json->intendation > 0);

    if(json->error) return;

    json->intendation -= 1;

    if(json->mode == XJSON_STATE_READ)
    {
        xjson_expect(json, '}');
        xjson_try(json, ',');
    }
    else
    {
        // This is to overwrite the previous ',' insertion
        json->current--;

        // Special case for empty objects;
        if(*json->current == '{')
            json->current++;

        if(json->pretty_print) xjson_print_new_line(json);
        xjson_print_token(json, "}", 1);
        xjson_print_token(json, ",", 1);
    }

    // Special case for closing the root object, null-terminate the output string
    if(json->intendation == 0 && json->mode == XJSON_STATE_WRITE)
    {
        *(--json->current) = '\0';
    }
}

void xjson_array_begin(xjson* json, const char* key)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->error) return;

    if(json->mode == XJSON_STATE_READ)
    {
        if(key != NULL){
            xjson_expect_key(json, key);
        }

        xjson_expect(json, '[');
    }
    else
    {
        if(key != NULL){
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':')
        {
            xjson_print_new_line(json);
        }

        xjson_print_token(json, "[", 1);
    }
    json->intendation += 1;
}

void xjson_array_end(xjson* json)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);
    XJSON_ASSERT(json->intendation > 0);

    if(json->error) return;

    json->intendation -= 1;

    if(json->mode == XJSON_STATE_READ)
    {
        xjson_expect(json, ']');
        xjson_try(json, ',');
    }
    else
    {
        json->current--;

        // Special case for empty arrays;
        if(*json->current == '[')
            json->current++;

        if(json->pretty_print) xjson_print_new_line(json);
        xjson_print_token(json, "]", 1);
        xjson_print_token(json, ",", 1);
    }
}

// TODO: Find a better way to handle this. It's not very nice :(
bool xjson_array_reached_end(xjson* json, int current, int size)
{
    if(json->mode == XJSON_STATE_READ)
    {
        if(*json->current == ']' || json->error)
            return true;

        return false;
    }

    return current >= size;
}

void xjson_key(xjson* json, const char** key)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->mode == XJSON_STATE_READ)
    {
        xjson_expect_and_parse_string(json, key);
        xjson_expect(json, ':');
    }
    else
    {
        if(json->pretty_print) xjson_print_new_line(json);
        xjson_print_key(json, *key);
    }
}

void xjson_integer(xjson* json, const char* key, void* val, xjson_int_type type)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);
    XJSON_ASSERT(val);

    if(json->mode == XJSON_STATE_READ)
    {
        if(key != NULL)
        {
            xjson_expect_key(json, key);
        }

        int64_t number;
        xjson_expect_and_parse_int(json, &number);
        if(json->error) return;

        switch (type)
        {
        case XJSON_INT_TYPE_U8:
            *(uint8_t*)val = (uint8_t)number;
            break;
        case XJSON_INT_TYPE_U16:
            *(uint16_t*)val = (uint16_t)number;
            break;
        case XJSON_INT_TYPE_U32:
            *(uint32_t*)val = (uint32_t)number;
            break;
        case XJSON_INT_TYPE_U64:
            *(uint64_t*)val = (uint64_t)number;
            break;
        case XJSON_INT_TYPE_I8:
            *(int8_t*)val = (int8_t)number;
            break;
        case XJSON_INT_TYPE_I16:
            *(int16_t*)val = (int16_t)number;
            break;
        case XJSON_INT_TYPE_I32:
            *(int32_t*)val = (int32_t)number;
            break;
        case XJSON_INT_TYPE_I64:
            *(int64_t*)val = (int64_t)number;
            break;
        default:
            // TODO: error
            break;
        }

        xjson_try(json, ',');
    }
    else
    {
        if(key != NULL)
        {
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':')
        {
            xjson_print_new_line(json);
        }

        int len = 0;
        switch (type)
        {
        case XJSON_INT_TYPE_U8:
            len = sprintf((char*)json->current, "%" PRIu8, *(uint8_t*)val);
            break;
        case XJSON_INT_TYPE_U16:
            len = sprintf((char*)json->current, "%" PRIu16, *(uint16_t*)val);
            break;
        case XJSON_INT_TYPE_U32:
            len = sprintf((char*)json->current, "%" PRIu32, *(uint32_t*)val);
            break;
        case XJSON_INT_TYPE_U64:
            len = sprintf((char*)json->current, "%" PRIu64, *(uint64_t*)val);
            break;
        case XJSON_INT_TYPE_I8:
            len = sprintf((char*)json->current, "%" PRIi8, *(int8_t*)val);
            break;
        case XJSON_INT_TYPE_I16:
            len = sprintf((char*)json->current, "%" PRIi16, *(int16_t*)val);
            break;
        case XJSON_INT_TYPE_I32:
            len = sprintf((char*)json->current, "%" PRIi32, *(int32_t*)val);
            break;
        case XJSON_INT_TYPE_I64:
            len = sprintf((char*)json->current, "%" PRIi64, *(int64_t*)val);
            break;
        default:
            // TODO: error
            break;
        }
        json->current += len;

        xjson_print_token(json, ",", 1);
    }
}

void xjson_u8(xjson* json, const char* key, uint8_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_U8);
}

void xjson_u16(xjson* json, const char* key, uint16_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_U16);
}

void xjson_u32(xjson* json, const char* key, uint32_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_U32);
}

void xjson_u64(xjson* json, const char* key, uint64_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_U64);
}

void xjson_i8(xjson* json, const char* key, int8_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_I8);
}

void xjson_i16(xjson* json, const char* key, int16_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_I16);
}

void xjson_i32(xjson* json, const char* key, int32_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_I32);
}

void xjson_i64(xjson* json, const char* key, int64_t* val)
{
    xjson_integer(json, key, val, XJSON_INT_TYPE_I64);
}

void xjson_float(xjson* json, const char* key, float* val)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->mode == XJSON_STATE_READ)
    {
        if(key != NULL)
        {
            xjson_expect_key(json, key);
        }

        double out;
        xjson_expect_and_parse_double(json, &out);
        if(!json->error)
            *val = (float)out;

        xjson_try(json, ',');
    }
    else
    {
        if(key != NULL)
        {
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':')
        {
            xjson_print_new_line(json);
        }

        int len = sprintf((char*)json->current, "%f", *val);
        json->current += len;

        xjson_print_token(json, ",", 1);
    }
}

void xjson_double(xjson* json, const char* key, double* val)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->mode == XJSON_STATE_READ)
    {
        if(key != NULL)
        {
            xjson_expect_key(json, key);
        }

        xjson_expect_and_parse_double(json, val);
        xjson_try(json, ',');
    }
    else
    {
        if(key != NULL)
        {
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':')
        {
            xjson_print_new_line(json);
        }

        int len = sprintf((char*)json->current, "%f", *val);
        json->current += len;

        xjson_print_token(json, ",", 1);
    }
}

void xjson_bool(xjson* json, const char* key, bool* val)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->mode == XJSON_STATE_READ)
    {
        if(key != NULL)
        {
            xjson_expect_key(json, key);
        }

        xjson_expect_and_parse_bool(json, val);
        xjson_try(json, ',');
    }
    else
    {
        if(key != NULL)
        {
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':')
        {
            xjson_print_new_line(json);
        }

        int len = sprintf((char*)json->current, "%s", *val == true ? "true" : "false");
        json->current += len;

        xjson_print_token(json, ",", 1);
    }
}

void xjson_string(xjson* json, const char* key, const char** str)
{
    XJSON_ASSERT(json);
    XJSON_ASSERT(json->mode != XJSON_STATE_UNITIALIZED);

    if(json->mode == XJSON_STATE_READ)
    {
        if(key != NULL)
        {
            xjson_expect_key(json, key);
        }

        xjson_expect_and_parse_string(json, str);
        xjson_try(json, ',');
    }
    else
    {
        if(key != NULL)
        {
            if(json->pretty_print) xjson_print_new_line(json);
            xjson_print_key(json, key);
        }
        else if(json->pretty_print && xjson_lookback(json) != ':')
        {
            xjson_print_new_line(json);
        }

        int len = sprintf((char*)json->current, "\"%s\"", *str);
        json->current += len;

        xjson_print_token(json, ",", 1);
    }
}
#endif // XJSON_H_IMPLEMENTATION
