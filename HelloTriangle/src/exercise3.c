#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "utils.h"
#include "shader.h"
#include "vbo.h"
#include "vao.h"

static int width = 800;
static int height = 600;

static float white_vs[] = {
	0.5f, 0.5f,
	0.0f, 0.0f,
	0.0f, 0.5f,
};

static float yellow_vs[] = {
	-0.5f, -0.5f,
	0.0f, 0.0f,
	0.0f, -0.5f,
};

GLFWwindow *glde_create_window(int w, int h, const char *title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *win = glfwCreateWindow(w, h, title, NULL, NULL);
	return win;
}

int glde_init_glad(void) {
	int res = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	return res;
}

void glde_handle_resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(void) {
	glfwInit();
	GLFWwindow *window = glde_create_window(width, height, "Exercise 3");
	if (window == NULL) {
		ERR("Failed to create window\n");
		goto defer;
	}

	glfwMakeContextCurrent(window);
	if (glde_init_glad() == 0) {
		ERR("Failed to initialize glad\n");
		goto defer;
	}

	glViewport(0, 0, width, height);
	glfwSetWindowSizeCallback(window, glde_handle_resize);

	Shader *white_vertex = load_shader_from_file("./shaders/triangle.vertex.glsl", VERTEX);
	if (white_vertex == NULL) {
		ERR("Failed to load vertex shader from file");
		goto defer;
	}

	Shader *yellow_vertex = load_shader_from_file("./shaders/exercise1.vertex.glsl", VERTEX);
	if (yellow_vertex == NULL) {
		ERR("Failed to load vertex shader from file");
		goto defer;
	}

	Shader *white_frag = load_shader_from_file("./shaders/triangle.frag.glsl", FRAGMENT);
	if (white_frag == NULL) {
		ERR("Failed to load white fragment shader from file");
		goto defer;
	}

	Shader *yellow_frag = load_shader_from_file("./shaders/exercise1.frag.glsl", FRAGMENT);
	if (yellow_frag == NULL) {
		ERR("Failed to load yellow fragment shader from file");
		goto defer;
	}

	if (compile_shader(white_vertex) != 0 
			|| compile_shader(yellow_vertex) != 0
			|| compile_shader(white_frag) != 0 
			|| compile_shader(yellow_frag) != 0) {
		ERR("Failed to compile shader(s)");
		goto defer;
	} 

	ShaderProgram *white_prg = create_shader_program(white_vertex, white_frag);
	if (white_prg == NULL) {
		ERR("Failed to allocate memory for shader program");
		goto defer;
	}

	ShaderProgram *yellow_prg = create_shader_program(yellow_vertex, yellow_frag);
	if (yellow_prg == NULL) {
		ERR("Failed to allocate memory for yellow shader program");
		goto defer;
	}

	if (link_shader_program(white_prg) != 0
			|| link_shader_program(yellow_prg) != 0) {
		ERR("Failed to link shaders");
		goto defer;
	}

	VBO whites = (VBO) {
		.id = 0,
		.vs = white_vs,
		.size = sizeof(white_vs),
	};

	VAO vwhites = (VAO) {
		.id = 0,
	};

	VBO yellows = (VBO) {
		.id = 0,
		.vs = yellow_vs,
		.size = sizeof(yellow_vs),
	};

	VAO vyellows = (VAO) {
		.id = 0,
	};

	vbo_init(&whites);
	vbo_init(&yellows);

	vbo_load_data(&whites);
	vbo_load_data(&yellows);

	vao_init(&vwhites);
	vao_init(&vyellows);

	vao_bind(&vwhites);
	vbo_bind(&whites);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	vbo_unbind(&whites);
	vao_unbind(&vwhites);

	vao_bind(&vyellows);
	vbo_bind(&yellows);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	vbo_unbind(&yellows);
	vao_unbind(&vyellows);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(white_prg -> id);
		vao_bind(&vwhites);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(yellow_prg -> id);
		vao_bind(&vyellows);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vbo_destroy(&whites);
	vbo_destroy(&yellows);

defer:
	if (white_vertex != NULL) {
		destroy_shader(white_vertex);
	}

	if (yellow_vertex != NULL) {
		destroy_shader(yellow_vertex);
	}

	if (white_frag != NULL) {
		destroy_shader(white_frag);
	}

	if (yellow_frag != NULL) {
		destroy_shader(yellow_frag);
	}

	if (white_prg != NULL) {
		destroy_program(white_prg);
	}

	if (yellow_prg != NULL) {
		destroy_program(yellow_prg);
	}

	if (window != NULL) {
		glfwDestroyWindow(window);
	}

	glfwTerminate();
	return 0;
}
