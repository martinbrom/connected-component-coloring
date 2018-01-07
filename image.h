#ifndef __IMAGE__
#define __IMAGE__

#include <stdio.h>

/**
 * Class containing image data
 */
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int max_grey_value;
    unsigned int **data;
} image;

/**
 * Prints an error message to stderr and closes given file
 * @param message
 * @param file
 */
void reading_error(char *message, FILE *file);

/**
 * Reads a file with given file name and validates data
 * @param filename
 * @return Pointer to an image instance populated
 *         with data from a given file
 */
image *read_image(char *filename);

/**
 * Saves given image instance as a file with given file name
 * @param img
 * @param filename
 * @return Exit code
 */
int save_image(image *img, char *filename);

/**
 * Clears memory taken by given image
 * @param img
 */
void free_image(image **img);

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
int in_bounds(int x, int y, int w, int h);

#endif // __IMAGE__
