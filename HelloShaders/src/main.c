#include "../../include/glad/glad.h"
#include "../../glde/utils.h"
#include "../../glde/shader.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

#define VSHADER_PATH "/home/dev/.software/glde/HelloShaders/shaders/rotate.v.glsl"
#define FSHADER_PATH "/home/dev/.software/glde/HelloShaders/shaders/rainbow.f.glsl"

static int width = 800;
static int height = 600;

GLFWwindow *glde_create_window(const char *title, int width, int height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	return window;
}

int main(void) {
	glfwInit();
	GLFWwindow *window = glde_create_window("Shaders", width, height);
	if (window == NULL) {
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 1) {
		ERR("Failed to initialize GLAD");
		goto defer;
	}

	Shader *vshad = load_shader_from_file(VSHADER_PATH, VERTEX);
	if (vshad == NULL) {
		ERR("Failed to load vertex shader");
		goto defer;
	}

	Shader *fshad = load_shader_from_file(FSHADER_PATH, FRAGMENT);
	if (fshad == NULL) {
		ERR("Failed to load fragment shader!");
		goto defer;
	}

	// TODO: multiple shader compile log, Is that even worth it?
	// Think mark, Think!
	if (compile_shader(vshad) != 0 || compile_shader(fshad) != 0) {
		ERR("Failed to compile shaders!");
		goto defer;
	}

	ShaderProgram *prog = create_shader_program(vshad, fshad);
	if (prog == NULL) {
		ERR("Failed to create program");
		goto defer;
	}

	if (link_shader_program(prog) != 0) {
		char buf[512] = {0};
		get_link_log(prog, buf, 512);
		ERR("Failed to link shader program: %s", buf);
		goto defer;
	}

	while (!glfwWindowShouldClose(window)) {
		glUseProgram(prog -> id);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

defer:
	if (prog != NULL) {
		destroy_program(prog);
	}

	if (vshad != NULL) {
		destroy_shader(vshad);
	}

	if (fshad != NULL) {
		destroy_shader(fshad);
	}

	if (window != NULL) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
	return 0;
}
