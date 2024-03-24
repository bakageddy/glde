#version 330 core

layout (location = 0) in vec3 pos;

uniform float angle;
uniform float in_time;
uniform float in_resolution_x;
uniform float in_resolution_y;

out vec3 vertex_output;
out float time;
out float resolution_x;
out float resolution_y;

void main() {
	float shift = radians(angle);
	vec3 transform;
	transform.x = (pos.x * cos(shift)) - (pos.y * sin(shift));
	transform.y = (pos.x * sin(shift)) - (pos.y * cos(shift));
	transform.z = 0;
	transform.x *= 1.5;
	transform.y *= 1.5;
	gl_Position = vec4(transform, 1.0f);
	vertex_output = transform;
	time = in_time;
	resolution_x = in_resolution_x;
	resolution_y = in_resolution_y;
}
