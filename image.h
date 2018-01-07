#ifndef __IMAGE__
#define __IMAGE__

#include <stdio.h>

#define uint unsigned int

/**
 * Class containing image data
 */
typedef struct {
    int width;
    int height;
    int max_grey_value;
    int **data;
} image;

image *read_image(char *filename);

int save_image(image *image, char *filename);

void free_image(image **image);

int is_black_white(image *image);

int in_bounds(int x, int y, int w, int h);

#endif // __IMAGE__
