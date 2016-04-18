#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "errnomsg.h"
#include "mandelbrot.h"

void output_image(FILE *output_file, struct rgb* image, int image_width, int image_height) {
    fprintf(output_file, "P6 %d %d 255 ", image_width, image_height);
    fwrite(image, image_height, image_width * sizeof(struct rgb), output_file);
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage: %s [options] output-file\n"
               "\n"
               "Options:\n"
               "    -w, --width        Image width; default is 1200\n"
               "    -h, --height       Image height; default is 800\n"
               "    -c, --zoom-origin  Zoom origin; default is -0.5,0\n"
               "    -z, --zoom         Zoom percentage; default is 100\n"
               "    -i, --iterations   Number of iterations to check; default is 1000\n"
               "\n"
               "When output-file is -, output to stdout\n", argv[0]);
        exit(0);
    }

    char *output_filename = NULL;
    int image_width = 1200;
    int image_height = 800;
    long iterations = 1000;
    double centerx = -0.5;
    double centery = 0;

    bool ddash = false; /* the double dash "--" */
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            ddash = true;
        } else if (argv[i][0] == '-' && argv[i][1] == 0) {
            output_filename = argv[i];
        } else if (argv[i][0] == '-' && !ddash) {

#define options(x,y) if (strcmp(argv[i], x) == 0 || strcmp(argv[i], y) == 0)
#define param_chk() if (i+1 == argc) { printf("Missing parameter for '%s'\n", argv[i]); exit(1); }

            /* TODO --zoom and --zoom-origin */
            options("-w", "--width") {
                param_chk();
                sscanf(argv[++i], "%d", &image_width);

            } else options("-h", "--height") {
                param_chk();
                sscanf(argv[++i], "%d", &image_height);

            } else options("-i", "--iterations") {
                param_chk();
                sscanf(argv[++i], "%ld", &iterations);

            } else {
                printf("Unrecognized option '%s'\n", argv[i]);
                exit(1);
            }

#undef options
#undef param_chk

        } else {
            output_filename = argv[i];
        }
    }

    if (output_filename == NULL) {
        fprintf(stderr, "No output file specified\n");
        return 1;
    }

    FILE *output_file;

    if (output_filename[0] == '-' && output_filename[1] == 0) {
        output_file = stdout;
    } else {
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            errno_msg(output_filename);
            return 1;
        }
    }

    struct rgb *image = malloc(image_width * image_height * sizeof(struct rgb));
    generate(image, image_width, image_height, centerx, centery, iterations);

    output_image(output_file, image, image_width, image_height);

    return 0;
}
