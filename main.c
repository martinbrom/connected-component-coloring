#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main() {
    image *image = read_image("in.pgm");

    if (!image) {
        printf("ERROR: Cannot load image!\n");
        return EXIT_FAILURE;
    }

    save_image("out.pgm", image);

    free_image(&image);

    return EXIT_SUCCESS;
}