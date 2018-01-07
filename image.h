#ifndef __IMAGE__
#define __IMAGE__

#include <stdio.h>

/**
 * Class containing image data
 */
typedef struct {
    size_t width;
    size_t height;
    unsigned int max_grey_value;
    unsigned int **data;
} image;

/**
 * Reads a file with given file name
 * @param filename
 * @return Pointer to an image instance populated
 *         with data from a given file
 */
image *read_image(char *filename);

/**
 * Saves given image instance as a file with given file name
 * @param image
 * @param filename
 * @return Exit code
 */
int save_image(image *image, char *filename);

/**
 * Clears memory taken by given image
 * @param image
 */
void free_image(image **image);

/**
 * Checks whether an image contains only accepted colors,
 * black or white
 * @param image
 * @return True (1) when image contains only black and white
 *         pixels, False (0) otherwise
 */
int is_black_white(image *image);

/**
 * Checks whether given x and y coordinates
 * lie within image bounds
 * @param x
 * @param y
 * @param w
 * @param h
 * @return True (1) if given coordinates are within
 *         image bounds, False (0) otherwise
 */
int in_bounds(int x, int y, size_t w, size_t h);

#endif // __IMAGE__
