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
#ifndef INPUT_H
#define INPUT_H

typedef enum { false, true } bool;

typedef struct input {
    unsigned char *data;
    int size;
    int offset;
}input_t;

void init_input(input_t *in, void *data, int size);
bool has_bytes(input_t *in, int count);
unsigned char get_byte(input_t *in);
unsigned short get_short(input_t *in);
unsigned int get_int(input_t *in);
unsigned long long get_long(input_t *in);
void get_bytes(input_t *in, void *to, int count);

#endif /* INPUT_H */