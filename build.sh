#!/usr/bin/env sh

set -xe

# gcc ./HelloTriangle/src/exercise1.c lib/*.c -o ./build/exercise1 $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
# gcc ./HelloTriangle/src/exercise2.c lib/*.c -o ./build/exercise2 $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
# gcc ./HelloTriangle/src/exercise3.c lib/*.c -o ./build/exercise3 $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
# gcc ./HelloTriangle/src/main.c lib/*.c -o ./build/HelloTriangle $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address

# gcc ./HelloShaders/src/main.c lib/*.c -o ./build/HelloShaders $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
gcc ./HelloShaders/src/shader_toy.c lib/*.c -o ./build/ShaderToy $(pkg-config --static --libs --cflags glfw3) -I./include/glad/ -Wall -Wextra -Wwrite-strings -std=c11 -ggdb -fsanitize=address
