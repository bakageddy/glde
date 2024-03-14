#ifndef GLDE_SHADER_H
#define GLDE_SHADER_H

#include "../include/glad/glad.h"
#include "utils.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	VERTEX,
	FRAGMENT,
} ShaderType;

typedef struct {
	GLuint id;
	const char *src;
	const char *src_path;
	ShaderType type;
} Shader;

typedef struct {
	GLuint id;
} ShaderProgram;

inline static char *get_shader_source(const char *path);
inline static Shader *load_shader_from_file(const char *src_path, ShaderType type);
inline static int compile_shader(Shader *sh);
inline static char* get_compile_log(Shader *sh, char *buf, size_t size);
inline static void destroy_shader(Shader *sh);

inline static ShaderProgram* create_shader_program(Shader *vertex, Shader *fragment);
inline static int link_shader_program(ShaderProgram *p);
inline static char* get_link_log(ShaderProgram *p, char *buf, size_t size);
inline static void destroy_program(ShaderProgram *p);

#define GLDE_SHADER_IMPL // Debug
#ifdef GLDE_SHADER_IMPL
inline static char *get_shader_source(const char *path) {
	int handle = open(path, O_RDONLY);
	if (handle == -1) {
		LOG("Failed to open file: %s", strerror(errno));
		return NULL;
	}
	struct stat info;
	fstat(handle, &info);
	int size = info.st_size;
	char *file_contents = (char *) malloc((size + 1) * sizeof(char));

	if (!file_contents) return NULL;
	if (read(handle, file_contents, size + 1) == -1) {
		LOG("Failed to read file: %s", strerror(errno));
		return NULL;
	}
	file_contents[size] = '\0';
	close(handle);
	return file_contents;
}

inline static Shader *load_shader_from_file(const char *src_path, ShaderType type) {
	Shader *s = (Shader *) malloc(sizeof(Shader));
	if (s == NULL) return NULL;
	// id = 0, means it's uninitialized
	s -> id = 0;
	s -> type = type;
	s -> src_path = src_path;
	s -> src = get_shader_source(src_path);
	if (s -> src == NULL) return NULL;
	return s;
}

inline static int compile_shader(Shader *sh) {
	if (sh == NULL || sh -> src == NULL) return 1;
	if (sh -> type == VERTEX)
		sh -> id = glCreateShader(GL_VERTEX_SHADER);
	else 
		sh -> id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sh -> id, 1, &sh -> src, NULL);
	glCompileShader(sh -> id);
	GLint compiled;
	glGetShaderiv(sh -> id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_TRUE) return 0;
	else return 1;
}

inline static char* get_compile_log(Shader *sh, char *buf, size_t size) {
	if (buf == NULL || sh == NULL || size <= 0) return NULL;
	glGetShaderInfoLog(sh -> id, size, NULL, buf);
	return buf;
}

inline static void destroy_shader(Shader *sh) {
	if (sh == NULL || sh -> src == NULL) return;
	free((void*) sh -> src);
	free(sh);
}

inline static ShaderProgram* create_shader_program(Shader *vertex, Shader *fragment) {
	if (vertex -> type != VERTEX || fragment -> type != FRAGMENT) {
		ERR("Need Vertex Shader and Fragment Shader");
		return NULL;
	}
	ShaderProgram *prog = (ShaderProgram *) malloc(sizeof(ShaderProgram));
	if (prog == NULL) return NULL;
	prog -> id = glCreateProgram();
	glAttachShader(prog -> id, vertex -> id);
	glAttachShader(prog -> id, fragment -> id);
	return prog;
}

inline static char *get_link_log(ShaderProgram *p, char *buf, size_t size) {
	if (buf == NULL || p == NULL || size <= 0) return NULL;
	glGetProgramInfoLog(p -> id, size, NULL, buf);
	return buf;
}

// 0 -> Successful Link
// 1 -> Error in Linking
inline static int link_shader_program(ShaderProgram *p) {
	if (p == NULL) return 1;
	glLinkProgram(p -> id);
	GLint result;
	glGetProgramiv(p -> id, GL_LINK_STATUS, &result);
	if (result == GL_TRUE) return 0;
	else return 1;
}

inline static void destroy_program(ShaderProgram *p) {
	if (p == NULL) return;
	glDeleteProgram(p -> id);
	free(p);
}

#endif // GLDE_SHADER_IMPL

#endif // GLDE_SHADER_H
