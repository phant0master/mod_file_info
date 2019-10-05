#ifndef __MOD_STRUCT__
#define __MOD_STRUCT__

#include <stdint.h>

#define _MK 0x2E4B2E4D
#define FILE_FORMAT_TAG_OFFSET 0x438

struct mod_sample_info
{
    char title[22];
    uint16_t sample_length_bytes;
    signed char finetune;
    unsigned char linear_vol;
    uint16_t repeat_offset;
    uint16_t repeat_length;
};

typedef struct mod_file_header_t
{
    char song_title[20];
    struct mod_sample_info sample_info[31];
    uint8_t num_of_patterns;
    uint8_t end_pos_jump;
    uint8_t pattern_table[128];

}mod_file_header_t;

#endif