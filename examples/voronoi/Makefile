CC=gcc
NAME=voronoi
SRC=voronoi.c ../../src/canvas.c ../../src/vec.c ../../src/jpeg.c
CFLAGS= -std=c11 -Wall -Wextra -pedantic
LIBS=-lm
PKGS=libjpeg

$(NAME): $(SRC)
	$(CC) -o $(NAME) $(SRC) $(FLAGS) $(LIBS) `pkg-config --libs $(PKGS)` -ggdb

run: $(NAME)
	./$(NAME) && feh voronoi.png
