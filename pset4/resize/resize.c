/**
// File: resize.c
// Name: Jan Clawson
// Date: 5/13/2017
// Course: CS50 
// Desc: A program that resizes 24-bit uncompressed BMP 4.0 by a factor of n
// Usage: Usage: ./resize n infile outfile
          where 1<n<100 and n is an int
// File Dependencies: stdio.h , stdlib.h , bmp.h
// Subfunctions: largely copied from provided function "copy.c"
 */
       
#include <stdio.h>
#include <stdlib.h>

// include the data structure for organizing Bitmap header information and RGB triples
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage: correct number of arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // remember filenames and turn the resize integer into an int data type
    char *resize = argv[1];
    int n = atoi(resize);
    char *infile = argv[2];
    char *outfile = argv[3];

    // check if n less than or equal to 100
    if (*argv[1] > 100)
    {
        fprintf(stderr, "the first (n) must be a positive integer less than or equal to 100,n");
        return 1;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    // fread(pointer to a struct, size of element to read, number of element to read, FILE* to read from)
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine padding for scanlines on inread file
    int oldpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //change file headers
    int biWidthOriginal = bi.biWidth;
    int biHeightOriginal = bi.biHeight;
    bi.biWidth = n*bi.biWidth;
    bi.biHeight = n*bi.biHeight;

    // DETERMINE PADDING FOR SCANLINES ON OUTREAD FILE
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi.biSizeImage = ((sizeof(RGBTRIPLE)*bi.biWidth) + padding)*abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Begin enlargment process
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(biHeightOriginal); i < biHeight; i++)
    {
        // create a while loop for vertical scaling (pixel resize is performed n times)
        int counter = 0;
        // note where the beginning of the scanline is. We will revert to reading from this position in the file 
        // several times
        int beginLine = ftell(inptr);
        
        while (counter < n)
        {
            
            // for loop iterating over pixels in scanline, also doing the resizing process in scanline
            for (int j = 0; j < biWidthOriginal; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile. We do this n times for enlargement
                for (int k = 0; k<n; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // skip over padding, if any
            fseek(inptr, oldpadding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
            
            counter= counter+1;
            
            // if we still have vertical englargement needed, go back to the start of the scanline in the infile
            if (counter < n)
                fseek(inptr,beginLine,SEEK_SET);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
