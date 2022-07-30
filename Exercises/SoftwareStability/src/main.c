#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EndianSwap(Data) ((Data & 0x000000FF) << 24) | ((Data & 0x0000FF00) << 8) | ((Data & 0x00FF0000) >> 8) | ((Data & 0xFF000000) >> 24)

#define FAIL 0
#define SUCCESS 1

void swapBuffer(unsigned char *array, int offset)
{
    unsigned *pU32;
    int i;

    for (i = 0; i < offset; i += 4)
    {
        pU32 = (unsigned *)&array[i];
        *pU32 = EndianSwap(*pU32);
    }

    return;
}

int main(int argc, char *argv[])
{
    unsigned int result = SUCCESS;

    unsigned int input_file_size = 0;
    unsigned int output_size = 0;
    unsigned int outfile_no = 1;

    unsigned char *swap_array;
    unsigned char inf_name[255];
    unsigned char outf_name[14];
    unsigned char outsize[255];
    unsigned char *fptr;
    unsigned char tmp[8];

    FILE *inf_handle;
    FILE *outf_handle;

    do
    {
        /* Request File name from user */
        printf("\nPlease enter file name (Q to quit):\n");
        scanf("%s", &inf_name[0]);

        if ((strncmp((char *)&inf_name[0], "q", 1) == 0) || (strncmp((char *)&inf_name[0], "Q", 1) == 0))
        {
            printf("Exiting\n\n");
            exit(0);
        }

        /* Check if file can be opened, otherwise warn the user */
        inf_handle = fopen((char *)&inf_name[0], "r");
        if (inf_handle == NULL)
        {
            printf("ERROR: File could not be found\n");
            result = FAIL;
        }

        if (result == SUCCESS)
        {
            /* Find input file size */
            fseek(inf_handle, 0, SEEK_END);
            input_file_size = ftell(inf_handle);

            printf("Size of input file: %dB\n", input_file_size);

            /* Request data size */
            printf("\nPlease provide amount of data you want to swap:\n");
            scanf("%s", &outsize[0]);

            output_size = atoi((char *)&outsize[0]);
            printf("Size of output file: %dB\n", output_size);

            /* check if output_size is divisible by 4 or remaining bytes become meaningless */
            if (output_size % 4)
            {
                printf("ERROR: This is a 4B swap operation\nsize should be multiple of 4\n");
                result = FAIL;
            }

            if (output_size > input_file_size)
            {
                printf("ERROR: Given size (%u) is larger than input file size (%u)\n", output_size, input_file_size);
                result = FAIL;
            }

            if (result == SUCCESS)
            {
                /* Allocate memory for file */
                fptr = malloc(input_file_size);

                if (fptr == NULL)
                {
                    printf("ERROR: Memory could not be allocated\n");
                    result = FAIL;
                }
            }

            /* Allocate memory for swap array */
            swap_array = malloc(output_size);

            if (swap_array == NULL)
            {
                free(fptr);
                printf("ERROR: Memory could not be allocated\n");
                result = FAIL;
            }

            if (result == SUCCESS)
            {
                /* Read input file into memory */
                fseek(inf_handle, 0, SEEK_SET);
                fread(fptr, sizeof(char), input_file_size, inf_handle);

                memcpy(swap_array, fptr, output_size);

                swapBuffer(swap_array, output_size);

                /* Create a new output file with swapped data */
                strncpy((char *)outf_name, "output", 14);
                sprintf((char *)tmp, "%d"
                                     ".bin",
                        outfile_no);
                strncat((char *)outf_name, (char *)tmp, 14);

                outfile_no++;

                outf_handle = fopen((char *)outf_name, "w");
                if (outf_handle != NULL)
                {
                    fwrite(swap_array, sizeof(char), output_size, outf_handle);
                    fclose(outf_handle);
                }
                else
                {
                    printf("ERROR: Couldn't open output file %s\n", (char *)outf_name);
                }

                free(fptr);
                free(swap_array);
            }
        }

        fclose(inf_handle);

        if (result != SUCCESS)
        {
            // Reset variable for next round
            result = SUCCESS;
        }

    } while (1);

    return 0;
}