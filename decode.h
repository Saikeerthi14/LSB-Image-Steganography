#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_OUTPUT_BUF_SIZE 25
#define MAX_FILE_SUFFIX 4


typedef struct _decodeInfo
{
    char *image_file;
    FILE *fptr_image_file;
    char *output_data_file;
    FILE *fptr_output_file;
    char magic_string[2];
    int size_extn_output_file;
    char extn_output_file[MAX_FILE_SUFFIX];
    int output_data_size;
    char output_data[MAX_OUTPUT_BUF_SIZE];
}DecodeInfo;

Status do_decoding(int argc,char *argv[],DecodeInfo *decInfo);

Status read_validation_decode_args(int argc,char *argv[],DecodeInfo *deInfo);

Status check_magic_string_in_file(DecodeInfo *decInfo);

//Status open_files(DecodeInfo *decInfo);

void decode_string_from_image(FILE *fptr_image_file,char *magic_string,int size);

char decode_byte_from_image(char *arr);

Status decode_size_of_extn(DecodeInfo *decInfo);

int decode_int_from_image(FILE *fptr_image_file);

Status decode_output_file_extn(DecodeInfo *decInfo);

Status validate_output_file_extn(DecodeInfo *decInfo);

Status decode_output_data_size(DecodeInfo *decInfo);

Status decode_output_data_from_image(DecodeInfo *decInfo);


#endif

