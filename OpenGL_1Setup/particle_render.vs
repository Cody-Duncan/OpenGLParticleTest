#version 330 core

uniform mat4 u_worldMat;
uniform mat4 u_viewProjMat;
out float accessFloat;

void main (void)
{
	mat4 finalMatrix = (u_viewProjMat * u_worldMat);
	gl_Position = finalMatrix * vec4(0,0,0,1);
	accessFloat = gl_VertexID / 512.0;
}