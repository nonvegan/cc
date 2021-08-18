main: src/main.c src/canvas.c
	gcc -Wall -Wextra -pedantic -o main src/main.c src/canvas.c src/yuv.c -lm
