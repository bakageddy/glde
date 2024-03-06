#ifndef __SHADER_H
#define __SHADER_H

#include <stddef.h>

typedef struct {
	unsigned int id;
	const char *source;
} VertexShader;

VertexShader vertex_shader_create(void) {
	return (VertexShader) {.id = 0, .source = NULL};
}

void vertex_shader_set_source(VertexShader* vs, const char *source) {
	if (!vs) return;
	vs -> source = source;
}

#endif//__SHADER_H
