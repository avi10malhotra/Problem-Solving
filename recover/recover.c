// recovers the 50 lost images by the cs50 team. You're Welcome.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BLOCK;

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }
    
    // open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr,"The Forensic Image cannot be opened for reading.\n");
        return 2;
    }
    
    // defining variables
    BLOCK buffer[512];
    char filename[50];
    int counter = 0;
    FILE *temp = NULL;
    
    // iterate until  the end of file is reached
    while (!feof(inptr))
    {
        // check for JPEG signature
        if ( buffer[0] == 0xff &&
             buffer[1] == 0xd8 &&
             buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // case file already exsits
            if (temp != NULL)
                fclose(temp);
                
            // labelling new found jpg files    
            sprintf(filename, "%03i.jpg", counter);
            
            // opening the file for writing
            temp = fopen(filename, "w");
            
            // increment the counter
            counter++;
            
            fwrite(buffer, sizeof(buffer), 1, temp);
        }
        
        // skip over the rest of the contents in the file
        else if (counter > 0)
        {
            // put jpg into temp
            fwrite(buffer, sizeof(buffer), 1, temp);            
        }
        
        // read the jpg files from inptr
        fread(buffer, sizeof(buffer), 1, inptr);
    }
    
    // close the file and exit
    fclose(inptr);
    fclose(temp);
    return 0;
}