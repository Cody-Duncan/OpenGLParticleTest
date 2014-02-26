#version 330 core

out float textureSample;

void main(void)
{
	textureSample = gl_VertexID / 512.0; 
}