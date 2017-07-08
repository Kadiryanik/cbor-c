/**
 * \file
 *         Test file for CBOR decoder
 * \author
 *         Kadir Yanık      <eleqiac2@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>

#include "decoder.h"

extern int result_offset;
extern bool include_bytes;

#ifndef TEST
#define TEST 3
#endif /* TEST */

int main(int argc, char *argv[]){
    printf("Testing = %d\n\n", TEST);
#if TEST == 1
    //[[1, 2], [3, 4], [1, 1]]
    unsigned char data[] = { 0x83, 0x82, 0x01, 0x02, 0x82, 0x03, 0x04, 0x82, 0x01, 0x01};  
#elif TEST == 2
    //["ab", ["b", "ab", "c"]]  
    unsigned char data[] = {  0x82, 0x62, 0x61, 0x62, 0x83, 0x61, 0x62, 0x62, 0x61, 0x62, 0x61, 0x63 };
#elif TEST == 3
    //["a", {"b": "c"}, {3: "e", 4: "a"}] 
    unsigned char data[] = { 0x83, 0x61, 0x61, 0xa1, 0x61, 0x62, 0x61, 0x63, 0xa2, 0x03, 0x61, 0x65, 0x04, 0x61, 0x61};
#elif TEST == 4
    //{"a": 1, "b": [2, 3]}
    unsigned char data[] = { 0xa2, 0x61, 0x61, 0x01, 0x61, 0x62, 0x82, 0x02, 0x03 };
#elif TEST == 5
    //{"a": "A", "b": "B", "c": "C", "d": "D", "e": "E"} 
    unsigned char data[] = { 0xa5, 0x61, 0x61, 0x61, 0x41, 0x61, 0x62, 0x61, 0x42, 0x61, 0x63, 0x61, 0x43, 0x61, 0x64, 0x61, 0x44, 0x61, 0x65, 0x61, 0x45 };
#elif TEST == 6
    //{1: "a", 2: "b"} 
    unsigned char data[] = { 0xa2, 0x01, 0x61, 0x61, 0x02, 0x61, 0x62 };
#else
/*  little complex
        [
         [
          { 
           1: 4, 
           2: h'1', 
           3: "10", 
           4: [1, 5, 7], 
           5: h'6465616462', 
           -1: h'deadbeefcafedeadbeefcafedeadbeef'
          }, 
          {
           1: "41", 
           2: h'0', 
           3: 5, 
           4: [6, 8], 
           5: "deadb", 
           -1: h'deadbeefcafedeadbeefcafedeadbeef'
          }
         ]
        ]
*/
    unsigned char data[] = { 0x81,
                              0x82,
                               0xa6,
                                0x01, 0x04,
                                0x02, 0x41, 0x01,
                                0x03, 0x62, 0x31, 0x30,
                                0x04, 0x83, 0x01, 0x05, 0x07,
                                0x05, 0x45, 0x64, 0x65, 0x61, 0x64, 0x62,
                                0x20, 0x50, 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe, 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe, 0xde, 0xad, 0xbe, 0xef,
                               0xa6,
                                0x01, 0x62, 0x34, 0x31,
                                0x02, 0x41, 0x00,
                                0x03, 0x05,
                                0x04, 0x82, 0x06, 0x08,
                                0x05, 0x65, 0x64, 0x65, 0x61, 0x64, 0x62,
                                0x20, 0x50, 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe, 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe, 0xde, 0xad, 0xbe, 0xef
                            };
#endif /* TESTS */
    input_t in;
    init_input(&in, data, sizeof(data));
    
    initialize_decoder();
    decode(&in);

    printf("ResultSize = %d\n", result_offset);
    if(!include_bytes){
        printf("%.*s\n", result_offset, result);
    } else{
        int i;
        int printing_bytes = 0;
        for(i = 0; i < result_offset; i++){
            if(result[i] == 'h' && result[i + 1] == '\''){
                i+=2;
                printf("h'");
                printing_bytes = 1;
            }
            if(result[i] == '\''){
                printing_bytes = 0;
            }
            if(printing_bytes){
                printf("%x", result[i]);
            } else{
                printf("%c", result[i]);
            }
        }
        printf("\n");
    }
    return 0;
}