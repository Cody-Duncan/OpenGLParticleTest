#version 330

uniform sampler2D texture0;
out vec4 colorOut;
 
void main() 
{
	colorOut = texture(texture0, gl_PointCoord);   
}