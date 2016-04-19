thing: main.c errnomsg.c errnomsg.h mandelbrot.c mandelbrot.h
	gcc -Wall -Wextra -O3 -fopenmp main.c errnomsg.c mandelbrot.c -lm -o mandelbrot-gen
