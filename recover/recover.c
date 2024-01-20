#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[512];
    int i = -1;

    while (fread(buffer, 1, 512, file) == 512)
    {

        // check all jpgs in the file
        if (
            buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0
            )
        {
            char *new = malloc(8);
            i = i + 1;
            sprintf(new, "%03i.jpg", i);
            FILE *img = fopen(new, "w");
            fwrite(buffer, 1, 512, img);
            free(new);
            fclose(img);
        }
        // Check if 000.jpg exist?
        else if (i > -1)
        {
            char *old = malloc(8);
            sprintf(old, "%03i.jpg", i);
            FILE *img2 = fopen(old, "a");
            fwrite(buffer, 1, 512, img2);
            free(old);
            fclose(img2);
        }
    }
    // Close files
    fclose(file);
}