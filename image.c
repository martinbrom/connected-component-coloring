#include <stdio.h>
#include <stdlib.h>
#include "image.h"

image *read_image(char *image_name) {
    FILE *file;
    int buffer_size = 4;
    char buffer[buffer_size];
    size_t width, height;
    unsigned int max;
    image *image;
    int i, j;

    // check given image name
    if (!image_name) {
        fprintf(stderr, "ERROR: No image name given!\n");
        return NULL;
    }

    // open file
    file = fopen(image_name, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Cannot read file!\n");
        return NULL;
    }

    // read the first line
    fgets(buffer, buffer_size, file);

    // read image width and height
    if (fscanf(file, "%zu %zu", &width, &height) != 2) {
        fprintf(stderr, "ERROR: Wrong format of width or height!\n");
        fclose(file);
        return NULL;
    }

    // read maximal gray value
    if (fscanf(file, "%u\n", &max) != 1 || max > 255) {
        fprintf(stderr, "ERROR: Wrong format of maximal grey value!\n");
        fclose(file);
        return NULL;
    }

    // allocate memory for image instance
    image = malloc(sizeof(image));
    if (!image) {
        fclose(file);
        return NULL;
    }

    // allocate memory for image data
    image->data = (unsigned int **) malloc(height * sizeof(unsigned int *));
    if (!image->data) {
        free(image);
        fclose(file);
        return NULL;
    }

    for (i = 0; i < height; i++) {
        image->data[i] = (unsigned int *) malloc(width * sizeof(unsigned int));

        if (!image->data[i]) {
            i--;
            for (i; i >= 0; i--) {
                free(image->data[i]);
            }

            free(image->data);
            free(image);
            fclose(file);
            return NULL;
        }
    }

    // read data fom file
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            // maybe check for not enough data?
            image->data[i][j] = (unsigned int) fgetc(file);
        }
    }

    printf("HERE\n");
    image->width = width;
    printf("HERE\n");
    image->height = height;
    printf("HERE\n");
    image->max_grey_value = max;
    printf("HERE\n");
    fclose(file);
    printf("HERE\n");
    return image;
}

int save_image(image *image, char *image_name) {
    FILE *file = NULL;
    int i, j;
    // check image and image name?

    // open file
    file = fopen(image_name, "w");
    if (!file) {
        fprintf(stderr, "ERROR: Cannot create file!\n");
        return EXIT_FAILURE;
    }

    // write data
    fprintf(file, "%s\n", "P5");
    fprintf(file, "%zu %zu\n", image->width, image->height);
    fprintf(file, "%u\n", image->max_grey_value);

    // write all image pixels
    for (i = 0; i < image->height; ++i) {
        for (j = 0; j < image->width; ++j) {
            fputc(image->data[i][j], file);
        }
    }

    // write and close
    fflush(file);
    fclose(file);

    return EXIT_SUCCESS;
}

/**
 *
 * @param image
 */
void free_image(image **image) {
    size_t image_height;
    int i;

    if (!image || !*image) {
        return;
    }

    image_height = (*image)->height;
    for (i = 0; i < image_height; i++) {
        free((*image)->data[i]);
    }

    free((*image)->data);
    free((*image));

    *image = NULL;
}

/**
 *
 * @param image
 * @return
 */
int is_black_white(image *image) {
    unsigned int i, j;
    for (i = 0; i < image->height; i++) {
        for (j = 0; j < image->width; j++) {
            if (image->data[i][j] != 255 && image->data[i][j] != 0) {
                return 0;
            }
        }
    }

    return 1;
}

/**
 *
 * @param x
 * @param y
 * @param w
 * @param h
 * @return
 */
int in_bounds(int x, int y, size_t w, size_t h) {
    return x >= 0 && x < w && y >= 0 && y < h;
}
