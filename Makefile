CC=gcc
NAME=main
SRC=src/main.c src/canvas.c src/y4m2.c src/gl.c src/x11.c src/vec.c src/glfw.c src/gif.c src/png.c src/jpeg.c
CFLAGS= -std=c11 -Wall -Wextra -pedantic
LIBS=-lm -lgif
PKGS=glfw3 gl glew x11 libpng libjpeg

$(NAME): $(SRC)
	$(CC) -o $(NAME) $(SRC) $(FLAGS) $(LIBS) `pkg-config --libs $(PKGS)` -ggdb

run: $(NAME)
	./$(NAME) && feh KEKW.jpeg
