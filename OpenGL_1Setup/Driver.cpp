#include <string>       //string
#include <stdio.h>      //cout, endl
#include <windows.h>    //windows header
#include <fstream>      //file stream
#include <limits>       //numeric limits
#include <crtdbg.h>     //memory leak debugging

#include <GL/freeglut.h>
#include <glload\gl_load.hpp>

int ScreenWidth = 800;
int ScreenHeight = 600;

int WindowHandle;

void Draw();
void ResizeWindow(int newWidth, int newHeight);
void KeyboardDown(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void MouseButton(int button, int state, int x, int y);
void MouseMove(int x, int y);

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

void Draw()
{
    
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
