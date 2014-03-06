#version 330

out vec4 colorOut;
 
void main() 
{
	colorOut = vec4(sin(gl_PointCoord.x), gl_PointCoord.y, 1, 0);   
}