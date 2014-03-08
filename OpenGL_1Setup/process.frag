#version 330

in vec2 uvCoord;
out vec4 colorOut;
 
void main() 
{
	colorOut = vec4( uvCoord.x , uvCoord.y, 1, 1);   
}