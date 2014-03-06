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

#define TEX_SIZE 64

int ScreenWidth = 800;
int ScreenHeight = 600;

int WindowHandle;

FrameBufferObject frame1;

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
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

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
GLuint textureShader;
GLuint processShader;
GLint MVP_Matrix_ID;

glm::mat4 viewProj;

GLuint vertex_AO;
GLuint vertexBuffer;
static const GLfloat g_vertex_buffer_data[] = {
    -0.5f, 0.0f, 0.0f, 
    -0.5f, 1.0f, 0.0f,
    10.0f, 0.0f, 0.0f, 
};

GLuint singleV_AO;
GLuint singleVBuffer;
static const GLfloat singleVBufferData[] = 
{
    0.0f, 0.0f, 0.0f, 
};

void Initialize()
{
    //set clear color and enable features
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    

    //Create Base Shader
    vertexShader = CreateShader("base.vert", GL_VERTEX_SHADER);
    pixelShader = CreateShader("base.frag", GL_FRAGMENT_SHADER);

    baseShader = glCreateProgram();
    glBindAttribLocation(baseShader, 0, "position");
    LinkProgram(baseShader, vertexShader, pixelShader);

    MVP_Matrix_ID = glGetUniformLocation(baseShader, "MVP");


	//Create Base Shader
    vertexShader = CreateShader("process.vert", GL_VERTEX_SHADER);
    pixelShader = CreateShader("process.frag", GL_FRAGMENT_SHADER);

	processShader = glCreateProgram();
    glBindAttribLocation(processShader, 0, "position");
    LinkProgram(processShader, vertexShader, pixelShader);


    //Create Texture Shader
    vertexShader = CreateShader("texture.vert", GL_VERTEX_SHADER);
    pixelShader = CreateShader("texture.frag", GL_FRAGMENT_SHADER);
    textureShader = glCreateProgram();
    glBindAttribLocation(textureShader, 0, "position");
    LinkProgram(textureShader, vertexShader, pixelShader);

    printActiveUniforms(baseShader);
    
    //World, View, Projection matrices
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 0, 7),
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

    frame1.CreateFBO(TEX_SIZE, TEX_SIZE);

	//create/bind a Vertex Array Object (VAO)
	glGenVertexArrays(1, &vertex_AO);
	glBindVertexArray(vertex_AO);

	//fill VAO
	glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //bind the vertex buffer to attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                                // attribute. 0 must match layout in shader
        3,                                // 3 floats per vertex
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );


	glGenVertexArrays(1, &singleV_AO);
	glBindVertexArray(singleV_AO);

    //create a SINGLE VERTEX BUFFER
    glGenBuffers(1, &singleVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, singleVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(singleVBufferData), singleVBufferData, GL_STATIC_DRAW);

    //bind the vertex buffer to attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                                // attribute. 0 must match layout in shader
        3,                                // 3 floats per vertex
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );
}

void Draw()
{
    static float angle = 0.5;
    angle += 0.5;
    glm::mat4 world; //reset matrix
    world = glm::rotate(world, angle, glm::vec3(0,1,0));
    glm::mat4 result = viewProj * world;
    
	glEnable(GL_POINT_SPRITE);

    //--------------processing pass------------------
    
	glUseProgram(processShader);
    frame1.Bind();

    glViewport(0, 0, TEX_SIZE, TEX_SIZE);
    glClearColor( 0.0f, 1.0f, 0.0f, 1.0f ); //green texture
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(40);

        //update matrix in shader
        glBindVertexArray(singleV_AO);
        glDrawArrays(GL_POINTS, 0, 1);
        
    frame1.Unbind();
    glUseProgram(0);
    
	
    glViewport(0,0,ScreenWidth, ScreenHeight);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
    //----------------texture draw--------------------

    glUseProgram(textureShader);

		glEnable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frame1.texture);
		glUniform1i(glGetUniformLocation(textureShader, "texture0"), 0); //bind texture 0 to GL_TEXTURE0
		

        //draw fbo texture
        glBindVertexArray(singleV_AO);
        glDrawArrays(GL_POINTS, 0, 1);

    glUseProgram(0);
    

    //--------------- point draw ------------------
    glUseProgram(baseShader);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        //draw 3 points
        glUniformMatrix4fv(MVP_Matrix_ID, 1, GL_FALSE, &(result[0][0]));
		glBindVertexArray(vertex_AO);
        glDrawArrays(GL_POINTS, 0, 3);
        
    glUseProgram(0);
	
    glutSwapBuffers();

    glutPostRedisplay();
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
