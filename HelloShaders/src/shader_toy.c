#include "../../include/glad/glad.h"
#include "../../glde/utils.h"
#include "../../glde/shader.h"
#include "../../glde/vbo.h"
#include "../../glde/vao.h"
#include "../../glde/ibo.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

#define VSHADER_PATH "/home/dinu/.software/glde/HelloShaders/shaders/rotate.v.glsl"
#define FSHADER_PATH "/home/dinu/.software/glde/HelloShaders/shaders/rainbow.f.glsl"

static int width = 600;
static int height = 600;

static float angle = 0.0f;

static float coordinates[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f
};

static unsigned int indices[] = {
	0, 1, 3,
	0, 3, 2,
};

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

void handle_enter(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_REPEAT)
		angle += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		angle += 1.0f;
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

	glViewport(0, 0, width, height);
	glfwSetWindowSizeCallback(window, glde_handle_resize);

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

	if (compile_shader(vshad) != 0) {
		char buf[512] = {0};
		get_compile_log(vshad, buf, 512);
		ERR("Failed to compile vertex shader: %s", buf);
		goto defer;

	}

	if (compile_shader(fshad) != 0) {
		char buf[512] = {0};
		get_compile_log(fshad, buf, 512);
		ERR("Failed to compile fragment shader: %s", buf);
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

	VBO triangle = (VBO) {
		.id = 0,
		.vs = coordinates,
		.size = sizeof(coordinates),
	};

	IBO elems = (IBO) {
		.id = 0,
		.indices = indices,
		.size = sizeof(indices),
	};


	VAO vt = {0};

	vao_init(&vt);

	ibo_init(&elems);
	ibo_load_data(&elems);
	vbo_init(&triangle);
	vbo_load_data(&triangle);

	vao_bind(&vt);
	vbo_bind(&triangle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	vbo_unbind(&triangle);
	vao_unbind(&vt);

	vao_bind(&vt);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {
		glUseProgram(prog -> id);
		int uni_loc = glGetUniformLocation(prog -> id, "angle");
		glUniform1f(uni_loc, angle);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.11328f, 0.125f, 0.1289f, 1.0f);

		ibo_bind(&elems);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		handle_enter(window);
	}
	vao_destroy(&vt);
	vbo_destroy(&triangle);
	ibo_destroy(&elems);

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
