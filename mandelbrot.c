#include <complex.h>
#include "types.h"

static long in_mandelbrot_set(double complex c, long iterations) {
    long i;
    double complex z = 0;
    for (i = 0; i < iterations; i++) {
        if (cabs(z) > 2) {
            return i;
        }
        z = z*z+c;
    }
    return -1;
}

void generate(struct rgb *image, int image_width, int image_height,
              double centerx, double centery, long iterations) {
    int *histogram = malloc(iterations * sizeof(int));
    int y;
    for (y = 0; y < image_height; y++) {
        int x;
        for (x = 0; x < image_width; x++) {
            long i = in_mandelbrot_set(((double) x / (double) image_width * 3.0 - 2.0) + ((double) y / (double) image_height - 0.5) * 2.0 * I, iterations);
            if (i == -1) {
                image[y*image_width+x].r = 255;
                image[y*image_width+x].g = 255;
                image[y*image_width+x].b = 255;
            } else {
                int c = (int) ((double) i / (double) iterations * 255);
                image[y*image_width+x].r = c;
                image[y*image_width+x].g = c;
                image[y*image_width+x].b = c;
            }
        }
    }
}
