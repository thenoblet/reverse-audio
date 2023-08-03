#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Cannot open input file\n");
        return 2;
    }

    // Read header
    WAVHEADER bf;
    fread(&bf, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    if (!check_format(bf))
    {
        printf("File is not a WAV file\n");
        return 3;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Cannot open ouput file\n");
        return 4;
    }

    // Write header to file
    fwrite(&bf, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    int blockSize = get_block_size(bf);


    // Write reversed audio to file
    fseek(input, 0, SEEK_END);
    long int file_position = ftell(input);

    while (file_position > sizeof(WAVHEADER))
    {
        file_position -= blockSize;
        fseek(input, file_position, SEEK_SET);
        char *buffer = malloc(blockSize);
        fread(buffer, sizeof(char), blockSize, input);
        fwrite(buffer, sizeof(char), blockSize, output);
        free(buffer);
    }

    //closing files
    fclose(input);
    fclose(output);

    return 0;
}

// Check if file is of the WAV format
int check_format(WAVHEADER header)
{
    char wavformat[5] = "WAVE";
    if (strncmp((const char *)header.format, wavformat, 4) == 0)
    {
        return 1;
    }
    return 0;
}

//Get block size of the given WAV file
int get_block_size(WAVHEADER header)
{
    int bytePerSample = header.bitsPerSample / 8;
    int SizeOfBlock = header.numChannels * bytePerSample;
    return SizeOfBlock;
}