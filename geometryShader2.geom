#version 330 core
layout (points) in;
layout(triangle_strip, max_vertices = 32) out;

in VS_OUT {
	float pLength;
	float pAngle;
} gs_in[];

void main() {
	vec3 pos = gl_in[0].gl_Position.xyz;
	float pi = 3.14159;
	float l = gs_in[0].pLength;
	float t = gs_in[0].pAngle; 
	float r = 0.02;
	vec4 bob = vec4(pos + vec3(l * sin(t), -l * cos(t), 0.0f), 1.0f);
	for(int i = 0; i < 15; i++) {
		float theta = (pi / 180.0f) * 12 * i;
		float x = r * cos(theta);
		float y = r * sin(theta);
		gl_Position = bob + vec4(x, y, 0.0f, 0.0f);
		EmitVertex();		
		gl_Position = bob + vec4(x, -y, 0.0f, 0.0f);
		EmitVertex();
	}
	gl_Position = bob + (r * vec4(1, 0, 0.0f, 0.0f));
	EmitVertex();
	EndPrimitive();
}	