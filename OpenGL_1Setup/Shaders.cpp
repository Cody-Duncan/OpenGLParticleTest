#include "Shaders.h"

#include <fstream>
#include <vector>

using namespace std;

char* ReadFile(const char* name)
{
    std::ifstream f;
    f.open(name, std::ios_base::binary); 
    f.seekg(0, std::ios_base::end);      
    int length = (int)f.tellg();              
    f.seekg (0, std::ios_base::beg);   

    char* content = new char [length+1]; 
    f.read (content, length);            
    f.close();                           

    content[length] = 0;
    return content;
}

GLint CreateShader(const char * filename, GLenum type)
{
    //get source
    char* shaderSource = ReadFile(filename);

    /////////////// compile shader ///////////////
    GLuint shader = glCreateShader(type);
    glShaderSource(shader,1, &shaderSource, NULL); 
    glCompileShader(shader);
    delete shaderSource;

    //check if it compiled
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status > 1) 
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length];
        glGetShaderInfoLog(shader, length, NULL, buffer);
        printf("Compile log for %s:\n%s\n", filename, buffer);

        delete buffer;
    }

    return shader;
}

void LinkProgram(GLuint program, GLuint vertexShader, GLuint pixelShader)
{   
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);

    /////////////// link the program ///////////////
    glLinkProgram(program);

    //check if it linked
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != 1) 
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length];
        glGetProgramInfoLog(program, length, NULL, buffer);
        printf("Link log:\n%s\n", buffer);

        delete buffer;
    }
}


void printActiveUniforms(GLuint baseShader)
{
    int numActiveAttribs;
    glGetProgramInterfaceiv(baseShader, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveAttribs);
    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);
    std::vector<GLint> values(properties.size());
    for(int attrib = 0; attrib < numActiveAttribs; ++attrib)
    {
        glGetProgramResourceiv(baseShader, GL_UNIFORM, attrib, properties.size(),
        &properties[0], values.size(), NULL, &values[0]);
            
        nameData.resize(properties[0]); //The length of the name.
        glGetProgramResourceName(baseShader, GL_UNIFORM, attrib, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);
    }
}