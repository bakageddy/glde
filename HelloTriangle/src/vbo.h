#ifndef __VBO_H__
#define __VBO_H__

#include <stddef.h>
#include "../include/glad/glad.h"

typedef struct {
	GLuint id;
	void *vs;
	size_t size;
} VBO;

static inline void VBO_Init(VBO *vbo) {
	glGenBuffers(1, &(vbo -> id));
}

static inline void VBO_Bind(VBO *vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo -> id);
	glBufferData(GL_ARRAY_BUFFER, vbo -> size, vbo -> vs, GL_STATIC_DRAW);
}

static inline void VBO_Unbind(VBO *vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif // !__VBO_H__

