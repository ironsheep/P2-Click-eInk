#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "argparse.h"

#include "New Piskel-coffee-002.c"

typedef int bool;
#define FALSE 0
#define TRUE (-1)

static const char *const usages[] = {
    "convert [options] -o {outfilename}",
    NULL,
};

int main(int argc, const char **argv)
{
    bool opt_nodo = FALSE;
    bool opt_debug = FALSE;
    bool opt_verbose = FALSE;
    const char *outfile = NULL;
    const char *imageName = NULL;
    const char *defaultImageName = "img";

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("convert options"),
        OPT_BOOLEAN('n', "nodo", &opt_nodo, "emit messages only don't do work", NULL, 0, 0),
        OPT_BOOLEAN('d', "debug", &opt_debug, "emit debug messages", NULL, 0, 0),
        OPT_BOOLEAN('v', "verbose", &opt_verbose, "emit verbose messages", NULL, 0, 0),
        OPT_STRING('o', "outfile", &outfile, "filename of output file", NULL, 0, 0),
        OPT_STRING('i', "imagename", &imageName, "label of image in memory (def: 'img')", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nA brief description of what the program does and how it works.", "\nAdditional description of the program after the description of the arguments.");
    argc = argparse_parse(&argparse, argc, argv);
    if (opt_nodo != FALSE)
        printf("Nodo: ON\n");
    if (opt_debug != FALSE)
        printf("Debug: ON\n");
    if (opt_verbose != FALSE)
        printf("Verbose: ON\n");

    if (outfile != NULL)
        printf("Writing to: %s\n", outfile);

    if (imageName == NULL) {
        imageName = defaultImageName;
    }
    if (opt_verbose) {
        printf("Image is named: %s\n", imageName);
    }

    if (argc != 0) {
        printf("argc: %d\n", argc);
        int i;
        for (i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, *(argv + i));
        }
    }

    if (outfile == NULL) {
        printf("\n\nconvert: ERROR missing output filename\n\n");
        argparse_usage(&argparse);
        return 255;
    }


    FILE *f;
    f = fopen(outfile, "w");

    // connection from included Piskel file
    int imgFrameHeight = NEW_PISKEL_FRAME_HEIGHT;
    int imgFrameWidth = NEW_PISKEL_FRAME_WIDTH;
    const u_int32_t *imgDataArray = &new_piskel_data[0][0];

    int nIdx = 0;
    long value = 0l;
    long color = 0l;
    int emptyLineCt = 0;

    bool bFirstPass = TRUE;
    bool bFoundNonEmpty = FALSE;
    for(int row = 0; row < imgFrameHeight; row++) {
        if(bFoundNonEmpty == FALSE) {
            // presscan row. If leading row and empty don't emit!
            for(int col = 0; col < imgFrameWidth; col += 2) {
                int nIdx = (row * imgFrameWidth) + col;
                int value = imgDataArray[nIdx];
                if(value != 0) {
                    bFoundNonEmpty = TRUE;
                    break;
                }
            }
            if(bFoundNonEmpty == FALSE) {
                emptyLineCt++;
                continue;   // skip to next row
            }
        }
        if(bFirstPass) {
            if(opt_verbose) {
                printf("\nImage is %d X %d px\n", imgFrameWidth, imgFrameHeight - emptyLineCt);
            }
            bFirstPass = FALSE;
        }
        for(int col = 0; col < imgFrameWidth; col++) {
            nIdx = (row * imgFrameWidth) + col;
            value = new_piskel_data[0][nIdx];
            color = value & 0xFFFFFF;
            if(col == 0) {
                //printf("' values 0x%X thru 0x%X\n", nIdx, nIdx + imgFrameHeight - 1);
            }
            if(value == 0) {
               if(opt_debug) printf("*   ");
            }
            else {
                // printf("$%.6lX, ", color);
                if(opt_debug) printf("$%.2lX ", color & 0xFF);
            }
            //if((col + 1) % 16 == 0) {
            //    printf("\n");
            //}
            //else if((col + 1) % 32 == 0) {
            //    printf("\n");
            //}
        }
        if(opt_debug) printf("\n");
    }
    if(opt_debug) printf("\n");

    int actualLineCt = imgFrameHeight - emptyLineCt;

    fprintf(f, "DAT { Overlay Bitmap Data }\n\n");
    fprintf(f, "'  Image by Convert v1.0\n\n");
    fprintf(f, "\t%sWidth\tWORD\t%d\n", imageName, imgFrameWidth / 2);
    fprintf(f, "\t%sHeight\tWORD\t%d\n", imageName, actualLineCt);
    fprintf(f, "\t%sBits\n", imageName);
    int rowNbr = 1;
    for(int row = emptyLineCt; row < imgFrameHeight; row++) {
        fprintf(f, "\t\tBYTE\t");
        for(int col = 0; col < imgFrameWidth; col += 2) {
            int nIdx = (row * imgFrameWidth) + col;
            int valueLt = new_piskel_data[0][nIdx];
            int valueRt = new_piskel_data[0][nIdx+1];
            valueLt = (valueLt == 0) ? 0x07 : valueLt & 0x03;
            valueRt = (valueRt == 0) ? 0x07 : valueRt & 0x03;
            int packedByte = (valueLt << 4) | valueRt;
            if(col == (imgFrameWidth - 2)) {
                fprintf(f, "$%.2X", packedByte);
            }
            else {
                fprintf(f, "$%.2X, ", packedByte);
            }
        }
        fprintf(f, "\t' row %d of %d\n", rowNbr++, actualLineCt);
    }
    int nonEmptyRows = imgFrameHeight - emptyLineCt;
    fprintf(f, "' Converted %d x %d (%d) pixel values into %d bytes\n", imgFrameWidth, nonEmptyRows, imgFrameWidth * nonEmptyRows, (imgFrameWidth * nonEmptyRows) / 2);
    fprintf(f, "\t%sBitsEnd\n", imageName);
    fprintf(f, "\t%sLenBytes\tWORD\t%sBitsEnd - %sBits\t' Image length in bytes\n", imageName, imageName, imageName);

    fprintf(f, "\n");
   //fprintf(f, "%s\n[ %d ]\n\n", *name, number);
    fclose(f);
    return 0;
}
