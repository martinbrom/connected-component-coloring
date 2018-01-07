#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char *argv[]) {

    // check program arguments
    if (argc < 3) {
        printf("How to use:\n");
        printf("Use 'ccl.exe <input_image> <output_image>' on windows\n");
        printf("Use 'ccl <input_image> <output_image>' on linux\n");
        return EXIT_FAILURE;
    }

    // load image
    image *image = read_image(argv[1]);
    if (!image) {
        printf("ERROR: Cannot load image!\n");
        return EXIT_FAILURE;
    }

    // check whether the image contains only black and white color
    if (!is_black_white(image)) {
        printf("ERROR: Image must contain only black (0x00) and white (0xFF) pixels!\n");
        return EXIT_FAILURE;
    }

    // label components
    if (label_components(image)) {
        printf("ERROR: Something went wrong during the process of labeling components!\n");
        return EXIT_FAILURE;
    }

    // save image
    if (save_image(argv[2], image)) {
        printf("ERROR: Something went wrong during the labeling!\n");
        return EXIT_FAILURE;
    }

    // free memory taken by image
    free_image(&image);

    return EXIT_SUCCESS;
}