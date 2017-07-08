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
#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>

#include "input.h"

/* It will be counted if the other opens without closing one. */
#define EXPECTING_ARRAY 15 /* [1, 2], [3, 4], [5, 6] 	-> 1 array expecting
			      [1, [2, 3, [4,5]]] 	-> 3 array expecting */

#define EXPECTING_MAP 	15 /* Same as array */

#define EXPECTING_STATE 15 /* [] 		-> 1 state
                              [1, [1, 2]] 	-> 2 state
			      [[1, [2, 3]]] 	-> 3 state */

/* Static value used for embedded systems
   as you know, dynamic memory allocation
   is not avaible all platform.		*/
#define RESULT_SIZE 200

typedef enum {
    STATE_TYPE,
    STATE_PINT,
    STATE_NINT,
    STATE_BYTES_SIZE,
    STATE_BYTES_DATA,
    STATE_STRING_SIZE,
    STATE_STRING_DATA,
    STATE_ARRAY,
    STATE_MAP,
    STATE_TAG,
    STATE_ERROR
} enum_state;

enum_state state;
unsigned int current_length;

/* Holds result data which is human readable format */
unsigned char result[RESULT_SIZE];
int result_offset;

/* Holds array, map and state history */
uint8_t array_history[EXPECTING_ARRAY];
uint8_t map_history[EXPECTING_MAP];
uint8_t state_history[EXPECTING_STATE];

uint8_t array_offset;
uint8_t map_offset;
uint8_t state_offset;

/* Reading map_id or map_data */
bool map_reading_id;

/* For decide printing format */
bool include_bytes;

void initialize_decoder();
void put_text(unsigned char *data, int len);
void decode(input_t *in);
int set_history();

void on_integer(int value);
void on_bytes(unsigned char *data, int size);
void on_string(const char *data, size_t len);
void on_array(int size);
void on_map(int size);
void on_tag(unsigned int tag);
void on_special(unsigned int code);
void on_bool(bool);
void on_null();
void on_undefined();
void on_error(const char *error);
void on_extra_integer(unsigned long long value, int sign);
void on_extra_tag(unsigned long long tag);
void on_extra_special(unsigned long long tag);

#endif /* DECODER_H */