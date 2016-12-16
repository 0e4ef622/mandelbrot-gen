#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include "types.h"

static double in_mandelbrot_set(double complex c,
                        long iterations, long escape_radius) {
    long i;
    double complex z = 0;
    for (i = 0; i < iterations; i++) {
        double r = cabs(z);
        if (r > escape_radius) {
            return (double) i + 1 - log10(log10(r)) / log10(2);
        }
        z = z*z+c;
    }
    return 0;
}

static struct rgb color(double i) {
    struct rgb r;
    i = fmod(i, 100.0);
    if (i < 30) {
        double norm = i / 30.0;
        norm *= norm;
        r.r = norm * 0x00;
        r.g = norm * 0x00;
        r.b = norm * 0x55;
    } else if (i < 70) {
        i -= 30.0;

        double norm = i / 40.0;
        r.r = norm * 0xaa;
        r.g = norm * 0xaa;
        r.b = norm * 0xaa + 0x55;

    } else {
        i -= 70.0;

        double norm = i / 30.0;
        r.r = (1.0-norm) * 0xaa;
        r.g = (1.0-norm) * 0xaa;
        r.b = (1.0-norm) * 0xff;
    }

    return r;
}

void generate(FILE *output_file, int image_width, int image_height,
            double xmin, double xmax, double ymin, double ymax,
            long escape_radius, long iterations) {

    struct rgb *line = malloc(image_width * sizeof(struct rgb));
    for (int y = 0; y < image_height; y++) {

        #pragma omp parallel for
        for (int x = 0; x < image_width; x++) {

            /* map pixels to complex numbers according to given ranges then test
               if they're in the set */
            double real = (double)x/(double)image_width * (xmax-xmin) + xmin;
            double imag = (1-(double)y/(double)image_height) * (ymax-ymin) + ymin;
            double i = in_mandelbrot_set(real + I*imag, iterations,
                                                             escape_radius);
            line[x] = color(i);
        }
        fwrite(line, 3, image_width, output_file);
    }
    free(line);
}
