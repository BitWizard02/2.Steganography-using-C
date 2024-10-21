#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"
#include "decode.h"

int main(int argc , char *argv[])
{
    // Check the operation types
    // call check_operationtype fucnction
    if (argc >= 4 && argc <= 5)
    {    
        if(check_operation_type(argv , argc) == e_encode)
        {
            //  Declaring the EncodeInfo structure 
            EncodeInfo encodeinfo;
            if (read_and_validate_encode_args(argv , &encodeinfo , argc) == e_success) 
            {
                printf("Validate the arguments \n");
                if ( do_encoding( &encodeinfo ) == e_success )
                {   
                    printf("\nYour Secret data has been ENCODED Successfully!\n");
                    return e_success;
                }
                else
                {
                    printf("Failed to Encode the data !\n");
                }
            }
            else
            {
                printf("Invalid Arguments \n");
                printf("ERROR - Encoding : ./a.out -e <file.bmp> <secret_file> <output.bmp>(optional) \n" );
                printf("ERROR - Decoding : ./a.out -d <file.bmp> <output_file>(optional) \n");
                return 1;
            }
        }
    }
    if ((argc >= 3) && (argc <= 4))
    {
        if ( check_operation_type(argv , argc) == e_decode) 
        {   
                DecodeInfo decodeinfo;
                if (read_and_validate_decode_args( argv , &decodeinfo , argc ) == e_success )
                {
                    printf("Validated the arguments\n");
                    if ( do_decoding( &decodeinfo ) == e_success )
                    {
                        printf("\nYour Secret data has been DECODED Successfully!\n");
                    }
                    else
                    {
                        printf("Failed to Decode the data \n");
                    }
                }
                else
                {
                    printf("Invalid Arguments \n");
                    printf("ERROR - Encoding : ./a.out -e <file.bmp> <secret_file> <output.bmp>(optional) \n" );
                    printf("ERROR - Decoding : ./a.out -d <file.bmp> <output_file>(optional) \n");
                    return 1;
                }
        }
        else
        {
            printf("Invalid Arguments\n");
            printf("ERROR - Encoding : ./a.out -e <file.bmp> <secret_file> <output.bmp>(optional) \n" );
            printf("ERROR - Decoding : ./a.out -d <file.bmp> <output_file>(optional) \n");
            return 1;
        }
    }
    else
    {
        printf("Invalid Arguments \n");
        printf("ERROR - Encoding : ./a.out -e <file.bmp> <secret_file> <output.bmp>(optional) \n" );
        printf("ERROR - Decoding : ./a.out -d <file.bmp> <output_file>(optional) \n");
        return 1;
    }
    return 0;
}
