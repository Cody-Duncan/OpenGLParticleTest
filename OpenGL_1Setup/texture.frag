#version 330

uniform sampler2D texture0;
out vec4 colorOut;
 
void main() 
{
	colorOut = texture(texture0, vec2(0.5,0.5) );   
}