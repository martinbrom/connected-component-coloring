#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "labelling.h"

/**
 * Main application function
 * @param argc
 * @param argv
 * @return Exit code
 */
int main(int argc, char *argv[]) {

    // check program argument count
    if (argc <= 2) {
        printf("How to use:\n");
        printf("ccl.exe <input_image> <output_image>\n");
        return EXIT_FAILURE;
    }

    // load image
    image *img = read_image(argv[1]);
    if (!img) {
        printf("Image loading failed!\n");
        return EXIT_FAILURE;
    }

    // label components
    if (label_components(img)) {
        printf("Component labelling failed!\n");
        return EXIT_FAILURE;
    }

    // save image
    if (save_image(img, argv[2])) {
        printf("Image saving failed!\n");
        return EXIT_FAILURE;
    }

    // free memory taken by image
    free_image(&img);

    return EXIT_SUCCESS;
}