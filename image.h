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

// TODO: Add parameter names

/**
 *
 * @return
 */
int label_components(image *);

/**
 * Reads a file with given file name
 * @return Pointer to an image instance populated
 *         with data from a given file
 */
image *read_image(char *);

/**
 * Saves given image instance as a file with given file name
 * @return Exit code
 */
int save_image(char *, image *);

/**
 * Clears memory taken by given image
 */
void free_image(image **);

/**
 * Checks whether an image contains only accepted colors,
 * black or white
 * @return True (1) when image contains only black and white
 *         pixels, False (0) otherwise
 */
int is_black_white(image *);

/**
 * Checks whether given x and y coordinates
 * lie within image bounds
 * @return True (1) if given coordinates are within
 *         image bounds, False (0) otherwise
 */
int in_bounds(int, int, int, int);

#endif // __IMAGE__
