thing: main.c errnomsg.c errnomsg.h mandelbrot.c mandelbrot.h
	clang -Wall -Wextra -O3 -lm main.c errnomsg.c mandelbrot.c -o mandelbrot-gen
