#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include <errno.h>

typedef struct {
	GLuint id;
	char *src;
} VShader;

static int width = 800;
static int height = 600;

#define LOG(MESSAGE, ...) fprintf(stdout, "LOG: %s:%d:" MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define ERR(MESSAGE, ...) fprintf(stderr, "ERR: %s:%d:" MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);

GLFWwindow *MakeWindow(int width, int height, const char *title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	return window;
}

char *get_shader_source(const char *path) {
	int handle = open(path, O_RDONLY);
	if (handle == -1) {
		LOG("Failed to open file: %s", strerror(errno));
		return NULL;
	}
	struct stat info;
	fstat(handle, &info);
	int size = info.st_size;
	char *file_contents = malloc((size + 1) * sizeof(char));

	if (!file_contents) return NULL;
	if (read(handle, file_contents, size + 1) == -1) {
		LOG("Failed to read file: %s", strerror(errno));
		return NULL;
	}
	file_contents[size] = '\0';
	close(handle);
	return file_contents;
}

int InitGLAD(void) {
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return result;
}

int main(void) {
	glfwInit();

	GLFWwindow *window = MakeWindow(width, height, "Chapter 1 Exercise 1");
	if (window == NULL) {
		goto defer;
	}

	glfwMakeContextCurrent(window);

	int result = InitGLAD();
	if (result == 1) {
		goto defer;
	}

defer:
	if (window)
		glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
