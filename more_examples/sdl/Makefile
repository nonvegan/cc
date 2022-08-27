CC=gcc
NAME=sdl
SRC=sdl.c ../../src/canvas.c
CFLAGS= -std=c11 -Wall -Wextra -pedantic
LIBS=-lm
PKGS=sdl2

$(NAME): $(SRC)
	$(CC) -o $(NAME) $(SRC) $(FLAGS) $(LIBS) `pkg-config --libs --cflags $(PKGS)` -ggdb

run: $(NAME)
	./$(NAME)
