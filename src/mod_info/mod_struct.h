#ifndef __MOD_STRUCT__
#define __MOD_STRUCT__

#include <stdint.h>

#define _MK 0x2E4B2E4D
#define FILE_FORMAT_TAG_OFFSET  0x438
#define BYTES_PER_NOTE  4
#define NUM_OF_CHANNELS 4
#define ROWS_IN_PATTERN 64

struct sample_info
{
    char title[22];
    uint16_t sample_length_bytes;
    signed char finetune;
    unsigned char linear_vol;
    uint16_t repeat_offset;
    uint16_t repeat_length;
};

struct note_data
{
    uint8_t note[BYTES_PER_NOTE];
};

struct pattern_row_data
{
    struct note_data channel[NUM_OF_CHANNELS];
};

struct pattern
{
    struct pattern_row_data row[ROWS_IN_PATTERN];
};

struct mod_header
{
    char song_title[20];
    struct sample_info sample_info[31];
    uint8_t num_of_patterns;
    uint8_t end_pos_jump;
    uint8_t pattern_table[128];
    uint32_t file_format_tag;
};

typedef struct mod_file_t
{
    struct mod_header header;
    struct pattern pattern[256];
}mod_file_t;

#endif