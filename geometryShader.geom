#version 330 core
layout (points) in;
layout(line_strip, max_vertices = 2) out;

in VS_OUT {
	float pLength;
	float pAngle;
} gs_in[];

void main() {
	vec3 pos = gl_in[0].gl_Position.xyz;
	float l = gs_in[0].pLength;
	float t = gs_in[0].pAngle; 
	gl_Position = vec4(pos, 1.0f);
	EmitVertex();
	vec3 bob = pos + vec3(l * sin(t), -l * cos(t), 0.0f);
	gl_Position = vec4(bob, 1.0f);
	EmitVertex();
	EndPrimitive();
}	