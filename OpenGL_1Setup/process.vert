#version 330

uniform mat4 MVP;

in vec3 position;
in vec2 uv;

out vec2 uvCoord;
 
void main() 
{ 
	vec3 pos = position;
    gl_Position = vec4(pos,1.0);

    uvCoord = uv;
}