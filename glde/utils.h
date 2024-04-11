#ifndef GLDE_UTILS_H
#define GLDE_UTILS_H

#include <stdio.h>
#include <GLFW/glfw3.h>

#define LOG(MESSAGE, ...) fprintf(stdout, "LOG: %s:%d: " MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define ERR(MESSAGE, ...) fprintf(stderr, "ERR: %s:%d: " MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);

GLFWwindow *glde_create_window(const char *title, int width, int height);
void glde_handle_resize(GLFWwindow *window, int width, int height);


#ifdef GLDE_UTILS_IMPL

void glde_handle_resize(GLFWwindow *window, int width, int height) {
	(void) window;
	glViewport(0, 0, width, height);
}

GLFWwindow *glde_create_window(const char *title, int width, int height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	return window;
}

#endif // GLDE_UTILS_IMPL
#endif // !__GLDE_UTILS_H
