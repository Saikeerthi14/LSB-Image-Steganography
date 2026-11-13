#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<string.h>

char decode_byte_from_image(char *arr)
{
    char ch=0;
    for(int i=0;i<8;i++)
    {
        ch=ch|((arr[i]&1)<<i);
    }
    return ch;
}

void decode_string_from_image(FILE *fptr_image_file,char *string,int size)
{
    char arr[8];
    for(int i=0;i<size;i++)
    {
        fread(arr,8,1,fptr_image_file);
        string[i]=decode_byte_from_image(arr);
    }
}
Status check_magic_string_in_file(DecodeInfo *decInfo)
{
    decInfo->fptr_image_file=fopen(decInfo->image_file,"rb");
    if(decInfo->fptr_image_file==NULL)
    {
        printf("ERROR: Unable to open file %s\n", decInfo->image_file);
        return e_failure;
    }
    else 
       printf("%s file is opened successfully\n",decInfo->image_file);
    //skip the 54 bytes of header in image file
    fseek(decInfo->fptr_image_file,54,SEEK_SET);
    decode_string_from_image(decInfo->fptr_image_file,decInfo->magic_string,2);
    if(strcmp(decInfo->magic_string,"#*")==0)
    {
        return e_success;
    }
    else
    {
        printf("Magic string not found in %s\n",decInfo->image_file);
        return e_failure;
    }
    
}
Status read_validation_decode_args(int argc,char *argv[],DecodeInfo *deInfo)
{
    int i;
    for(i=2;i<argc;i++)
    {
        if(strstr(argv[i],".bmp")!=NULL)
        {
            deInfo->image_file=argv[i];
            if(check_magic_string_in_file(deInfo)==e_success)
            {
                printf("Magic string found in %s\n", deInfo->image_file);
                break;
            }
        }
    }
    if(argv[argc-1]!=NULL)
    {
        deInfo->output_data_file=argv[argc-1];
        if(strstr(deInfo->output_data_file,".bmp")==NULL)
        {
            char ch='.';
            char *ret=strchr(deInfo->output_data_file,ch);
            if(ret!=NULL && (ret+1)!=NULL)
            {
                printf("output file having extension\n");
            }
            else
            {
                printf("Output file must have any extension but not .bmp\n");
                return e_failure;
            }
        }
        else
        {
            strcpy(deInfo->output_data_file,"output.txt");
            printf("Output file given defaultly as %s\n",deInfo->output_data_file);
        }
    }
    else if(argv[argc-1]==NULL)
    {
        strcpy(deInfo->output_data_file,"output.txt");
        printf("Output file given defaultly as %s\n",deInfo->output_data_file);
    }
    else
    {
        printf("Output file name not given\n");
        return e_failure;
    }
    return e_success;
}
Status do_decoding(int argc,char *argv[],DecodeInfo *decInfo)
{
    if(decode_size_of_extn(decInfo)!=e_success)
    {
        printf("Error:during decoding the size of output file extension\n");
        return e_failure;
    }
    if(decode_output_file_extn(decInfo)!=e_success)
    {
        printf("Error:during decoding the output file extension\n");
        return e_failure;
    }
    if(validate_output_file_extn(decInfo)!=e_success)
    {
        printf("Output file extension is not valid\n");
        return e_failure;
    }
    if(decode_output_data_size(decInfo)!=e_success)
    {
        printf("Error:during decoding the size of output file\n");
        return e_failure;
    }
    if(decode_output_data_from_image(decInfo)!=e_success)
    {
        return e_failure;
    }
    return e_success;

}

int decode_int_from_image(FILE *fptr_image_file)
{
    char arr[32];
    fread(arr,32,1,fptr_image_file);
    int num=0;
    for(int i=0;i<32;i++)
    {
        num=num|((arr[i]&1)<<i);
    }
    return num;
}

Status decode_size_of_extn(DecodeInfo *decInfo)
{
    decInfo->size_extn_output_file=decode_int_from_image(decInfo->fptr_image_file);
    printf("Successfully decoded the size of output file extension\n");
    printf("%d\n",decInfo->size_extn_output_file);
    return e_success;
}

Status decode_output_file_extn(DecodeInfo *decInfo)
{
    decode_string_from_image(decInfo->fptr_image_file,decInfo->extn_output_file,decInfo->size_extn_output_file);
    printf("Successfully decoded the output file extension\n");
    printf("%s\n",decInfo->extn_output_file);
    return e_success;
}

Status validate_output_file_extn(DecodeInfo *decInfo)
{
    char ch='.';
    char *ret=strchr(decInfo->output_data_file,ch);
    if(ret!=NULL && (ret+1)!=NULL)
    {
        if(strcmp(ret,decInfo->extn_output_file)==0)
        {
            printf("Output file extension is valid\n");
            printf("%s\n",ret);
            return e_success;
        }
        else
        {
            decInfo->output_data_file=strtok(decInfo->output_data_file,".");
            //printf("%s\n",decInfo->output_data_file);
            sprintf(decInfo->output_data_file,"%s%s",decInfo->output_data_file,decInfo->extn_output_file);
            printf("Output file extension is changed as decoded output file extension\n");
            printf("%s\n",decInfo->output_data_file);
            return e_success;
        }
    }
    else
    {
        printf("Output file extension is not valid\n");
        return e_failure;
    }
}

Status decode_output_data_size(DecodeInfo *decInfo)
{
    decInfo->output_data_size=decode_int_from_image(decInfo->fptr_image_file);
    printf("Successfully decoded the size of output file\n");
    printf("%d\n",decInfo->output_data_size);
    return e_success;
}

Status decode_output_data_from_image(DecodeInfo *decInfo)
{
    decode_string_from_image(decInfo->fptr_image_file,decInfo->output_data,decInfo->output_data_size);
    decInfo->fptr_output_file=fopen(decInfo->output_data_file,"w");
    if(decInfo->fptr_output_file==NULL)
    {
        printf("ERROR: Unable to open file %s\n", decInfo->output_data_file);
        return e_failure;
    }
    else
        printf("%s file is opened successfully\n",decInfo->output_data_file);
    for(int i=0;i<decInfo->output_data_size;i++)
    {
        putc(decInfo->output_data[i],decInfo->fptr_output_file);
    }
    printf("Successfully decoded secret data into the %s file\n",decInfo->output_data_file);
    fclose(decInfo->fptr_output_file);
    fclose(decInfo->fptr_image_file);
    return e_success;
}