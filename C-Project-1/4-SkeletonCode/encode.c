#include <stdio.h>
#include <string.h>
#include "common.h"
#include "encode.h"
#include "types.h"


// Function Definitions 

//   Get image size
//   Input: Image file ptr
//   Output: width * height * bytes per pixel (3 in our case)
//   Description: In BMP Image, width is stored in offset 18,
//   and height after that. size is 4 bytes
 
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

    // Return image capacity
    return width * height * 3;
}

//  
//   Get File pointers for i/p and o/p files
//   Inputs: Src Image file, Secret file and
//   Stego Image file
//   Output: FILE pointer for above files
//   Return Value: e_success or e_failure, on file errors
  
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
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


OperationType check_operation_type( char *argv[] , int argc ) 
{

    // Checking if argv[1] == "-e"  => return e_encode
    // Checking if argv[1] == "-d"  => return e_decode
    // else                         => return e_unsupported 

    if ( strcmp( argv[ 1 ] , "-e" ) == 0  )
    {
        if ( ( argc >= 4 ) && ( argc <= 5) )
            return e_encode ;
        else
            return e_unsupported ;
    }
    
    else if ( strcmp( argv[ 1 ] , "-d" ) == 0  )
    {
        if ( ( argc >= 3 ) && ( argc <= 4) )
            return e_decode ;
        else
            return e_unsupported ;
    }

    else
            return e_unsupported ;  
    
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo , int argc )
{
   char str1[50];
   strcpy(str1,argv[2]);
   char *cptr1=strstr(str1,".bmp");
                                         
   if (cptr1 && (*(cptr1 +4) == '\0' ))
   {
        encInfo->src_image_fname = argv[2];
           // printf("Source file name has been updated \n");
        char str2[50];
        strcpy(str2,argv[3]);                  //printf("the str string is %s\n" , str2 );
        char *cptr2;
        if (cptr2 = strstr(str2,".txt"))
        {
            if (cptr2 && (*(cptr2 + 4) == '\0' ))
            {
                // Secret file name
                encInfo->secret_fname = argv[3];
                // Secret file extension 
                strcpy( encInfo->extn_secret_file ,".txt" );        //printf("secretfile extension is %s\n" , encInfo->extn_secret_file ) ;
                if ( argc == 5 )
                {
                    char str3[50];
                    strcpy( str3 , argv[4] );
                    char *cptr3 = strstr( str3 , ".bmp");

                    if (cptr3 && (*(cptr3 + 4 ) == '\0'))
                    {
                        encInfo->stego_image_fname = argv[4];
                        return e_success;
                    }
                    else
                    {
                        return e_failure;
                    }
                }
                else
                {
                    encInfo->stego_image_fname = "output_encoded_image.bmp";
                    return e_success;
                }
                
            }
            else
            {
                return e_failure;
            }
        }   
        else if (cptr2 = strstr ( str2 , ".c"))
        {
            if (cptr2 && (*(cptr2 + 2) == '\0' ))
            {
                // Secret file name
                encInfo->secret_fname = argv[3];
                // Secret file extension 
                strcpy( encInfo->extn_secret_file , ".c" );             //printf("secretfile extension is %s\n" , encInfo->extn_secret_file ) ;

                if ( argc == 5 )
                {
                    char str3[50];
                        strcpy( str3 , argv[4]);
                        char *cptr3 = strstr( str3 , ".bmp");
                        if ( cptr3 && (*(cptr3 + 4 ) == '\0' ) )
                        {
                           encInfo->stego_image_fname = argv[4];
                           return e_success;
                        }
                        else
                        {
                           return e_failure;
                        }
                }
                else
                {
                    encInfo->stego_image_fname = "output_encoded_image.bmp";
                    printf("The default output file is output_encoded_image.bmp \n");
                    return e_success;
                }
            }
            else
            {
                return e_failure;
            }
        }
        else if ( cptr2 = strstr ( str2 , ".sh"))
        {
            if ( cptr2 && (*(cptr2 + 3) == '\0' ))
            {
                // Secret file name
                encInfo->secret_fname = argv[3];
                // Secret file extension 
                strcpy( encInfo->extn_secret_file , ".sh" );
                                                //printf("secretfile extension is %s\n" , encInfo->extn_secret_file ) ;
                if ( argc == 5 )
                {
                    char str3[50];
                        strcpy( str3 , argv[4]);
                        char *cptr3 = strstr( str3 , ".bmp");
                         if ( cptr3 && (*(cptr3 + 4 ) == '\0' ))
                         {
                            encInfo->stego_image_fname = argv[4];
                            return e_success;
                         }
                         else
                         {
                            return e_failure;
                         }
                }
                else
                {
                    encInfo->stego_image_fname = "output_encoded_image.bmp";

                    return e_success;
                }
            }
            else
            {
                return e_failure;
            }
        }
        else
        {
            return e_failure;
        }
   }
   else
   {
        return e_failure;
   }
}
uint get_file_size(FILE *fptr)
{
    fseek( fptr , 0 , SEEK_END );
    return ftell( fptr );
}
 
