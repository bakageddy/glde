#!/usr/bin/env sh

gcc src/*.c lib/*.c -o main $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
