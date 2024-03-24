#ifndef GLDE_IBO_H
#define GLDE_IBO_H

#include <stddef.h>
#include "../include/glad/glad.h"

typedef struct {
	unsigned int id;
	unsigned int *indices;
	size_t size;
} IBO;

static inline void ibo_init(IBO *ibo);
static inline void ibo_bind(IBO *ibo);
static inline void ibo_unbind(IBO *ibo);
static inline void ibo_load_data(IBO *ibo);
static inline void ibo_destroy(IBO *ibo);

// TODO: Uncomment this after writing the header
#define GLDE_IBO_IMPL

#ifdef GLDE_IBO_IMPL

static inline void ibo_init(IBO *ibo) {
	glGenBuffers(1, &(ibo -> id));
}

static inline void ibo_bind(IBO *ibo) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo -> id);
}

static inline void ibo_unbind(IBO *ibo) {
	(void) ibo;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static inline void ibo_load_data(IBO *ibo) {
	ibo_bind(ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo -> size, ibo -> indices, GL_STATIC_DRAW);
	ibo_unbind(ibo);
}

static inline void ibo_destroy(IBO *ibo) {
	glDeleteBuffers(1, &(ibo -> id));
}

#endif // GLDE_IBO_IMPL

#endif // GLDE_IBO_H
