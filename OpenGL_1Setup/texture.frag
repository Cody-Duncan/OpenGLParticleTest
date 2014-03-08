#version 330

uniform sampler2D texture0;
in vec2 TexCoordinate;
out vec4 colorOut;
 
void main() 
{
	colorOut = texture(texture0, TexCoordinate);   
}