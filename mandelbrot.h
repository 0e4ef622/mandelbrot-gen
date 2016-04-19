#ifndef MANDELBROT_H
#define MANDELBROT_H

void generate(struct rgb *image, int image_width, int image_height, double xmin,
              double xmax, double ymin, double ymax, long escape_radius, long iterations);

#endif
