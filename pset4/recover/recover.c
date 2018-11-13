/**
// File: resize.c
// Name: Jan Clawson
// Date: 5/13/2017
// Course: CS50 
// Desc: A program that recovers JPEGs from a forensic image
// Usage: Accepts one command-line argument, the name of a forensic image
          It really depends on the JPEGs being grouped together nicely

// File Dependencies: stdio.h , stdlib.h
// Subfunctions: 
Helpful stack exchange on pointers:
http://stackoverflow.com/questions/4331439/how-to-replace-a-char-in-char
https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm
http://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 
 int main(int argc, char *argv[])
 {
     // ensure proper usage
     if (argc != 2)
     {
         fprintf(stderr, "Usage: ./recover infile.\n");
         return 1;
     }
     
     // get the memory card file and set it to a string literal
     char *infile = argv[1];
     
     // open memory card file, check if it is null
     FILE *inptr = fopen(infile, "r");
     if (inptr == NULL)
     {
         fprintf(stderr, "Could not open %s.\n", infile);
         return 2;
     }
    
    // find how big the file is (in bytes) so we can loop over its memory. Then move pointer back to beginning.
    fseek(inptr, 0, SEEK_END); 
    int fileSize = ftell(inptr); 
    fseek(inptr, 0, SEEK_SET);
    
    // have a filenames counter and initial file name char array. initialize it for writing
    int filenames_int = 0;
    char filename[8] = "000.jpg";
    FILE *img = fopen(filename, "w");
    
    //repeat until end of card
    for(int i = 0; i < fileSize/512; i++) 
    { 
        // read 512 bytes into a buffer that takes from the heap
        unsigned char* buffer = malloc(512);
        fread(buffer, 512, 1, inptr);

        // start of a new JPEG. NOTE THE NEAT BITWISE OPERATOR WITH HEX!
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //First quesiton: have we already found a jpeg? 
            // if we havent start new jpeg
            if (filenames_int == 0)
            {
                //we have started a new JPEG! write the 512 bytes into the open file. Iterate file counter
                fwrite(buffer,512,1,img);
                filenames_int += 1;
            }
            
            // If we already have found a jpeg, close current file and start a new file
            else
            {
                // close previous file
                fclose(img);
                
                // create new file name, iterate counter
                sprintf(filename, "%03i.jpg",filenames_int);
                filenames_int += 1;
                
                //open new file, write the 512 bytes into it
                img = fopen(filename, "w");
                fwrite(buffer,512,1,img);
            }
        }
        
        // If this batch of 512 bytes does not have the jpeg function header
        else
        {
            // Have we already found a JPEG?
            // no -> this isn't jpeg information. Discard 512 byte by going to start of loop after clearing buffer
            if  (filenames_int == 0)
            {
                //NOTE: HAVE TO CLEAR BUFFER TO PREVENT LEAK!
                free(buffer);
                continue;
            }
            
            // yes -> write to current open file
            else
            {
                fwrite(buffer,512,1,img);
            }
        }
        // Free the buffer memory for next loop
        free(buffer);
    } 
 }