#include <stdio.h>
#include <stdlib.h>
#include "image.h"

image *read_image(char *image_name) {

    // check given image name
    if (!image_name) {
        fprintf(stderr, "ERROR: No image name given!\n");
        return NULL;
    }

    // open file
    FILE *file = fopen(image_name, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Cannot read file!\n");
        return NULL;
    }

    // read the first line
    int buffer_size = 4;
    char buffer[buffer_size];
    fgets(buffer, buffer_size, file);

    // read image width and height
    int width, height;
    if (fscanf(file, "%u %u", &width, &height) != 2) {
        fprintf(stderr, "ERROR: Wrong format of width or height!\n");
        fclose(file);
        return NULL;
    }

    // read maximal gray value
    int max;
    if (fscanf(file, "%u\n", &max) != 1 || max > 255) {
        fprintf(stderr, "ERROR: Wrong format of maximal grey value!\n");
        fclose(file);
        return NULL;
    }

    // allocate memory for image instance
    image *image = malloc(sizeof(image));
    if (!image) {
        fclose(file);
        return NULL;
    }

    // allocate memory for image data
    image->data = (int **) malloc(height * sizeof(int *));
    if (!image->data) {
        free(image);
        fclose(file);
        return NULL;
    }

    int i, j;
    for (i = 0; i < height; i++) {
        image->data[i] = (int *) malloc(width * sizeof(int));

        if (!image->data[i]) {
            for (i; i <= 0; i--) {
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
            image->data[i][j] = (uint) fgetc(file);
        }
    }

    image->width = width;
    image->height = height;
    image->max_grey_value = (uint) max;
    fclose(file);

    return image;
}

int save_image(image *image, char *image_name) {
    if (!image) {
        fprintf(stderr, "ERROR: Image instance missing!");
        return EXIT_FAILURE;
    }

    if (!image_name) {
        fprintf(stderr, "ERROR: Image name missing!");
        return EXIT_FAILURE;
    }

    // open file
    FILE *file = NULL;
    file = fopen(image_name, "w");
    if (!file) {
        fprintf(stderr, "ERROR: Cannot create file!\n");
        return EXIT_FAILURE;
    }

    // write data
    fprintf(file, "%s\n", "P5");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "%d\n", image->max_grey_value);

    int i, j;
    for (i = 0; i < image->height; ++i) {
        for (j = 0; j < image->width; ++j) {
            fputc(image->data[i][j], file);
        }
    }

    fflush(file);
    fclose(file);
    return EXIT_SUCCESS;
}

void free_image(image **image) {
    if (!image || !*image) {
        return;
    }

    int i;
    int image_height = (*image)->height;
    for (i = 0; i < image_height; i++) {
        free((*image)->data[i]);
    }

    free((*image)->data);
    free((*image));

    *image = NULL;
}

int is_black_white(image *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            if (image->data[i][j] != 255 && image->data[i][j] != 0) {
                return 0;
            }
        }
    }

    // printf("Image is black and white only...\n");
    return 1;
}

int in_bounds(int x, int y, int w, int h) {
    return x >= 0 && x < w && y >= 0 && y < h;
}
