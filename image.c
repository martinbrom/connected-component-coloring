#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

image *read_image(char *image_name) {
    FILE *input_file = NULL;
    image *img = NULL;
    size_t width, height;
    unsigned int max, i, j;
    int buffer_length = 4;
    char buffer[buffer_length];

    if (!image_name) {
        fprintf(stderr, "ERROR: no filename given!\n");
        return NULL;
    }

    /* open file */
    input_file = fopen(image_name, "r");
    if (!input_file) {
        fprintf(stderr, "ERROR: cannot read the file %s!\n", image_name);
        return NULL;
    }

    /* read the first line with the magic number and check if it equals the PGM_P5 magic number */
    if (!fgets(buffer, buffer_length, input_file) || strncmp(buffer, "P5", 2) != 0){
        fprintf(stderr, "ERROR: File is not in correct format!\n");
        fclose(input_file);
        return NULL;
    }

    /* read the image width and height */
    if (fscanf(input_file, "%zu %zu", &width, &height) != 2) {
        fprintf(stderr, "ERROR: File has width and height in the wrong format!\n");
        fclose(input_file);
        return NULL;
    }

    /* read the max gray value */
    if (fscanf(input_file, "%u\n", &max) != 1 || max > 255) {
        fprintf(stderr, "ERROR: File has max grey item in the wrong format!\n");
        fclose(input_file);
        return NULL;
    }

    /* alloc image */
    img = malloc(sizeof(image));
    if (!img) {
        fclose(input_file);
        return NULL;
    }

    /* alloc image data*/
    img->data = (unsigned int **) malloc(height * sizeof(unsigned int *));
    if (!img->data) {
        free(img);
        fclose(input_file);
        return NULL;
    }
    for (i = 0; i < height; i++) {
        img->data[i] = (unsigned int *) malloc(width * sizeof(unsigned int));
        if (!img->data[i]) {
            while (i) {
                free(img->data[--i]);
            }
            free(img->data);
            free(img);
            fclose(input_file);
            return NULL;
        }
    }

    /* read data from the file */
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (feof(input_file)) {
                fprintf(stderr, "ERROR: not enough data in the file according to image dimensions!\n");
                for (i = 0; i < height; i++) {
                    free(img->data[i]);
                }
                free(img->data);
                free(img);
                fclose(input_file);
                return NULL;
            }
            img->data[i][j] = (unsigned int) fgetc(input_file);
        }
    }

    img->width = width;
    img->height = height;
    img->max_grey_value = max;

    fclose(input_file);

    return img;
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
    fprintf(file, "%zu %zu\n", image->width, image->height);
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
    int i;

    if (!image || !*image) {
        return;
    }

    for (i = 0; i < (*image)->height; i++) {
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

    return 1;
}

int in_bounds(int x, int y, size_t w, size_t h) {
    return x >= 0 && x < w && y >= 0 && y < h;
}
