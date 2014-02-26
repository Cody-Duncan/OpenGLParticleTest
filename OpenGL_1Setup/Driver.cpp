#include <string>       //string
#include <stdio.h>      //cout, endl
#include <windows.h>    //windows header
#include <fstream>      //file stream
#include <limits>       //numeric limits
#include <crtdbg.h>     //memory leak debugging

#include <glload/gl_all.h>
#include <glload\gl_load.hpp>
#include <GL/freeglut.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "FrameBufferObject.h"
#include "Shaders.h"

int ScreenWidth = 800;
int ScreenHeight = 600;

int WindowHandle;

void Draw();
void ResizeWindow(int newWidth, int newHeight);
void KeyboardDown(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void MouseButton(int button, int state, int x, int y);
void MouseMove(int x, int y);

void Initialize();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion (3, 3);                      //opengl version
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); //set for compatibility mode

    glutInitWindowSize(ScreenWidth, ScreenHeight);

    //sets the initial display mode
    // double buffered
    // RGBA mode window
    // depth buffer mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    //CREATE THE WINDOW
    WindowHandle = glutCreateWindow("OpenGL setup Test");
    if(WindowHandle < 1) 
    {
        fprintf(stderr,"ERROR: Could not create a new rendering window.\n");
        exit(EXIT_FAILURE);
    }
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    //Load implementation of openGL functions automatically
    //Alternatives to glLoad are documented here:http://www.opengl.org/wiki/OpenGL_Loading_Library
    // GLEW, GL3W, OpenGL Loader Generator, GlLoad
    glload::LoadFunctions();

    Initialize();
    
    //hook function bindings
    glutDisplayFunc(&Draw);
    glutReshapeFunc(&ResizeWindow);

    glutKeyboardFunc(&KeyboardDown);
    glutKeyboardUpFunc(&KeyboardUp);

    glutMouseFunc(&MouseButton);
    glutMotionFunc(&MouseMove);

    glutMainLoop();

    return 0;
}

GLuint vertexShader;
GLuint pixelShader;
GLuint baseShader;
GLint MVP_Matrix_ID;

glm::mat4 viewProj;

void Initialize()
{
    //set clear color and enable features
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //Create Shader
    vertexShader = CreateShader("vert.glsl", GL_VERTEX_SHADER);
    pixelShader = CreateShader("pixe.glsl", GL_FRAGMENT_SHADER);

    GLuint baseShader = glCreateProgram();
    glBindAttribLocation(baseShader, 0, "position");
    glBindAttribLocation(baseShader, 1, "color");
    LinkProgram(baseShader, vertexShader, pixelShader);
    glUseProgram(baseShader);
    
    MVP_Matrix_ID = glGetUniformLocation(baseShader, "MVP");


    //World, View, Projection matrices
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 2, -3),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );
    glm::mat4 projection = glm::perspective(
        75.0f,        //FOV
        4.0f / 3.0f,  //aspect ratio
        0.1f,         //near plane
        100.0f        //far plane
    );
    viewProj =  projection * view;
}

void Draw()
{
    glm::mat4 world;;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(baseShader);

    glPushMatrix();
                
        //update matrix in shader
        glUniformMatrix4fv(MVP_Matrix_ID, 1, GL_FALSE, &(viewProj * world)[0][0]);

        //draw cube
        glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glPopMatrix();
            
    glFlush();
    glutSwapBuffers();
}

void ResizeWindow(int newWidth, int newHeight)
{
    
}

void KeyboardDown(unsigned char key, int x, int y)
{
    
}

void KeyboardUp(unsigned char key, int x, int y)
{
    
}

void MouseButton(int button, int state, int x, int y)
{
    
}

void MouseMove(int x, int y)
{
    
}
