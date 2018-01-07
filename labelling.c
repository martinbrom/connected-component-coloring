#include <stdlib.h>
#include "labelling.h"
#include "disjoint_set.h"

/**
 * Colors image components with distinct grey colors
 * using disjoint sets and performing union finds
 * A mask is applied to each image pixel and if any
 * grey or white (not black) pixels are found in the mask,
 * current pixel is colored with a color of one of mask
 * pixels and all mask pixels' colors are united (belong to one component)
 * After that, each pixel is looped through again and
 * its value is set to the value of its distinct set's
 * root element and maximal grey value of image is found
 * @param image
 * @return Colored image
 */
int label_components(image *image) {
    unsigned int neighbors[4];   // array of mask pixel colors
    unsigned int set_index = 1;  // starts at 1 because color 0 is black, not grey
    unsigned int neighbor_index = 0;
    unsigned int sets[10000];    // array of disjoint set roots
    unsigned int i;

    // initialize all items as their own distinct sets
    for (i = 0; i < 10000; i++) {
        sets[i] = i;
    }

    // all directions to get mask pixels from current
    // pixel in a format of [x_offset, y_offset] clockwise
    int mask[4][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    int x, mx, y, my, m;
    unsigned int neighbor, color = 1;
    unsigned int root;
    unsigned int max_grey_value;

    // first image pass
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
            for (i = 0; i < neighbor_index; i++) {
                neighbor = neighbors[i];
                unite_sets(sets, image->data[y][x], neighbor);
            }
        }
    }

    // second image pass
    // set color of each pixel to the value of its disjoint set root
    // and find the maximal grey pixel value
    max_grey_value = 0;
    for (y = 0; y < image->height; y++) {
        for (x = 0; x < image->width; x++) {
            root = find_root(sets, image->data[y][x]);
            image->data[y][x] = root;

            if (root > max_grey_value) {
                max_grey_value = root;
            }
        }
    }

    image->max_grey_value = max_grey_value;

    return EXIT_SUCCESS;
}