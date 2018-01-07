#include <stdio.h>
#include <stdlib.h>
#include "image.h"

// TODO: Document functions

// TODO: Add to header file
int find_root(int *roots, int item) {
    int parent = roots[item];
    // printf("finding root of item = %d\n", item);

    // if parent is the same as item, we found the set root,
    // else try to look recursively up the tree until we find root
    return parent == item ? item : find_root(roots, parent);
}

// TODO: Add to header file
void unite_sets(int *roots, int x, int y) {

    // printf("uniting x = %d and y = %d\n", x, y);

    int root_x = find_root(roots, x);
    int root_y = find_root(roots, y);

    // both colors already belong to the same set
    if (root_x == root_y) {
        return;
    }

    // set the root with bigger index to the value
    // of the root with smaller index
    if (root_x < root_y) {
        roots[root_y] = roots[root_x];
    } else {
        roots[root_x] = roots[root_y];
    }
}

int label_components(image *image) {
    if (!image) {
        fprintf(stderr, "ERROR: No image given!");
        return EXIT_FAILURE;
    }

    // create equivalence and neighbors table
    printf("Creating arrays...");
    int sets[10000];
    int set_index = 1;  // starts at 1 because color 0 is black, not grey
    int neighbors[4];
    int neighbor_index = 0;
    printf("done\n");

    // all directions to get mask pixels from current
    // pixel in a format of [x_offset, y_offset]
    printf("Creating mask...");
    int mask[4][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
    printf("done\n");

    // first image pass
    printf("First algorithm pass...");
    int x, mx, y, my, m, neighbor = 0, color = 1;
    for (y = 0; y < image->height; y++) {
        for (x = 0; x < image->width; x++) {

            // pixel is black, skip it because
            // it doesn't need to be labelled
            if (image->data[y][x] == 0) {
                continue;
            }

            // clear a table of neighboring colors
            neighbor_index = 0;

            // all mask pixels
            for (m = 0; m < 4; m++) {
                mx = x + mask[m][0];
                my = y + mask[m][1];

                if (!in_bounds(mx, my, image->width, image->height)) {
                    continue;
                }

                // if this mask pixel is not black, add it as a current pixel neighbor
                neighbor = image->data[my][mx];
                if (neighbor) {
                    neighbors[neighbor_index] = neighbor;
                    neighbor_index++;
                }
            }

            // if current pixel has a colored (non-black) neighbor pick the first one
            // else select a new previously not used color
            if (neighbor_index != 0) {
                image->data[y][x] = neighbors[0];
            } else {
                sets[set_index++] = color;
                image->data[y][x] = color;
                color++;
            }

            // unite all colors found in this step, pixel color and mask pixel colors
            int i;
            for (i = 0; i < neighbor_index; i++) {
                neighbor = neighbors[i];
                unite_sets(sets, image->data[y][x], neighbor);
            }
        }
    }

    int i;
    for (i = 0; i < 400; i++) {
        printf("%d\n", sets[i]);
    }

    // second image pass
    // TODO: Second image pass

    printf("all done\n");

    return EXIT_SUCCESS;
}

image *read_image(char *image_name) {

    // check given image name
    if (!image_name) {
        fprintf(stderr, "ERROR: No image name given!\n");
        return NULL;
    }

    // open file
    printf("Opening file...\n");
    FILE *file = fopen(image_name, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Cannot read file!\n");
        return NULL;
    }

    // read the first line
    printf("Reading the first line...\n");
    int buffer_size = 4;
    char buffer[buffer_size];
    fgets(buffer, buffer_size, file);

    // read image width and height
    printf("Reading image width and height...");
    int width, height;
    if (fscanf(file, "%u %u", &width, &height) != 2) {
        fprintf(stderr, "ERROR: Wrong format of width or height!\n");
        fclose(file);
        return NULL;
    }
    printf("w = %d, h = %d\n", width, height);

    // read maximal gray value
    printf("Reading maximal grey value...");
    int max;
    if (fscanf(file, "%u\n", &max) != 1 || max > 255) {
        fprintf(stderr, "ERROR: Wrong format of maximal grey value!\n");
        fclose(file);
        return NULL;
    }
    printf("mgv = %d\n", max);

    // allocate memory for image instance
    printf("Allocating memory for image instance...\n");
    image *image = malloc(sizeof(image));
    if (!image) {
        fclose(file);
        return NULL;
    }

    // allocate memory for image data
    printf("Allocating memory for image data...\n");
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
            while (i != 0) {
                i--;
                free(image->data[i]);
            }

            free(image->data);
            free(image);
            fclose(file);
            return NULL;
        }
    }

    // read data fom file
    printf("Reading data from file...\n");
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (feof(file)) {
                fprintf(stderr, "ERROR: Not enough data in the file %s according to image dimensions!\n", image_name);

                for (i = 0; i < height; i++) {
                    free(image->data[i]);
                }

                free(image->data);
                free(image);
                fclose(file);
                return NULL;
            }

            image->data[i][j] = (uint) fgetc(file);
        }
    }

    image->width = width;
    image->height = height;
    image->max_grey_value = (uint) max;
    fclose(file);

    printf("Loading finished, returning image...\n");
    return image;
}

int save_image(char *image_name, image *image) {
    int i, j;

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
