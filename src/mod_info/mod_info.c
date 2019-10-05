#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <byteswap.h>
#include "mod_struct.h"

mod_file_header_t header;

static void mod_big_endian_to_little(struct mod_sample_info *sample_nfo)
{
    sample_nfo->sample_length_bytes = __bswap_16(sample_nfo->sample_length_bytes) * 2;
    sample_nfo->repeat_offset = __bswap_16(sample_nfo->repeat_offset) * 2;
    sample_nfo->repeat_length = __bswap_16(sample_nfo->repeat_length) * 2;
}

static void mod_print_sample_info(struct mod_sample_info *sample_nfo)
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

int main(int argc, char **argv)
{
    FILE *file;

    printf("----------------------Mod file info----------------------\n");
    printf("Loading file: %s\n\n", argv[1]);

    if(NULL == (file = fopen(argv[1], "rb")))
    {
        printf("fopen fail!\n");
        return -1;
    }

    uint32_t tmp = 0;    

    fseek(file, FILE_FORMAT_TAG_OFFSET, SEEK_SET);

    // Read .mod file initials
    if(0 == fread(&tmp, sizeof(uint32_t), 1, file))
    {
        printf("fread fail!\n");
        return -2;
    }

    // Check if .mod file has a M.K. initials
    if(tmp == _MK)
        fseek(file, 0x00, SEEK_SET);
    else
    {
        printf("File has no \"M.K\" initials, exiting...\n");
        return -3;
    }
    
    // Read .mod file header
    if(0 == fread(&header, sizeof(mod_file_header_t), 1, file))
    {
        printf("fread fail!\n");
        return -4;
    }

    for(int i = 0; i < 31; i++)
    {
        mod_big_endian_to_little(&header.sample_info[i]);
        mod_print_sample_info(&header.sample_info[i]);
    }

    printf("\nNumber of patterns: %d\n", header.num_of_patterns);
    printf("End jump position: %d\n", header.end_pos_jump);

    printf("Patern table: ");
    for(int i = 0; i < header.num_of_patterns; i++)
        printf(" %d ", header.pattern_table[i]);

    printf("\n");
    return 0;
}
