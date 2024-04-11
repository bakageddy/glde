#version 330 core

in vec3 vertex_output;
in float time;
in float resolution_x;
in float resolution_y;
out vec4 frag_color;

vec3 palette(float t) {
	vec3 a = vec3(0.5, 0.5, 0.5);
	vec3 b = vec3(0.5, 0.5, 0.5);
	vec3 c = vec3(1.0, 1.0, 1.0);
	vec3 d = vec3(0.22, 0.44, 0.55);
	return a + b * cos(6.28318 * (c * t + d));
}

void main() {
	vec2 res = vec2(resolution_x, resolution_y);
	vec2 coord = vec2(vertex_output.x, vertex_output.y);
	vec2 uv = (coord.xy) * (res.xy / res.y);
	float d = length(uv);
	vec3 color = vec3(1.0);
	d = sin(d * 8 + time) / 8.0;
	d = abs(d);
	d = 0.01/d;
	color *= d;
	frag_color = vec4(color, 1.0f);
}
