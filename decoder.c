/*
   Copyright 2014-2015 Stanislav Ovsyannikov
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
	   Unless required by applicable law or agreed to in writing, software
	   distributed under the License is distributed on an "AS IS" BASIS,
	   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	   See the License for the specific language governing permissions and
	   limitations under the License.

	decode()[run()] and on_*() functions are modified
	added some function for human readable format
*/

/**
 * \file
 *         Simple implementation of RFC-7049 (CBOR)
 * \author
 *         Kadir Yanık      <eleqiac2@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "decoder.h"

#define DEBUG 0

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void put_text(unsigned char *data, int len){
    memcpy(result + result_offset, data, len);
    result_offset += len;
}

void decode(input_t *in) {
    unsigned int temp;

    while(1) {
        if(state == STATE_TYPE) {
            if(has_bytes(in, 1)) {
                unsigned char type = get_byte(in);
                unsigned char major_type = type >> 5;
                unsigned char minor_type = (unsigned char) (type & 31);

                switch(major_type) {
                    case 0: // positive integer
                        if(minor_type < 24) {
                            on_integer(minor_type);
                        } else if(minor_type == 24) { // 1 byte
                            current_length = 1;
                            state = STATE_PINT;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_PINT;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_PINT;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_PINT;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported integer type");
                        }
                        break;
                    case 1: // negative integer
                        if(minor_type < 24) {
                            on_integer(-1 -minor_type);
                        } else if(minor_type == 24) { // 1 byte
                            current_length = 1;
                            state = STATE_NINT;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_NINT;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_NINT;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_NINT;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported integer type");
                        }
                        break;
                    case 2: // bytes
                        if(minor_type < 24) {
                            state = STATE_BYTES_DATA;
                            current_length = minor_type;
                        } else if(minor_type == 24) {
                            state = STATE_BYTES_SIZE;
                            current_length = 1;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_BYTES_SIZE;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_BYTES_SIZE;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_BYTES_SIZE;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported bytes type");
                        }
                        break;
                    case 3: // string
                        if(minor_type < 24) {
                            state = STATE_STRING_DATA;
                            current_length = minor_type;
                        } else if(minor_type == 24) {
                            state = STATE_STRING_SIZE;
                            current_length = 1;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_STRING_SIZE;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_STRING_SIZE;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_STRING_SIZE;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported string type");
                        }
                        break;
                    case 4: // array
                        if(minor_type < 24) {
                            on_array(minor_type);
                        } else if(minor_type == 24) {
                            state = STATE_ARRAY;
                            current_length = 1;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_ARRAY;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_ARRAY;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_ARRAY;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported array type");
                        }
                        break;
                    case 5: // map
                        if(minor_type < 24) {
                            on_map(minor_type);
                        } else if(minor_type == 24) {
                            state = STATE_MAP;
                            current_length = 1;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_MAP;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_MAP;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_MAP;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported array type");
                        }
                        break;
                    case 6: // tag
                        if(minor_type < 24) {
                            on_tag(minor_type);
                        } else if(minor_type == 24) {
                            state = STATE_TAG;
                            current_length = 1;
                        } else if(minor_type == 25) { // 2 byte
                            current_length = 2;
                            state = STATE_TAG;
                        } else if(minor_type == 26) { // 4 byte
                            current_length = 4;
                            state = STATE_TAG;
                        } else if(minor_type == 27) { // 8 byte
                            current_length = 8;
                            state = STATE_TAG;
                        } else {
                            state = STATE_ERROR;
                            on_error("unsupported tag type");
                        }
                        break;
                }
            } else break;
        } else if(state == STATE_PINT) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        on_integer(get_byte(in));
                        state = STATE_TYPE;
                        break;
                    case 2:
                        on_integer(get_short(in));
                        state = STATE_TYPE;
                        break;
                    case 4:
                        temp = get_int(in);
                        if(temp <= INT_MAX) {
                            on_integer(temp);
                        } else {
                            on_extra_integer(temp, 1);
                        }
                        state = STATE_TYPE;
                        break;
                    case 8:
                        on_extra_integer(get_long(in), 1);
                        state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(state == STATE_NINT) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        on_integer(-(int) get_byte(in));
                        state = STATE_TYPE;
                        break;
                    case 2:
                        on_integer(-(int) get_short(in));
                        state = STATE_TYPE;
                        break;
                    case 4:
                        temp = get_int(in);
                        if(temp <= INT_MAX) {
                            on_integer(-(int) temp);
                        } else if(temp == 2147483648u) {
                            on_integer(INT_MIN);
                        } else {
                            on_extra_integer(temp, -1);
                        }
                        state = STATE_TYPE;
                        break;
                    case 8:
                        on_extra_integer(get_long(in), -1);
                        break;
                }
            } else break;
        } else if(state == STATE_BYTES_SIZE) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        current_length = get_byte(in);
                        state = STATE_BYTES_DATA;
                        break;
                    case 2:
                        current_length = get_short(in);
                        state = STATE_BYTES_DATA;
                        break;
                    case 4:
                        current_length = get_int(in);
                        state = STATE_BYTES_DATA;
                        break;
                    case 8:
                        state = STATE_ERROR;
                        on_error("extra long bytes");
                        break;
                }
            } else break;
        } else if(state == STATE_BYTES_DATA) {
            if(has_bytes(in, current_length)) {
                unsigned char temp[current_length];
                unsigned char *data = temp;
                get_bytes(in, data, current_length);
                state = STATE_TYPE;
                on_bytes(data, current_length);
            } else break;
        } else if(state == STATE_STRING_SIZE) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        current_length = get_byte(in);
                        state = STATE_STRING_DATA;
                        break;
                    case 2:
                        current_length = get_short(in);
                        state = STATE_STRING_DATA;
                        break;
                    case 4:
                        current_length = get_int(in);
                        state = STATE_STRING_DATA;
                        break;
                    case 8:
                        state = STATE_ERROR;
                        on_error("extra long array");
                        break;
                }
            } else break;
        } else if(state == STATE_STRING_DATA) {
            if(has_bytes(in, current_length)) {
                unsigned char temp[current_length];
                unsigned char *data = temp;
                get_bytes(in, data, current_length);
                state = STATE_TYPE;
                on_string((const char *)data, (size_t)current_length);
            } else break;
        } else if(state == STATE_ARRAY) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        on_array(get_byte(in));
                        state = STATE_TYPE;
                        break;
                    case 2:
                        on_array(current_length = get_short(in));
                        state = STATE_TYPE;
                        break;
                    case 4:
                        on_array(get_int(in));
                        state = STATE_TYPE;
                        break;
                    case 8:
                        state = STATE_ERROR;
                        on_error("extra long array");
                        break;
                }
            } else break;
        } else if(state == STATE_MAP) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        on_map(get_byte(in));
                        state = STATE_TYPE;
                        break;
                    case 2:
                        on_map(current_length = get_short(in));
                        state = STATE_TYPE;
                        break;
                    case 4:
                        on_map(get_int(in));
                        state = STATE_TYPE;
                        break;
                    case 8:
                        state = STATE_ERROR;
                        on_error("extra long map");
                        break;
                }
            } else break;
        } else if(state == STATE_TAG) {
            if(has_bytes(in, current_length)) {
                switch(current_length) {
                    case 1:
                        on_tag(get_byte(in));
                        state = STATE_TYPE;
                        break;
                    case 2:
                        on_tag(get_short(in));
                        state = STATE_TYPE;
                        break;
                    case 4:
                        on_tag(get_int(in));
                        state = STATE_TYPE;
                        break;
                    case 8:
                        on_extra_tag(get_long(in));
                        state = STATE_TYPE;
                        break;
                }
            } else break;
        } else if(state == STATE_ERROR) {
            PRINTF("State Error!\n");
            break;
        } else {
            PRINTF("Unknown State!\n");
        }
    }
}

void initialize_decoder(){
    memset(result, 0, sizeof(unsigned char) * RESULT_SIZE);
    result_offset = 0;

    memset(array_history, 0, sizeof(uint8_t) * EXPECTING_ARRAY);
    memset(map_history, 0, sizeof(uint8_t) * EXPECTING_MAP);
    memset(state_history, 0, sizeof(uint8_t) * EXPECTING_STATE);

    array_offset = 0;
    map_offset = 0;
    state_offset = 0;

    map_reading_id = true;
    include_bytes = false;
}

int set_history(){
    int comes_from_goto = 0;
    START:
    if(state_history[state_offset - 1] == STATE_ARRAY){
        // one item
        if(array_history[array_offset - 1] > 0){
            array_history[array_offset - 1]--;
        }
        // is this state done AND if there another state
        if(array_history[array_offset - 1] == 0){
            array_offset--;
            state_offset--;
            // if there another state AND it's item not compilated
            if(state_offset - 1 >= 0){
                if((state_history[state_offset - 1] == STATE_MAP && 
                			map_history[map_offset - 1] - 1 > 0) ||
                    (state_history[state_offset - 1] == STATE_ARRAY && 
                    			array_history[array_offset - 1] - 1 > 0)){
                    put_text("], ", 3);
                    comes_from_goto = 1;
                    goto START;
                }
            }
            put_text("]", 1);
            if(state_offset - 1 < 0){
                return 1;
            } else{
                goto START;
            }
        }
    } else if(state_history[state_offset - 1] == STATE_MAP){
        if(map_reading_id){
            map_reading_id = false;
            put_text(": ", 2);
            return 1;
        } else{
            map_reading_id = true;

            // one item
            if(map_history[map_offset - 1] > 0){
                map_history[map_offset - 1]--;
            }
            // is this state done AND if there another state
            if(map_history[map_offset - 1] == 0){
                map_offset--;
                state_offset--;
                // if there another state AND it's item not compilated
                if(state_offset - 1 >= 0){
                    if((state_history[state_offset - 1] == STATE_MAP && 
                    				map_history[map_offset - 1] - 1 > 0) ||
                        (state_history[state_offset - 1] == STATE_ARRAY && 
                        			array_history[array_offset - 1] - 1 > 0)){
                        put_text("}, ", 3);
                        comes_from_goto = 1;
                        goto START;
                    }
                }
                put_text("}", 1);
                if(state_offset - 1 < 0){
                    return 1;
                } else{
                    goto START;
                }
            }
        }
    }
    if(comes_from_goto) return 1;
    return 0;
}

void on_integer(int value) {
    PRINTF("Found an integer: %d\n", value);

    if(value < 0){
        unsigned char temp = (-1 * value) + 48;
        put_text("-", 1);
        put_text(&temp, 1);
    } else{
        unsigned char temp = value + 48;
        put_text(&temp, 1);
    }

    if(!set_history()){
        put_text(", ", 2);
    }
}

void on_bytes(unsigned char *data, int size) {
    PRINTF("Found bytes with size: %d\n", size);
    include_bytes = true;

    put_text("h'", 2);
    put_text(data, size);
    put_text("'", 1);
    
    if(!set_history()){
        put_text(", ", 2);
    }
}

void on_string(const char *data, size_t len) {
    PRINTF("Found string: '%.*s'\n", (int)len, data);

    put_text("\"", 1);
    put_text((unsigned char *)data, (int)(len));
    put_text("\"", 1);

    if(!set_history()){
        put_text(", ", 2);
    }
}

void on_array(int size) {
    PRINTF("Found array: %d\n", size);
    array_history[array_offset++] = size;
    state_history[state_offset++] = STATE_ARRAY;
    put_text("[", 1);
}

void on_map(int size) {
    PRINTF("Found map: %d\n", size);
    map_history[map_offset++] = size;
    state_history[state_offset++] = STATE_MAP;
    put_text("{", 1);
}

void on_tag(unsigned int tag) {
    PRINTF("Found tag: %d\n", tag);
}

void on_special(unsigned int code) {
    PRINTF("Found special: %d\n", code);
}

void on_bool(bool value) {
    PRINTF("Found bool: %s\n", value ? "true" : "false");
}

void on_null() {
    PRINTF("Found special: null\n");
}

void on_undefined() {
    PRINTF("Found special: undefined\n");
}

void on_error(const char *error) {
    PRINTF("Error: %s\n", error);
}

void on_extra_integer(unsigned long long value, int sign) {
    if(sign < 0) {
        PRINTF("Found long long integer: -%llu\n", value);
    } else {
        PRINTF("Found long long integer: %llu\n", value);
    }
}

void on_extra_tag(unsigned long long tag) {
    PRINTF("Found long long tag: %llu\n", tag);
}

void on_extra_special(unsigned long long tag) {
    PRINTF("Found long long special: %llu\n", tag);
}
