#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

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
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

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

OperationType check_operation_type(char *argv[])
{
    //checking argv[i] is "-e"==>return e code
    //else if ==>checking argc[i] is -d return decode
    //else ==>return e_unsupported
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        encInfo->src_image_fname=argv[2];
        //return e_success;
    }
    else
    {
        return e_failure;
    }
    
    if((strstr(argv[3],".txt")!=NULL)||(strstr(argv[3],".c")!=NULL)||(strstr(argv[3],".sh")!=NULL))
    {
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,strchr(argv[3],'.'));

        if(argv[4]!=NULL)
        {
            if(strcmp(strchr(argv[4],'.'),".bmp")==0)
            {
                encInfo->stego_image_fname=argv[4];
                //return e_success;
            }
            else
            {
                printf("need .bmp file\n");
                return e_failure;
            }
        }
        else
        {
            encInfo->stego_image_fname="output_fname.bmp";
            return e_success;
        }
    }
    else
    {
        printf("TRY AGAIN : With correct extn\n");
        return e_failure;
    }
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        if(check_capacity(encInfo)==e_success)
        {
            copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
            encode_magic_string(MAGIC_STRING, encInfo);
            encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo);
            encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
            encode_secret_file_size(encInfo->secret_file_size,encInfo);
            encode_secret_file_data(encInfo);
            copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
        }
        else
        {
            printf("Image capacity exceeded\n");
        }
    }
    else
    {
        printf("Error in opening files\n");
    }
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    int Encoding_things=54+(2+4+strlen(encInfo->extn_secret_file)+4+encInfo->size_secret_file)*8;

    
    if(encInfo->image_capacity->Encoding_things)
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
    char str[54];
    fseek(fptr_src_image,0,SEEK_SET);
    fread(str, 54, 1, fptr_src_image);
    fwrite(str, 54, 1, fptr_dest_image);

    return e_success;
}


Status encode_byte_to_lsb(char data, char *buffer)
{
    int get;
    for(int i = 0; i < 8; i++)
    {
        get=(data&(1<<i))>>i;
        buffer[i]=buffer[i]&(~1);
        buffer[i]=buffer[i]|get;
    }
    
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image,FILE *fptr_stego_image) 
{
    //read 8 bytes from the src file 
    //call encode_byte_to_lsb(data[0],buffer)
    //write 8 bytes to stego_img_size
    char buffer[8];
    for(int i = 0; i < strlen(data); i++) 
    {
        fread(buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], buffer);
        fwrite(buffer, 8, 1,fptr_stego_image);
    }
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);

}

Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char buffer[32]
    //read 32 bytes from the src_image
    //encode size to lsb
    //write 32 bytes to stego_image
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
}
Status encode_size_to_lsb(int data, char *buffer);
{
    for(int i-0;i<32;i++)
    {
        //logic remains same what we have used in encode_byte_to_lsb
        buffer[i]=buffer[i]&(~1);
        buffer[i]=buffer[i]|get;
    }
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);
{
    encode_data_to_image(file_extn,strlen(file_extn), encInfo->fptr_src_image,encInfo->fptr_stego_image)
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    //read 32 bytes from the src_image
    //call encode_size_to_lsb(file_size,buffer)
    //write 32 bytes to stego_image
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);

}
Status encode_size_to_lsb(int data,char *buffer)
{
    for(int i=0;i<32;i++)
    {
        int get;
        //logic remains same what we have used in encode_byte_to_lsb
        get=(data&(1<<i))>>i;
        buffer[i]=buffer[i]&(~1);
        buffer[i]=buffer[i]|get;
    }
}
Status encode_secret_file_data( EncodeInfo *encInfo)
{
    int fptr_secret;
    char file_data[encInfo->size_secret_file];
    fread(file_data,25,1,fptr_secret);
    encode_data_to_image(file_data,encInfo->size_secret_file,encInfo->size_secret_file,encInfo->fptr_stego_image);
}
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //read 1 byte from src image
    //write 1 byte from stego imge
    //till reching eof
     char buffer[1];
    for(int i=0;i!=EOF;i++)
    {
      fread(buffer,1,1,fptr_src_image);
      fwrite(buffer,1,1,fptr_stego_image);
    }
}