Status check_capacity(EncodeInfo *encInfo)
{
    // Getting the source image file size 
    uint size_src_img_file = get_image_size_for_bmp( encInfo->fptr_src_image);
    encInfo->size_image_file = size_src_img_file;
    
    // Getting the secret file size 
    uint size_secret_file = get_file_size( encInfo->fptr_secret);
    encInfo->size_secret_file = size_secret_file;

    uint Encode_things = 54 + (( 2 + 4 + strlen( encInfo->extn_secret_file) + 4 + size_secret_file ) * 8 );
    if ( size_src_img_file > Encode_things )
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind( fptr_src_image);
    char buffer[ 54 ];
    fread( buffer , 54 , 1 , fptr_src_image);
    fwrite( buffer , 54 , 1 , fptr_dest_image);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;
    for ( i = 0 ; i < 8 ; i++ )
    {
        int get = ( data & 1 << i ) >> i;
        image_buffer[i] = image_buffer[i] & ( ~ 1 );
        image_buffer[i] = image_buffer[i] | get; 
    }
}
Status encode_size_to_lsb( int data,char * buffer)
{
    int i;
    for ( i = 0 ; i < 32 ; i++ )
    {
        int get = ( data & 1 << i ) >> i;
        buffer[i] = buffer[i] & ( ~ 1 );
        buffer[i] = buffer[i] | get;
    }
}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[ 8 ];
    int i;

    for (i = 0 ; i < size ; i++ )
    {
        fread( buffer , 8 , 1 , fptr_src_image );
        encode_byte_to_lsb( data[i] , buffer );
        fwrite( buffer , 8 , 1 , fptr_stego_image);
    }
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image( MAGIC_STRING , strlen(magic_string) , encInfo->fptr_src_image , encInfo->fptr_stego_image );
    return e_success ;
    //printf("Magic string encoded successfully \n") ;
}

Status encode_secret_file_extn_size( long extn_size , EncodeInfo *encInfo)
{
    char buffer[32];
    fread( buffer , 32 , 1 , encInfo->fptr_src_image );
    encode_size_to_lsb( extn_size , buffer );
    fwrite( buffer , 32 , 1 , encInfo->fptr_stego_image );
    return e_success;
}

Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image( file_extn , strlen( file_extn ) , encInfo->fptr_src_image , encInfo->fptr_stego_image );
    return e_success ;
}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread( buffer , 32 , 1 , encInfo->fptr_src_image );
    encode_size_to_lsb( file_size , buffer );
    fwrite( buffer , 32 , 1 , encInfo->fptr_stego_image );
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src , FILE *fptr_dest  )
{ 
    char buffer[1];
    while ( !feof(fptr_src) )
    {
        fread( buffer , 1 , 1 , fptr_src );
        fwrite( buffer , 1 , 1 , fptr_dest );
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char file_data[ encInfo->size_secret_file ];
    rewind ( encInfo->fptr_secret );       //fseek( encInfo->fptr_secret , 0 , SEEK_SET) ;
    fread( file_data , encInfo->size_secret_file  , 1 , encInfo->fptr_secret );
    encode_data_to_image( file_data , encInfo->size_secret_file , encInfo->fptr_src_image , encInfo->fptr_stego_image );
    return e_success;
}



Status do_encoding(EncodeInfo *encodeinfo )
{
    if ( open_files( encodeinfo ) == e_success )
    {
        printf("Files are opened successfully \n");
        if ( check_capacity( encodeinfo ) == e_success ) 
        {
            printf("Required file size is available \n");
            if ( copy_bmp_header( encodeinfo->fptr_src_image , encodeinfo->fptr_stego_image ) == e_success ) 
            {
                printf("Image header is copied \n");
                if ( encode_magic_string( MAGIC_STRING , encodeinfo ) == e_success )
                {
                    printf("Magic string has been encoded \n");
                    if ( encode_secret_file_extn_size(  strlen(encodeinfo->extn_secret_file) , encodeinfo ) == e_success )
                    {
                        printf("Secret file extension size has been encoded \n");
                        if ( encode_secret_file_extn( encodeinfo->extn_secret_file , encodeinfo ) == e_success )
                        {
                            printf("Secret file extension has been encoded \n");
                            if ( encode_secret_file_size( encodeinfo->size_secret_file , encodeinfo ) == e_success )
                            {
                                printf("Secret file size has been encoded \n" );
                                if ( encode_secret_file_data( encodeinfo ) == e_success )
                                {
                                    printf("Secret file data has been encoded \n"); 
                                    if ( copy_remaining_img_data( encodeinfo->fptr_src_image , encodeinfo->fptr_stego_image ) == e_success )
                                    {
                                        printf("Remaining image data has been copied \n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Failed to encode Secret file data \n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("Failed to Secret file size \n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Failed to encode Secret file extension \n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to encode Secret File Extension Size \n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to encode Magic String \n");
                    return e_failure;
                }
            }
            else
            {
                printf("Error in copying Header !\n");
                return e_failure;
            }
        }
        else
        {
            printf("Size of the Image file is not enough \n");
            return e_failure;
        }  
    }
    else
    {
        printf("File operation Error\n");
        return e_failure;
    }
}
