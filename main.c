#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include<stdlib.h>
#include<string.h>
#include "types.h"

OperationType check_operation_type(int argc,char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        if(argc<4)
        {
            printf("Error: must give two file names\n");
        }
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        if(argc<3)
        {
            printf("Error: must give file names\n");
            return e_unsupported;
        }
        return e_decode;
    }
    else
        return e_unsupported;
}

int main(int argc , char *argv[])
{
   if(argc==1)
   {
        printf("Usage:\n");
        printf("For encoding : ./a.out -e imagefile secretdatafile (optional)outputimagefile\n");
        printf("For decoding : ./a.out -d outputimagefile (optional)multiplefiles outputfile(optional)\n");
   }
   else{
   OperationType res = check_operation_type(argc,argv);

   if(res == e_encode)
   {
        EncodeInfo encinfo;
        if(read_and_validate_encode_args(argv,&encinfo) == e_success)
        {
            do_encoding(&encinfo);
        }
        else
        {
            printf("Error in some validations\n");
            exit(1);
        }
   }
   else if(res == e_decode)
   {
        DecodeInfo decinfo;
        if(read_validation_decode_args(argc,argv,&decinfo)!=e_success)
        {
            printf("Some validation failed\n"); 
            exit(1);
        }
        else
        {
            do_decoding(argc,argv,&decinfo);
        }
   }
   else
   {
        printf("Error: Unsupported operation type\n");
        exit(1);
   }
}
}
