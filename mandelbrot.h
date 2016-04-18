#ifndef MANDELBROT_H
#define MANDELBROT_H

void generate(struct rgb *image, int image_width, int image_height,
              double centerx, double centery, long iterations);

#endif
