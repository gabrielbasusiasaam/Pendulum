#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 control;
out VS_OUT {
	float pLength;
	float pAngle;
} vs_out;


void main()
{
	gl_Position = vec4(pos, 1.0f);
	vs_out.pLength = control[0];
	vs_out.pAngle = control[1];
}