#ifndef GLDE_VBO_H
#define GLDE_VBO_H

#include <stddef.h>
#include "../include/glad/glad.h"

typedef struct {
	GLuint id;
	void *vs;
	size_t size;
} VBO;

static inline void vbo_init(VBO *vbo);
static inline void vbo_bind(VBO *vbo);
static inline void vbo_unbind(VBO *vbo);
static inline void vbo_load_data(VBO *vbo);
static inline void vbo_destroy(VBO *vbo);

#define GLDE_VBO_IMPL // Debug
#ifdef GLDE_VBO_IMPL

static inline void vbo_init(VBO *vbo) {
	glGenBuffers(1, &(vbo -> id));
}

static inline void vbo_bind(VBO *vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo -> id);
}

static inline void vbo_unbind(VBO *vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static inline void vbo_load_data(VBO *vbo) {
	vbo_bind(vbo);
	glBufferData(GL_ARRAY_BUFFER, vbo -> size, vbo -> vs, GL_STATIC_DRAW);
	vbo_unbind(vbo);
}

static inline void vbo_destroy(VBO *vbo) {
	glDeleteBuffers(1, &(vbo -> id));
}

#endif // GLDE_VBO_IMPL
#endif // !GLDE_VBO_H

