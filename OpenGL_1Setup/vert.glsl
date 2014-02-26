#version 330

uniform mat4 MVP;

in vec3 position;
in vec3 colorValue;
 
out vec3 colorV;
 
void main() 
{ 
    colorV = colorValue;
    gl_Position = MVP * vec4(position,1.0);
}