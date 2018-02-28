#version 330 core

uniform mat4 MVP;

uniform vec3 r;
uniform vec3 u;

uniform int draw_mode;

out vec3 pos;
out vec2 uv;
out vec3 spos;
out vec3 normal;
flat out int instance;

void main() {
	if (draw_mode == 0) {
		vec2 p = vec2(gl_VertexID%2, gl_VertexID/2);
		p = 2.0*p - 1.0;

		if (gl_InstanceID == 0) {
			spos = vec3(-1.0, 0.0, 0.0);
		} else {
			spos = vec3(1.0, 0.0, 0.0);
		}
		vec3 P = spos + r*p.x + u*p.y;

		uv = p;
		instance = gl_InstanceID;

	    gl_Position = MVP*vec4(P, 1.0);
	} else {
		float y = gl_VertexID % 2;
		float x = (gl_VertexID / 2)/15.0;

		float r = 0.1;
		vec3 p = vec3(r*cos(2.0*3.1416*x), r*sin(2.0*3.1416*x), -1.0 + 2.0*y);
		p = p.zxy;
		pos = p;

		normal = normalize(vec3(p.y, p.z, 0.0));


		gl_Position = MVP*vec4(p, 1.0);
	}
}
