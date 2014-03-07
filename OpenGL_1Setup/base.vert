#version 330

uniform mat4 MVP;

in vec4 position;
 
void main() 
{ 
    gl_Position = MVP * position;
}