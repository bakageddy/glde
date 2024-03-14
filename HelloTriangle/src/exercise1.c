#include <stdlib.h>
#include <stdio.h>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include "utils.h"

#define GLDE_SHADER_IMPL
#include "shader.h"

#define GLDE_VBO_IMPL
#include "vbo.h"

#define LOG_BUFFER_SIZE 512

static int width = 800;
static int height = 600;

static GLfloat vertices[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};

void window_resize_callback(GLFWwindow *window, int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

GLFWwindow *MakeWindow(int width, int height, const char *title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	return window;
}


int InitGLAD(void) {
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return result;
}

int main(void) {
	LOG("Initialize GLFW");
	glfwInit();

	LOG("Create GLFW window");
	GLFWwindow *window = MakeWindow(width, height, "Chapter 1 Exercise 1");
	if (window == NULL) {
		goto defer;
	}

	glfwMakeContextCurrent(window);

	LOG("Initialize GLAD");
	int result = InitGLAD();
	if (result == 0) {
		goto defer;
	}

	LOG("Load Vertex Shaders");
	Shader *vertex_sh = load_shader_from_file("./shaders/exercise1.vertex.glsl", VERTEX);
	if (vertex_sh == NULL) {
		goto defer;
	}

	LOG("Compile Vertex Shaders");
	result = compile_shader(vertex_sh);
	if (result != 0) {
		char buf[LOG_BUFFER_SIZE] = {0};
		get_compile_log(vertex_sh, buf, LOG_BUFFER_SIZE);
		LOG("Failed to compile shader: %s\n", buf);
		goto defer;
	}

	LOG("Load Fragment Shaders");
	Shader *fragment_sh = load_shader_from_file("./shaders/exercise1.frag.glsl", FRAGMENT);
	if (fragment_sh == NULL) {
		goto defer;
	}

	LOG("Compile Fragment Shaders");
	result = compile_shader(vertex_sh);
	if (result != 0) {
		char buf[LOG_BUFFER_SIZE] = {0};
		get_compile_log(fragment_sh, buf, LOG_BUFFER_SIZE);
		LOG("Failed to compile shader: %s\n", buf);
		goto defer;
	}

	LOG("Create Shader Program")
	ShaderProgram *prog = create_shader_program(vertex_sh, fragment_sh);
	if (prog == NULL) {
		ERR("Failed to create shader program");
		goto defer;
	}

	LOG("Link Shaders");
	result = link_shader_program(prog);
	if (result != 0) {
		char buf[LOG_BUFFER_SIZE] = {0};
		get_link_log(prog, buf, LOG_BUFFER_SIZE);
		LOG("Failed to link shader: %s\n", buf);
		goto defer;
	}

	VBO tri = (VBO) {
		.id = 0,
		.vs = vertices,
		.size = sizeof(vertices),
	};

	vbo_init(&tri);
	vbo_load_data(&tri);

	LOG("Start Main Loop");
	glViewport(0, 0, width, height);
	glfwSetWindowSizeCallback(window, window_resize_callback);
	while (!glfwWindowShouldClose(window)) {

		glUseProgram(prog -> id);
		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.0f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

defer:
	if (window)
		glfwDestroyWindow(window);
	if (vertex_sh)
		destroy_shader(vertex_sh);
	if (fragment_sh)
		destroy_shader(fragment_sh);
	if (prog)
		destroy_program(prog);
	vbo_destroy(&tri);
	glfwTerminate();
	return 0;
}
