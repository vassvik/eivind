#version 330 core

flat in int instance;
in vec2 uv;
in vec3 spos;
in vec3 pos;
in vec3 normal;
in float radius;

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
		if (length(uv) < 1.0) {
			float z = sqrt(1.0 - dot(uv, uv));
			vec3 n = normalize(vec3(uv, z));

			float s = dot(vec3(0.0, 0.0, 1.0), n);

			color = vec4(col, 1.0)*s;

			float d = length((spos + radius*(r*uv.x + u*uv.y - f*z)) - cpos);

			gl_FragDepth = (d - near)/(far - near);
		} else {
			//discard;
		}
	} else {
		float s = clamp(dot(vec3(1.0, 0.0, 0.0), normal), 0.0, 1.0)*0.5 + 0.5;
		vec3 col = pos.x > 0.0 ? vec3(1.0, 0.0, 0.0) : vec3(0.0, 1.0, 0.0);
		color = vec4(col*s, 1.0);

		float d = length(pos - cpos);

		gl_FragDepth = (d - near)/(far - near);
	}
	
}