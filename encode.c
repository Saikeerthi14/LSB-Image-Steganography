#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    //shift the offset position at first
    fseek(fptr_image,0,SEEK_SET);

    // Return image capacity
    return width * height * 3;

}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
        encInfo->src_image_fname = argv[2];
    else
    {
        printf("Source file must having .bmp type\n"); 
        return e_failure;
    }
    char ch='.';
    if(strchr(argv[3],ch)!=NULL)
    {
        encInfo->secret_fname=argv[3];
        char *ret=strrchr(argv[3],'.');
        if(ret!=NULL && (ret+1)!=NULL)
            strcpy(encInfo->extn_secret_file,ret);
    }
    else
    {
        printf("Secret file must have any extension\n"); 
        return e_failure;
    }

    if (argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp")!=NULL)
        {
            printf("Output image file having extension\n");
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("Output file must contain .bmp extension\n"); 
            return e_failure;
        }
    }
    else if(argv[4]==NULL)
    {
        encInfo->stego_image_fname = "stego.bmp";
        printf("Output image file name given defaultly\n");
    }
    else
    {
        printf("output image file name not given\n");
        return e_failure;
    }


    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret, 0, SEEK_END);

    encInfo->size_secret_file = ftell(encInfo->fptr_secret);

    rewind(encInfo->fptr_secret);

    int size_of_info = strlen(MAGIC_STRING) + sizeof(int) + strlen(encInfo->extn_secret_file) + sizeof(int) + encInfo->size_secret_file;

    int image_size = get_image_size_for_bmp(encInfo->fptr_src_image);

    if(image_size > (size_of_info * 8))
    {
        printf("Image have enough capacity to store the secret data\n");
        return e_success;
    }
    return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr[54];

    fread(arr, 54, 1, fptr_src_image);
    fwrite(arr, 54, 1, fptr_dest_image);
    printf("Successfully copied header information into stego image file\n");
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_string_to_image(magic_string, strlen(magic_string), encInfo);
    printf("Successfully encoded the magic string\n");
    return e_success;
}

Status encode_size_extn(EncodeInfo *encInfo)
{
    encode_int_to_lsb(strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("Successfully encoded the size of extension\n");
    printf("%ld\n",strlen(encInfo->extn_secret_file));
    return e_success;
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_string_to_image(file_extn,strlen(file_extn),encInfo);
    printf("Successfully encoded the extension of secret file\n");
    printf("%s\n",file_extn);
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_int_to_lsb(file_size, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("Successfully encoded the size of secret data\n");
    printf("%ld\n",file_size);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char arr[encInfo->size_secret_file];
    fread(arr, encInfo->size_secret_file ,1,encInfo->fptr_secret);
    encode_string_to_image(arr, encInfo->size_secret_file, encInfo);
    printf("Successfully encoded the secret data into %s file\n",encInfo->stego_image_fname);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buf[1000];
    int n;
    while((n=fread(buf,1,1000,fptr_src))>0)
    {
        fwrite(buf,1,n,fptr_dest);
    }
    printf("Successfully copied remaining data into stego image file\n");
    return e_success;

}

Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        printf("Unable to open the files\n");
        return e_failure;
    }
    else
    {
        printf("Files are opened successfully\n");
    }

    if (check_capacity(encInfo) == e_failure)
    {
        printf("Error: Image does not have enough capacity\n");
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)!=e_success)
    {
        printf("Error : while coping the header information\n");
        return e_failure;
    }

    if(encode_magic_string(MAGIC_STRING, encInfo)!=e_success)
    {
        printf("Error : during encoding magic string\n");
        return e_failure;
    }

    if(encode_size_extn(encInfo)!=e_success)
    {
        printf("Error : during encoding the size of secret file extension\n");
        return e_failure;
    }

    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)!=e_success)
    {
        printf("Error : during encoding secret file extension\n");
        return e_failure;
    }

    if(encode_secret_file_size(encInfo->size_secret_file, encInfo)!=e_success)
    {
        printf("Error : during encoding the size of secret data\n");
        return e_failure;
    }

    if(encode_secret_file_data(encInfo)!=e_success)
    {
        printf("Error : during encoding the secret data\n");
        return e_failure;
    }

    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)!=e_success)
    {
        printf("Error : during copying the remaining data\n");
        return e_failure;
    }
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    return e_success;
}

void encode_int_to_lsb(int data, FILE *fptr_src, FILE *fptr_stego)
{
    char arr[32];

    fread(arr,32,1,fptr_src);

    for(int i = 0; i < 32; i++)
    {
        char get = (data & 1<<i) >> i;

        arr[i] = arr[i] & (~1);

        arr[i] = arr[i] | get;
    }
    fwrite(arr,32,1,fptr_stego);
}

void encode_string_to_image(char *str, int size, EncodeInfo *encInfo)
{
    for(int i = 0; i < size; i++)
    {
        char arr[8];
        fread(arr, 8,1, encInfo->fptr_src_image);
        encode_byte_to_lsb(str[i], arr);
        fwrite(arr, 8, 1, encInfo->fptr_stego_image);
    }
}

void encode_byte_to_lsb(char data, char *arr)
{
    for(int i = 0; i < 8; i++)
    {
        char get = (data & 1<<i) >> i;

        arr[i] = arr[i] & (~1);

        arr[i] = arr[i] | get;
    }
}