#include "types.h"

typedef struct _DecodeInfo
{
    char *stegged_img_fname;
    FILE *fptr_stegged_img;

    char *secret_fname;
    FILE *fptr_secret_file;
    int secret_file_extn_size;
    char secret_file_extn[5];
    int secret_file_size;

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *DecInfo , int argc );

/* Perform the decoding */
Status do_decoding( DecodeInfo *Decodeinfo );

/* Get File pointer for Stegged Image file  */
Status open_bmp_file(DecodeInfo *Decodeinfo);

/* Skip the header of the stegged image file pointer */
Status skip_bmp_header( DecodeInfo *Decodeinfo);

/* Decode the Magic String */
Status decode_magic_string( FILE *stegged_fptr );

/* Decode byte from lsb */
char decode_byte_from_lsb( char * buffer );

/* Decode secret file extension size */
Status decode_secret_file_extn_size(  DecodeInfo *Decodeinfo );

/* Decode size form lsb */
int decode_size_from_lsb( char *buffer );

/* Decode secret file extension */
Status decode_secret_file_extn( DecodeInfo *Decodeinfo );

/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *Decodeinfo );

/* Decode the secret file data */
Status decode_secret_file_data( DecodeInfo *Decodeinfo );