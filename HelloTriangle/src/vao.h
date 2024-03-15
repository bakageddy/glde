#ifndef GLDE_VAO_H
#define GLDE_VAO_H

#include <stddef.h>
#include "../include/glad/glad.h"

typedef struct {
	GLuint id;
} VAO;


static inline void vao_init(VAO *v);
static inline void vao_bind(VAO *v);
static inline void vao_unbind(VAO *v);
static inline void vao_destroy(VAO *v);

#define GLDE_VAO_IMPL // Debug
#ifdef GLDE_VAO_IMPL

static inline void vao_init(VAO *v) {
	glGenVertexArrays(1, &v -> id);
}

static inline void vao_bind(VAO *v) {
	glBindVertexArray(v -> id);
}

static inline void vao_unbind(VAO *v) {
	glBindVertexArray(0);
}

static inline void vao_destroy(VAO *v) {
	glDeleteVertexArrays(1, &v -> id);
}

#endif // GLDE_VAO_IMPL
#endif // !GLDE_VAO_H
