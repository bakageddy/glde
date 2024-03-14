#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG(MESSAGE, ...) fprintf(stdout, "LOG: %s:%d:" MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define ERR(MESSAGE, ...) fprintf(stderr, "ERR: %s:%d:" MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

void window_resize_callback(GLFWwindow *window, int width, int height) {
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(w, true);
	}
}

GLfloat triangle_vertices[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

GLuint triangle_indices[] = {
	0, 1, 2,
	2, 3, 0,
};

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
	return file_contents;
}

int main(int argc, char** argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn Me Some Open GL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to initialize window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize glad\n");
		goto defer;
		return 1;
	}


	GLuint triangle_vshader = glCreateShader(GL_VERTEX_SHADER);
	const char *vertex_shader_source = get_shader_source("./shaders/triangle.vertex.glsl");
	if (vertex_shader_source == NULL) {
		goto defer;
	}

	glShaderSource(triangle_vshader, 1, &vertex_shader_source, NULL);
	glCompileShader(triangle_vshader);

	GLint vcompile_stat;
	glGetShaderiv(triangle_vshader, GL_COMPILE_STATUS, &vcompile_stat);
	if (vcompile_stat != GL_TRUE) {
		char log[512];
		glGetShaderInfoLog(triangle_vshader, 512, NULL, log);
		fprintf(stderr, "Failed to compile shader: %s\n", log);
		goto defer;
	}

	GLuint triangle_fshader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragment_shader_source = get_shader_source("./shaders/triangle.frag.glsl");
	if (fragment_shader_source == NULL) {
		goto defer;
	}

	glShaderSource(triangle_fshader, 1, &fragment_shader_source, NULL);
	glCompileShader(triangle_fshader);

	glGetShaderiv(triangle_fshader, GL_COMPILE_STATUS, &vcompile_stat);
	if (vcompile_stat != GL_TRUE) {
		char log[512];
		glGetShaderInfoLog(triangle_fshader, 512, NULL, log);
		LOG("Failed to compile shader: %s\n", log);
		goto defer;
	}

	GLuint shader_prog = glCreateProgram();
	glAttachShader(shader_prog, triangle_vshader);
	glAttachShader(shader_prog, triangle_fshader);
	glLinkProgram(shader_prog);
	
	glGetProgramiv(shader_prog, GL_LINK_STATUS, &vcompile_stat);
	if (vcompile_stat != GL_TRUE) {
		char log[512];
		glGetProgramInfoLog(shader_prog, 512, NULL, log);
		LOG("Failed to link shader: %s\n", log);
		goto defer;
	}

	glDeleteShader(triangle_fshader);
	glDeleteShader(triangle_vshader);

	LOG("Compiled shaders!");

	GLuint triangle_vao;
	GLuint triangle_vbo;
	GLuint vertex_ebo;
	glGenVertexArrays(1, &triangle_vao);
	glGenBuffers(1, &triangle_vbo);
	glGenBuffers(1, &vertex_ebo);

	glBindVertexArray(triangle_vao);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof(float)), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetWindowSizeCallback(window, window_resize_callback);

	while (!glfwWindowShouldClose(window)) {
		process_input(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_prog);
		glBindVertexArray(triangle_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ebo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &triangle_vao);
	glDeleteBuffers(1, &triangle_vbo);
	glDeleteBuffers(1, &vertex_ebo);
	glDeleteProgram(shader_prog);

defer:
	free((void*) vertex_shader_source);
	free((void*) fragment_shader_source);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
