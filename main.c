#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "errnomsg.h"
#include "mandelbrot.h"

void output_image_header(FILE *output_file, int image_width, int image_height) {
    fprintf(output_file, "P6 %d %d 255 ", image_width, image_height);
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf(
"Usage: %s [options] output-file\n"
"\n"
"Options:\n"
"    -w, --width        Image width; default is 1200\n"
"    -h, --height       Image height; default is 800\n"
"    -x, --x-range      Range to map pixels to; default is -2,1\n"
"    -y, --y-range      Range to map pixels to; default is -1,1\n"
"    -i, --iterations   Number of iterations to check; default is 1000\n"
"    -c, --center       Specify a center.\n"
"\n"
"When a center is specified, the x and y ranges\n"
"will become relative to the center. If only one\n"
"range is specified, the other will be adjusted to\n"
"match the aspect ratio of the image.\n"
"\n"
"Example usage: ./mandelbrot-gen --center -.745,.13 -x -.008,.008 -w 5464 -h 3072 out.ppm\n"
"Another example identical to the former: ./mandelbrot-gen -x -.753,-.737 -y .12550219619326503,.13449780380673498 -w 5464 -h 3072 out.ppm\n"
"No space is allowed before nor after the comma\n"
"\n"
"When output-file is -, output is to stdout\n", argv[0]);
        exit(0);
    }

    char *output_filename = NULL;
    int image_width = 1200;
    int image_height = 800;
    long iterations = 1000;
    double xmin = 0;
    double xmax = 0;
    double ymin = 0;
    double ymax = 0;
    double centerx = 0;
    double centery = 0;

    bool ddash = false; /* the double dash "--" */
    bool center = false;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            if (!ddash)
                ddash = true;
            else
                output_filename = "--";

        } else if (argv[i][0] == '-' && argv[i][1] == 0) {
            output_filename = argv[i];

        } else if (argv[i][0] == '-' && !ddash) {

#define options(x,y) if (strcmp(argv[i], x) == 0 || strcmp(argv[i], y) == 0)
#define param_chk() if (i+1 == argc) {\
    printf("Missing parameter for '%s'\n", argv[i]); exit(1); }

            options("-w", "--width") {
                param_chk();
                sscanf(argv[++i], "%d", &image_width);

            } else options("-h", "--height") {
                param_chk();
                sscanf(argv[++i], "%d", &image_height);

            } else options("-i", "--iterations") {
                param_chk();
                sscanf(argv[++i], "%ld", &iterations);

            } else options("-x", "--x-range") {
                param_chk();
                if (sscanf(argv[++i], "%lf,%lf", &xmin, &xmax) < 2) {
                    fprintf(stderr, "Bad format for parameter '%s'\n",
                                                                argv[i-1]);
                    return 1;
                } else if (xmin == xmax) {
                    fprintf(stderr, "Invalid range for parameter '%s'\n",
                                                                argv[i-1]);
                    return 1;
                }

            } else options("-y", "--y-range") {
                param_chk();
                if (sscanf(argv[++i], "%lf,%lf", &ymin, &ymax) < 2) {
                    fprintf(stderr, "Bad format for parameter '%s\n",
                                                                argv[i-1]);
                    return 1;
                } else if (ymin == ymax) {
                    fprintf(stderr, "Invalid range for parameter '%s'\n",
                                                                argv[i-1]);
                    return 1;
                }

            } else options("-c", "--center") {
                param_chk();
                if (sscanf(argv[++i], "%lf,%lf", &centerx, &centery) < 2) {
                    fprintf(stderr, "Bad format for parameter '%s\n",
                                                                argv[i-1]);
                    return 1;
                }
                center = true;

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

    if (center) {
        if ((xmin || xmax) && !(ymin || ymax)) {
            /* auto adjust y range according to image size */
            double y = (double) image_height / (double) image_width *
                                                        (xmax - xmin);
            xmin += centerx;
            xmax += centerx;

            ymin = centery - y / 2;
            ymax = centery + y / 2;

        } else if ((ymin || ymax) && !(xmin || xmax)) {
            /* auto adjust x range according to image size */
            double x = (double) image_width / (double) image_height *
                                                        (ymax - ymin);
            ymin += centery;
            ymax += centery;

            xmin = centerx - x / 2;
            xmax = centerx + x / 2;
        } else if ((ymin || ymax) && (xmin || xmax)) {
            fprintf(stderr, "Only specify one range when using '-c' or '--center'\n");
            return 1;
        } else {
            fprintf(stderr, "No range specified, ignoring '--center' option\n");
            ymin = -1;
            ymax = 1;
            xmin = -2;
            xmax = 1;
        }
    } else {
        if (!(xmin || xmax)) {
            xmin = -2;
            xmax = 1;
        }

        if (!(ymin || ymax)) {
            ymin = -1;
            ymax = 1;
        }
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

    output_image_header(output_file, image_width, image_height);

    generate(output_file, image_width, image_height, xmin, xmax,
                                    ymin, ymax, 256, iterations);
    return 0;
}
