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
	
	functions changed to out of class
*/

/*
 * \modified
 *         Kadir Yanık      <eleqiac2@gmail.com>
 */
#include <string.h>

#include "input.h"

void init_input(input_t *in, void *data, int size){
    in->data = (unsigned char *)data;
    in->size = size;
    in->offset = 0;
}

bool has_bytes(input_t *in, int count){
    return in->size - in->offset >= count;
}

unsigned char get_byte(input_t *in){
    return in->data[in->offset++];
}

unsigned short get_short(input_t *in){
    unsigned short value = ((unsigned short) in->data[in->offset] << 8) | ((unsigned short) in->data[in->offset + 1]);
    in->offset += 2;
    return value;
}

unsigned int get_int(input_t *in){
    unsigned int value = \
            ((unsigned int) in->data[in->offset    ] << 24) |
            ((unsigned int) in->data[in->offset + 1] << 16) |
            ((unsigned int) in->data[in->offset + 2] << 8 ) |
            ((unsigned int) in->data[in->offset + 3]);
    in->offset += 4;
    return value;
}

unsigned long long get_long(input_t *in){
    unsigned long long value = ((unsigned long long) in->data[in->offset] << 56) |
            ((unsigned long long) in->data[in->offset +1] << 48) | ((unsigned long long) in->data[in->offset +2] << 40) |
            ((unsigned long long) in->data[in->offset +3] << 32) | ((unsigned long long) in->data[in->offset +4] << 24) |
            ((unsigned long long) in->data[in->offset +5] << 16) | ((unsigned long long) in->data[in->offset +6] << 8 ) |
            ((unsigned long long) in->data[in->offset +7]);
    in->offset += 8;
    return value;
}

void get_bytes(input_t *in, void *to, int count) {
    memcpy(to, in->data + in->offset, count);
    in->offset += count;
}