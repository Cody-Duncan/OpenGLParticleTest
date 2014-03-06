#version 330

uniform mat4 MVP;

in vec3 position;
 
void main() 
{ 
	vec3 pos = position;
	pos.x = pos.x;
	pos.y = pos.y;
    gl_Position = vec4(pos,1.0);
}