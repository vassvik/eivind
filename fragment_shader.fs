#version 330 core

flat in int instance;
in vec2 uv;
in vec3 spos;
in vec3 pos;
in vec3 normal;

uniform vec3 f;
uniform vec3 r;
uniform vec3 u;
uniform vec3 cpos;
uniform float near;
uniform float far;

uniform int draw_mode;

out vec4 color;

void main() {
	if (draw_mode == 0) {

		vec3 col;
		if (instance == 0) {
			col = vec3(1.0, 0.0, 0.0);
		} else {
			col = vec3(0.0, 1.0, 0.0);
		}
		if (length(uv) < 0.5) {
			float z = sqrt(0.5*0.5 - dot(uv, uv));
			vec3 n = normalize(vec3(uv, z));

			float s = dot(vec3(0.0, 0.0, 1.0), n);

			color = vec4(col, 1.0);

			float d = length((spos + (0.5*r*uv.x + 0.5*u*uv.y - f*z)) - cpos);

			gl_FragDepth = (d - near)/(far - near);
		} else {
			discard;
		}
	} else {
		float s = dot(vec3(1.0, 0.0, 0.0), normal);
		color = vec4(vec3(0.0, 0.0, 1.0)*0.5 + 0.5 + 0.5*normal, 1.0);

		float d = length(pos - cpos);

		gl_FragDepth = (d - near)/(far - near);
	}
	
}