#include <stdio.h>
#include <stdint.h>

#include "New Piskel-coffee-002.c"

#define FALSE 0
#define TRUE (-1)

int main() {
    FILE *f;
    f = fopen("image_coffee.spin2", "w");
    typedef int bool;
//    #define NEW_PISKEL_FRAME_WIDTH 32
//    #define NEW_PISKEL_FRAME_HEIGHT 64

// Piskel data for "New Piskel"

// static const uint32_t new_piskel_data[1][2048] = {
    int nIdx = 0;
    long value = 0l;
    long color = 0l;
    int emptyLineCt = 0;

    bool bFirstPass = TRUE;
    bool bFoundNonEmpty = FALSE;
    for(int row = 0; row < NEW_PISKEL_FRAME_HEIGHT; row++) {
        if(bFoundNonEmpty == FALSE) {
            // presscan row. If leading row and empty don't emit!
            for(int col = 0; col < NEW_PISKEL_FRAME_WIDTH; col += 2) {
                int nIdx = (row * NEW_PISKEL_FRAME_WIDTH) + col;
                int value = new_piskel_data[0][nIdx];
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
            printf("\nImage is %d X %d px\n", NEW_PISKEL_FRAME_WIDTH, NEW_PISKEL_FRAME_HEIGHT - emptyLineCt);
            bFirstPass = FALSE;
        }
        for(int col = 0; col < NEW_PISKEL_FRAME_WIDTH; col++) {
            nIdx = (row * NEW_PISKEL_FRAME_WIDTH) + col;
            value = new_piskel_data[0][nIdx];
            color = value & 0xFFFFFF;
            if(col == 0) {
                //printf("' values 0x%X thru 0x%X\n", nIdx, nIdx + NEW_PISKEL_FRAME_HEIGHT - 1);
            }
            if(value == 0) {
                printf("*   ");
            }
            else {
                // printf("$%.6lX, ", color);
                printf("$%.2lX ", color & 0xFF);
            }
            //if((col + 1) % 16 == 0) {
            //    printf("\n");
            //}
            //else if((col + 1) % 32 == 0) {
            //    printf("\n");
            //}
        }
        printf("\n");
    }
    printf("\n");

    int actualLineCt = NEW_PISKEL_FRAME_HEIGHT - emptyLineCt;

    fprintf(f, "DAT { Overlay Bitmap Data }\n\n");
    fprintf(f, "'  Image by Convert v1.0\n\n");
    fprintf(f, "\timgWidth\tWORD\t%d\n", NEW_PISKEL_FRAME_WIDTH / 2);
    fprintf(f, "\timgHeight\tWORD\t%d\n", actualLineCt);
    fprintf(f, "\timgBits\n");
    int rowNbr = 1;
    for(int row = emptyLineCt; row < NEW_PISKEL_FRAME_HEIGHT; row++) {
        fprintf(f, "\t\tBYTE\t");
        for(int col = 0; col < NEW_PISKEL_FRAME_WIDTH; col += 2) {
            int nIdx = (row * NEW_PISKEL_FRAME_WIDTH) + col;
            int valueLt = new_piskel_data[0][nIdx];
            int valueRt = new_piskel_data[0][nIdx+1];
            valueLt = (valueLt == 0) ? 0x07 : valueLt & 0x03;
            valueRt = (valueRt == 0) ? 0x07 : valueRt & 0x03;
            int packedByte = (valueLt << 4) | valueRt;
            if(col == (NEW_PISKEL_FRAME_WIDTH - 2)) {
                fprintf(f, "$%.2X", packedByte);
            }
            else {
                fprintf(f, "$%.2X, ", packedByte);
            }
        }
        fprintf(f, "\t' row %d of %d\n", rowNbr++, actualLineCt);
    }
    int nonEmptyRows = NEW_PISKEL_FRAME_HEIGHT - emptyLineCt;
    fprintf(f, "' Converted %d x %d (%d) pixel values into %d bytes\n", NEW_PISKEL_FRAME_WIDTH, nonEmptyRows, NEW_PISKEL_FRAME_WIDTH * nonEmptyRows, (NEW_PISKEL_FRAME_WIDTH * nonEmptyRows) / 2);
    fprintf(f, "\timgBitsEnd\n");
    fprintf(f, "\timgLenBytes\tWORD\timgBitsEnd - imgBits\t' Image length in bytes\n");

    fprintf(f, "\n");
   //fprintf(f, "%s\n[ %d ]\n\n", *name, number);
    fclose(f);
    return 0;
}
