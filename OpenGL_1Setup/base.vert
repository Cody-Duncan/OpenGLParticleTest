#version 330

uniform mat4 MVP;
uniform sampler2D texture0;
uniform int textureSize;
uniform float inputVal;

in vec4 position;
out vec4 color;
 
void main() 
{ 
    float id = gl_VertexID;

    float v = (id / textureSize)/textureSize;
    float u = mod(gl_VertexID, textureSize)/textureSize;
    vec2 uv = vec2(u,v);

    gl_Position = MVP * texture2D(texture0, uv);
    gl_Position.x = gl_Position.x + 30*sin(inputVal)*cos(id/2)*0.3;
    gl_Position.y = gl_Position.y + 30*sin(inputVal);
    //gl_Position.x = gl_Position.x*20*sin(inputVal)-10;
    //gl_Position.y = gl_Position.y*70-60;
    
    color = texture2D(texture0, uv);
}