#!/usr/bin/env sh

gcc ./src/exercise1.c lib/*.c -o exercise1 $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
gcc ./src/exercise2.c lib/*.c -o exercise2 $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
gcc src/main.c lib/*.c -o main $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
