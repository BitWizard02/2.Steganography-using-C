#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    uint img_size;

    // // Fill with sample filenames
    // encInfo.src_image_fname = "beautiful.bmp";
    // encInfo.secret_fname = "secret.txt";
    // encInfo.stego_image_fname = "stego_img.bmp";

    // // Test open_files
    // if (open_files(&encInfo) == e_failure)
    // {
    // 	printf("ERROR: %s function failed\n", "open_files" );
    // 	return 1;
    // }
    // else
    // {
    // 	printf("SUCCESS: %s function completed\n", "open_files" );
    // }

    // // Test get_image_size_for_bmp
    // img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    // printf("INFO: Image size = %u\n", img_size);

    //check the opertaion type
        //call the check opertaion type function 
        int res=check_operation_type(*argv);
        if(res==0)
        {
            if(argc>=4 && argc<=5)
            {
                //start the encoding
                read_and_validate_encode_args(argv, &encInfo);
            }
            else
            {
                //printf error
                printf("ERROR: Please enter the correct number of arguments\n");
            }
        }
        


    return 0;
}
