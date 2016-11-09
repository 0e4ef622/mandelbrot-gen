mandelbrot-gen: main.c errnomsg.c errnomsg.h mandelbrot.c mandelbrot.h
	gcc -std=c11 -Wall -Wextra -O3 -fopenmp main.c errnomsg.c mandelbrot.c -lm -o mandelbrot-gen
