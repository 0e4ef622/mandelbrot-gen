#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    return 0;
}

static struct rgb color(long i, long iterations) {
    struct rgb r;
    i %= 500;
    if (i < 30) {
        double norm = (double) i / 50.0;
        r.r = norm * 0x00;
        r.g = norm * 0x00;
        r.b = norm * 0x55;

    } else if (i < 200) {
        i -= 30;

        double norm = (1 - (double) i / 170.0);
        r.r = norm * 0x88;
        r.g = norm * 0x55;
        r.b = norm * 0x55 + 0x55;

    } else {
        i -= 200;

        double norm = (1 - (double) i / 200.0);
        r.r = (1-norm) * 0x88;
        r.g = (1-norm) * 0x55;
        r.b = (1-norm) * 0xaa;
    }

    return r;
}

void generate(struct rgb *image, int image_width, int image_height,
              double xmin, double xmax, double ymin, double ymax, long iterations) {

#pragma omp parallel for
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {

            /* map pixels to complex numbers according to given ranges then test if they're in the set */
            //long i = in_mandelbrot_set(((double) x / (double) image_width * 3.0 - 2.0) + ((double) y / (double) image_height - 0.5) * 2.0 * I, iterations);
            long i = in_mandelbrot_set((double) x / (double) image_width * (xmax - xmin) + xmin  +  I * ((1 - (double) y / (double) image_height) * (ymax - ymin) + ymin), iterations);

            long index = y*image_width+x;
            //int c = (int) (pow((double) i / (double) iterations, 0.5) * 255);
            //int c = (int) ((double) i / (double) iterations * 255);
            image[index] = color(i, iterations);
        }
    }
}
