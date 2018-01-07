#include <stdio.h>
#include <stdlib.h>
#include "image.h"

/**
 * Prints given error message to stderr with
 * an exclamation mark and a newline at the end
 * and closes given file
 * @param message
 * @param file
 */
void reading_error(char *message, FILE *file) {
    fprintf(stderr, "%s", message);
    fprintf(stderr, "!\n");
    fclose(file);
}

/**
 * Reads a file with given image name, reads
 * needed data and validates, whether it is
 * in correct format and has valid values
 * @param image_name
 * @return Pointer to image
 */
image *read_image(char *image_name) {
    const unsigned int BUFFER_LENGTH = 4;
    unsigned int max;           // maximal grey value
    unsigned int x, y;          // loop variables
    unsigned int width, height; // image width and height
    image *img = NULL;          // pointer to image instance
    FILE *input_file = NULL;    // input file
    char buffer[BUFFER_LENGTH];
    char *file_type;            // PGM file type

    // open file for reading
    input_file = fopen(image_name, "r");
    if (!input_file) {
        reading_error("Cannot read file", input_file);
        return NULL;
    }

    // read first line
    file_type = fgets(buffer, BUFFER_LENGTH, input_file);
    if (file_type[0] != 'P') {
        reading_error("Invalid PGM file type", input_file);
        return NULL;
    }

    if (file_type[1] != '5') {
        reading_error("Valid PGM file type, but this application only processes P5", input_file);
        return NULL;
    }

    // read width and height
    if (fscanf(input_file, "%u %u\n", &width, &height) != 2) {
        reading_error("Incorrect width and height format", input_file);
        return NULL;
    }

    if (width == 0) {
        reading_error("Width too small", input_file);
        return NULL;
    }

    if (height == 0) {
        reading_error("Height too small", input_file);
        return NULL;
    }

    // read maximal grey value
    if (fscanf(input_file, "%u\n", &max) != 1) {
        reading_error("Incorrect maximal grey value format", input_file);
        return NULL;
    }

    if (max > 255) {
        reading_error("Maximal grey value too big", input_file);
        return NULL;
    }

    // allocate memory for image
    img = malloc(sizeof(image));

    img->width = width;
    img->height = height;
    img->max_grey_value = max;

    // allocate memory for image data
    img->data = (unsigned int **) malloc(height * sizeof(unsigned int *));

    // allocate memory for each image pixel line
    for (y = 0; y < height; y++) {
        img->data[y] = (unsigned int *) malloc(width * sizeof(unsigned int));

        for (x = 0; x < width; x++) {
            img->data[y][x] = (unsigned int) fgetc(input_file);
        }
    }

    fclose(input_file);

    // check whether the image contains only black and white colored pixels
    for (y = 0; y < img->height; y++) {
        for (x = 0; x < img->width; x++) {
            if (img->data[y][x] != 255 && img->data[y][x] != 0) {
                printf("Image cannot contain grey pixels!\n");
                free_image(&img);
                return NULL;
            }
        }
    }

    return img;
}

/**
 * Saves given image as a given image_name
 * in the PGM P5 format
 * @param img
 * @param image_name
 * @return Exit code
 */
int save_image(image *img, char *image_name) {
    int x, y;
    FILE *file;

    // open file for writing
    file = fopen(image_name, "w");
    if (!file) {
        fprintf(stderr, "File creation failed!\n");
        return EXIT_FAILURE;
    }

    // write data
    fprintf(file, "%s\n", "P5");
    fprintf(file, "%u %u\n", img->width, img->height);
    fprintf(file, "%u\n", img->max_grey_value);

    for (y = 0; y < img->height; ++y) {
        for (x = 0; x < img->width; ++x) {
            fputc(img->data[y][x], file);
        }
    }

    fflush(file);
    fclose(file);
    return EXIT_SUCCESS;
}

/**
 * Clears memory taken by image line by line,
 * then clears image data pointer and finally
 * the image itself
 * @param img
 */
void free_image(image **img) {
    int y;
    int image_height = (*img)->height;

    // free each image line
    for (y = 0; y < image_height; y++) {
        free((*img)->data[y]);
    }

    free((*img)->data);
    free((*img));
    *img = NULL;
}

/**
 * Given x and y coordinates lie within the image
 * if both are not negative and x is smaller than
 * image width and y is smaller than image height
 * @param x
 * @param y
 * @param w
 * @param h
 * @return True (1) if given coordinates are within
 *         image bounds, False (0) otherwise
 */
int in_bounds(int x, int y, int w, int h) {
    return x >= 0 && x < w && y >= 0 && y < h;
}