#ifndef __IMAGE__
#define __IMAGE__

#include <stdio.h>

#define uchar unsigned char

/**
 * Class containing image data
 */
typedef struct {
    int width;
    int height;
    int max_grey_value;
    int **data;
} image;

// define equivalence table

/**
 * Read a file with given file name
 * @return Pointer to an image instance populated
 *         with data from a given file
 */
image *read_image(char *);

/**
 * Saves given image instance as a file with given file name
 * @return Pointer to a file to be saved
 */
FILE *save_image(char *, image *);

/**
 * Clears memory taken by given image
 */
void free_image(image **);

/**
 * Labels components in given image
 * @return Exit code
 */
int label_components(image *);

/**
 * Checks whether given x and y coordinates
 * lie within image bounds
 * @return Logic value
 */
int in_bounds(int, int, int, int);

#endif // __IMAGE__
