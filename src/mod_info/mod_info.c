#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <byteswap.h>
#include "mod_struct.h"

mod_file_t mod_file;
static unsigned long sample_length_sum = 0;
static unsigned long file_size = 0;

static void mod_big_endian_to_little(struct sample_info *sample_nfo)
{
    sample_nfo->sample_length_bytes = __bswap_16(sample_nfo->sample_length_bytes) * 2;
    sample_nfo->repeat_offset = __bswap_16(sample_nfo->repeat_offset) * 2;
    sample_nfo->repeat_length = __bswap_16(sample_nfo->repeat_length) * 2;
}

static void mod_print_sample_info(struct sample_info *sample_nfo)
{
    printf("##################_SAMPLE INFO_##################\n");
    printf("    Sample name:    %s\n", sample_nfo->title);
    printf("    Sample lengh:   %u bytes\n", sample_nfo->sample_length_bytes);
    printf("    Sample finetune %i\n", sample_nfo->finetune);
    printf("    Sample volume   %u\n", sample_nfo->linear_vol);
    printf("    Sample r_offset %u\n", sample_nfo->repeat_offset);
    printf("    Sample r_length %u\n", sample_nfo->repeat_length);
    printf("######################_END_######################\n\n");
}

static void mod_print_one_pattern(struct pattern *_pattern)
{
    for(int i = 0; i < ROWS_IN_PATTERN; i++)
    {
        printf("    %02x  |   %02x %02x %02x %02x   %02x %02x %02x %02x     %02x %02x %02x %02x     %02x %02x %02x %02x\n",
            i,
            _pattern->row[i].channel[0].note[0], _pattern->row[i].channel[0].note[1], _pattern->row[i].channel[0].note[2], _pattern->row[i].channel[0].note[3], 
            _pattern->row[i].channel[1].note[0], _pattern->row[i].channel[1].note[1], _pattern->row[i].channel[1].note[2], _pattern->row[i].channel[1].note[3],
            _pattern->row[i].channel[2].note[0], _pattern->row[i].channel[2].note[1], _pattern->row[i].channel[2].note[2], _pattern->row[i].channel[2].note[3],
            _pattern->row[i].channel[3].note[0], _pattern->row[i].channel[3].note[1], _pattern->row[i].channel[3].note[2], _pattern->row[i].channel[3].note[3]);
    }
}

int main(int argc, char **argv)
{
    FILE *file;

    printf("#-------------------------------------------------------#\n");
    printf("#---------------------Mod file info---------------------#\n");
    printf("#-------------------------------------------------------#\n\n");

    if(argc <= 1)
    {
        printf("No file path provided!\n");
        return -1;
    }

    printf("Loading file: %s\n\n", argv[1]);

    if(NULL == (file = fopen(argv[1], "rb")))
    {
        printf("fopen fail!\n");
        return -12;
    }

    // Get file size in bytes
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    printf("File size is: %lu bytes\n", file_size);
    
    // Read .mod file mod_file
    if(0 == fread(&mod_file, sizeof(struct mod_header), 1, file))
    {
        printf("fread fail!\n");
        return -3;
    }

    // Check if .mod file has a M.K. initials
    if(mod_file.header.file_format_tag != _MK)
    {
        printf("File has no \"M.K\" initials, exiting...\n");
        return -4;
    }

    for(int i = 0; i < 31; i++)
    {
        mod_big_endian_to_little(&mod_file.header.sample_info[i]);
        mod_print_sample_info(&mod_file.header.sample_info[i]);
        sample_length_sum += mod_file.header.sample_info[i].sample_length_bytes;
    }

    printf("\nNumber of patterns: %d\n", mod_file.header.num_of_patterns);
    printf("End jump position: %d\n", mod_file.header.end_pos_jump);

    printf("Patern table: ");
    for(int i = 0; i < mod_file.header.num_of_patterns; i++)
        printf(" %d ", mod_file.header.pattern_table[i]);
    printf("\n");

    unsigned long patterns_length = file_size - sizeof(struct mod_header) - sample_length_sum;

    printf("Sample length sum:  %lu bytes\n", sample_length_sum);
    printf("Patterns length:    %lu bytes\n", patterns_length);

    printf("Size of pattern: %lu\n", sizeof(struct pattern));

    if(0 == fread(&mod_file.pattern, sizeof(struct pattern), patterns_length / sizeof(struct pattern), file))
    {
        printf("fread fail!\n");
        return -4;
    }

    mod_print_one_pattern(&mod_file.pattern[0]);

    printf("\n");
    return 0;
}
