#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "shader.h"
#include "utils.h"
#include "vao.h"
#include "vbo.h"

static int width = 800;
static int height = 600;

float triangle1[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};

float triangle2[] = {
	-1.0f, -1.0f, +0.0f,
	-1.0f, +0.0f, +0.0f,
	+0.0f, +0.0f, +0.0f
};

void window_resize_callback(GLFWwindow *window, int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

GLFWwindow *MakeWindow(int w, int h, const char *title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *wnd = glfwCreateWindow(w, h, title, NULL, NULL);
	return wnd;
}

int InitGLAD(void) {
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return result;
}

int main(void) {
	glfwInit();
	GLFWwindow *win = MakeWindow(width, height, "Exercise 2");
	if (win == NULL) {
		ERR("Failed to create window\n");
		goto defer;
	}

	glfwMakeContextCurrent(win);

	if (InitGLAD() == 0) {
		ERR("Failed to initialise glad!\n");
		goto defer;
	}

	glViewport(0, 0, width, height);
	glfwSetWindowSizeCallback(win, window_resize_callback);

	Shader *vert = NULL;
	vert = load_shader_from_file("./shaders/triangle.vertex.glsl", VERTEX);
	if (vert == NULL) {
		LOG("Failed to load vertex shader from file\n");
		goto defer;
	}

	Shader *frag = NULL;
	frag = load_shader_from_file("./shaders/triangle.frag.glsl", FRAGMENT);
	if (frag == NULL) {
		LOG("Failed to load fragment shader from file!\n");
		goto defer;
	}

	if (compile_shader(vert) != 0) {
		LOG("Failed to compile vertex shader!\n");
		goto defer;
	}

	if (compile_shader(frag) != 0) {
		LOG("Failed to compile fragment shader!\n");
		goto defer;
	}

	ShaderProgram *prg = create_shader_program(vert, frag);
	if (prg == NULL) {
		LOG("Failed to allocate a shader program");
		goto defer;
	}

	if (link_shader_program(prg) != 0) {
		LOG("Failed to link a shader program");
		goto defer;
	}

	VBO a = (VBO) {
		.id = 0,
		.vs = triangle1,
		.size = sizeof(triangle1),
	};

	VBO b = (VBO) {
		.id = 0,
		.vs = triangle2,
		.size = sizeof(triangle2),
	};

	VAO arra = (VAO) {
		.id = 0,
	};

	VAO arrb = (VAO) {
		.id = 0,
	};

	vao_init(&arra);
	vao_init(&arrb);
	vbo_init(&a);
	vbo_init(&b);
	vbo_load_data(&a);
	vbo_load_data(&b);

	vao_bind(&arra);
	vbo_bind(&a);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	vbo_unbind(&a);
	vao_unbind(&arra);
	glDisableVertexAttribArray(0);

	vao_bind(&arrb);
	vbo_bind(&b);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	vbo_unbind(&b);
	vao_unbind(&arrb);
	glDisableVertexAttribArray(0);

	while (glfwWindowShouldClose(win) != true) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(prg -> id);
		vao_bind(&arra);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		vao_unbind(&arra);

		glUseProgram(prg -> id);
		vao_bind(&arrb);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		vao_unbind(&arrb);


		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	vao_destroy(&arra);
	vao_destroy(&arrb);
	vbo_destroy(&a);
	vbo_destroy(&b);

defer:
	if (vert != NULL) {
		destroy_shader(vert);
	}

	if (frag != NULL) {
		destroy_shader(frag);
	}

	if (prg != NULL) {
		destroy_program(prg);
	}

	if (win != NULL) {
		glfwDestroyWindow(win);
	}
	return 0;
}
