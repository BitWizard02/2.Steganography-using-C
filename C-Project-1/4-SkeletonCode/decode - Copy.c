#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"
#include "types.h"



Status read_and_validate_decode_args(char *argv[], DecodeInfo *DecInfo , int argc )
{
    char str1[100];
    strcpy( str1 , argv[2]);

    char *cptr1 = strstr( str1 , ".bmp" );
    if ( cptr1 && ( *(cptr1 + 4 ) == '\0'))
    {
        DecInfo->stegged_img_fname = argv[2];
        if ( argc == 4 )
        {
            char str2[150];
            strcpy( str2 , argv[3] );
            char *cptr2 = strstr( str2 , "." );

            if ( cptr2 == NULL )
            {
                DecInfo->secret_fname = argv[3];
                return e_success;
            }
            else
            {
                printf("Failed to validate the arguments\n");
                return e_failure;
            } 

        }
        else
        {
            DecInfo->secret_fname = malloc(strlen("output_secret") + 5);

            if ( DecInfo->secret_fname != NULL )
            {
                strcpy (DecInfo->secret_fname , "output_secret" );
            }
            else
            {
                printf("Memory allocation failed \n" );
                return e_failure;
            }
            printf("No output file is mentioned \n");
            printf("The default output file name is %s\n" , DecInfo->secret_fname );
            return e_success;
        }

    }
}

Status open_bmp_file(DecodeInfo *Decodeinfo)
{
    Decodeinfo->fptr_stegged_img = fopen( Decodeinfo->stegged_img_fname , "r" );

    if ( Decodeinfo->fptr_stegged_img == NULL )
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", Decodeinfo->stegged_img_fname );

    	return e_failure;
    }
    return e_success;
}

Status skip_bmp_header( DecodeInfo *Decodeinfo)
{
    if ( fseek( Decodeinfo->fptr_stegged_img , 54 , SEEK_SET ) == 0 ) 
        return e_success;
    else
        return e_failure;
}

Status decode_magic_string( FILE *stegged_fptr ) 
{
    // To store the decoded magic string from stegged image 
    char magicstr[3];
    // To get the data from the stegged image to decode
    char buffer[8];
    int i;
    for ( i = 0 ; i < 2 ; i++ )
    {
        //printf("%d\n", ftell(stegged_fptr));
        fread( buffer , 8 , 1 , stegged_fptr );
        magicstr[i] = decode_byte_from_lsb( buffer);
    }
    magicstr[2] = '\0';
    //printf("%s",magicstr);
    char userinput[5];
    for ( i = 0 ; i < 4 ; i++ )
    {
        printf("Enter the Magic String : ");
        scanf(" %s" , userinput );
        if ( strcmp( magicstr , userinput ) == 0 )
        {
            printf("Entered magic string is Valid\n");
            return e_success;
        }
        else
        {
            printf("MagicString is Incorrect\n");
            printf("%d more attempts remaining \n" , 3 - i );
        }
    }
    return e_failure;

}

char decode_byte_from_lsb( char * buffer ) 
{
    char ch = 0;
    int i;
    int get;
    for ( i = 0 ; i < 8 ; i++ )
    {
        get = buffer[i] & (1 << 0);
        ch = ch | ( get << i );
    }
    return ch;
    
}

Status decode_secret_file_extn_size( DecodeInfo *Decodeinfo )
{
    char buffer[32];
    fread( buffer , 32 , 1 , Decodeinfo->fptr_stegged_img );
    Decodeinfo->secret_file_extn_size = decode_size_from_lsb ( buffer );
    return e_success;
}

int decode_size_from_lsb( char *buffer )
{
    int data = 0;
    int i;
    char get = 0; 
    for ( i = 0 ; i < 32 ; i++ )
    {
        get = buffer[i] & (1 << 0);
        data = data | ( get << i );
    }
    return data;
}

Status decode_secret_file_extn( DecodeInfo *Decodeinfo )
{
    char extension[ Decodeinfo->secret_file_extn_size + 1 ];
    int i;
    char buffer[8];
    for ( i = 0 ; i < Decodeinfo->secret_file_extn_size ; i++ )
    {
        fread( buffer , 8 , 1 , Decodeinfo->fptr_stegged_img );
        extension[i] = decode_byte_from_lsb( buffer );
    }
    extension[Decodeinfo->secret_file_extn_size] = '\0';
    strcat( Decodeinfo->secret_fname , extension );

    // Opening the new secret file to store secret data 
    Decodeinfo->fptr_secret_file = fopen(Decodeinfo->secret_fname, "w");
    // Do Error handling
    if (Decodeinfo->fptr_secret_file == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", Decodeinfo->secret_fname);

    	return e_failure;
    }

    return e_success;
}

Status decode_secret_file_size( DecodeInfo *Decodeinfo )
{
    char buffer[32] ;
    fread( buffer , 32 , 1 , Decodeinfo->fptr_stegged_img ) ;
    Decodeinfo->secret_file_size = decode_size_from_lsb( buffer ) ;
    return e_success ;
}

Status decode_secret_file_data( DecodeInfo *Decodeinfo ) 
{
    char buffer[8];
    char ch = 0;
    int i ;
    for ( i = 0 ; i < Decodeinfo->secret_file_size ; i++ )
    {
        fread( buffer , 8 , 1 , Decodeinfo->fptr_stegged_img );
        ch = decode_byte_from_lsb( buffer );
        fwrite( &ch , 1 , 1 , Decodeinfo->fptr_secret_file );
    }
    return e_success; 
}

Status do_decoding( DecodeInfo *Decodeinfo ) 
{
    if ( open_bmp_file( Decodeinfo ) == e_success )
    {
        printf("Files opened successfully \n");
        if ( skip_bmp_header( Decodeinfo) == e_success )
        {
            printf("Skipped the 54 Bytes\n");
            if ( decode_magic_string(  Decodeinfo->fptr_stegged_img ) == e_success )
            {
                printf("Decoded the magicstring \n");
                if ( decode_secret_file_extn_size( Decodeinfo ) == e_success  )
                {
                    printf("Secret file extn size decoded successfully \n");
                    if ( decode_secret_file_extn( Decodeinfo ) == e_success )
                    {
                        printf("Secret file extension decoded successfully \n");
                        if ( decode_secret_file_size( Decodeinfo ) == e_success ) 
                        {
                            printf("Secret file size has been decoded successfully \n");
                            if ( decode_secret_file_data( Decodeinfo ) == e_success ) 
                            {
                                return e_success;
                            }
                            else
                            {
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Failed to Decode the secret file size \n");
                        }
                    }
                    else
                    {
                        printf("Failed to decode the extension \n");
                    }
                }
                else
                {
                    printf("Failed to decode the extension size \n");
                }
            }
            else
            {
                printf("Failed to decode the magic string \n");
            } 
        }
        else
        {
            printf("Failed to SEEK_SET the pointer\n");
        }
    }
    else
    {
        printf("Failed to open the files\n");
        return e_failure;
    }
}