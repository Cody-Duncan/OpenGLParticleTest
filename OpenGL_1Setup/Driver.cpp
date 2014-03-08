/*

Notes

Try rendering to PixelBufferObject, then rebinding as a VertexBufferObject. 
See: http://devmaster.net/posts/11093/render-to-vertexbuffer-in-opengl-howto
*/

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

#include <random>

static const int textureWidth = 4096;
static const int textureHeight = textureWidth;

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

    int MaxVertexTextureImageUnits;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &MaxVertexTextureImageUnits);
    printf("MAX VERTEX TEXTURE IMAGE UNITS: %d\n", MaxVertexTextureImageUnits);

    glutMainLoop();

    return 0;
}

GLuint vertexShader;
GLuint pixelShader;
GLuint baseShader;
GLuint textureShader;
GLuint processShader;
GLint MVP_Matrix_ID;
GLint inputVal_ID;

glm::mat4 viewProj;

GLuint single_AO;
GLuint singleBuffer;
static const GLfloat singleBufferData[] = {
    0,0,0
};

GLuint vertex_AO;
GLuint vertexBuffer;
static const GLfloat g_vertex_buffer_data[] = {
    -0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 1.0f, 0.0f, 1.0f,
    10.0f, 0.0f, 0.0f, 1.0f,
};

GLuint quad_AO;
GLuint quadBuffer;
GLuint quadBufferUV;
static const GLfloat quadBufferData[] = 
{
   -1, -1, 0.0f,
    1, -1, 0.0f,
    1,  1, 0.0f,
   -1, -1, 0.0f,
    1,  1, 0.0f,
   -1,  1, 0.0f,
};

GLuint upleftQuad_AO;
GLuint upleftQuadBuffer;
static const GLfloat upleftQuadBufferData[] = 
{
   -0.95f, 0.10f, 0.0f,
   -0.30f, 0.10f, 0.0f,
   -0.30f, 0.85f, 0.0f,
   -0.95f, 0.10f, 0.0f,
   -0.30f, 0.85f, 0.0f,
   -0.95f, 0.85f, 0.0f,
};

GLuint upleftQuadBufferUV;
static const GLfloat upleftQuadBufferUVData[] = 
{
    0,0,
    1,0,
    1,1,
    0,0,
    1,1,
    0,1,
};

GLuint randomTexture;

void createSingleArray()
{
	glGenVertexArrays(1, &single_AO);
	glBindVertexArray(single_AO);

    //create a SINGLE VERTEX BUFFER
    glGenBuffers(1, &singleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, singleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(singleBufferData), singleBufferData, GL_STATIC_DRAW);

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

void createVertexArray()
{
    //create/bind a Vertex Array Object (VAO)
	glGenVertexArrays(1, &vertex_AO);
	glBindVertexArray(vertex_AO);

	//fill VAO
    //NOTE: buffer size must be same size as input texture data from FBO.
	glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    int numVerts = 20;
    int sizeBuffer = numVerts * 4 * sizeof(float);
    

    std::random_device rd;
    std::mt19937 e(rd());
    std::uniform_real_distribution<float> uniform_dist(0,1);

    

    float* startBuffer = new float[numVerts*4];
    for(int i = 0; i < numVerts*4; ++i)
    {
        startBuffer[i] = uniform_dist(e);
    }

    glBufferData(GL_ARRAY_BUFFER, sizeBuffer, startBuffer, GL_STATIC_DRAW);

    //bind the vertex buffer to attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                                // attribute. 0 must match layout in shader
        4,                                // 4 floats per vertex
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );
}


void createQuadArray()
{
	glGenVertexArrays(1, &quad_AO);
	glBindVertexArray(quad_AO);

    //create a SINGLE VERTEX BUFFER
    glGenBuffers(1, &quadBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadBufferData), quadBufferData, GL_STATIC_DRAW);

    int POS_index = glGetAttribLocation(processShader, "position");
    int UV_index = glGetAttribLocation(processShader, "uv");

    //bind the vertex buffer to attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        POS_index,                                // attribute. 0 must match layout in shader
        3,                                // 3 floats per vertex
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );

    
    glGenBuffers(1, &quadBufferUV);
    glBindBuffer(GL_ARRAY_BUFFER, quadBufferUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(upleftQuadBufferUVData), upleftQuadBufferUVData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        UV_index,                                // attribute. 1 must match layout in shader
        2,                                // 2 floats per set of UV
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );
}

void createULQuadArray()
{
	glGenVertexArrays(1, &upleftQuad_AO);
	glBindVertexArray(upleftQuad_AO);

    //create a SINGLE VERTEX BUFFER
    glGenBuffers(1, &upleftQuadBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, upleftQuadBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(upleftQuadBufferData), upleftQuadBufferData, GL_STATIC_DRAW);

    int POS_index = glGetAttribLocation(textureShader, "position");
    int UV_index = glGetAttribLocation(textureShader, "uv");

    //bind the vertex buffer to attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        POS_index,                                // attribute. 0 must match layout in shader
        3,                                // 3 floats per vertex
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );

    glGenBuffers(1, &upleftQuadBufferUV);
    glBindBuffer(GL_ARRAY_BUFFER, upleftQuadBufferUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(upleftQuadBufferUVData), upleftQuadBufferUVData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        UV_index,                                // attribute. 1 must match layout in shader
        2,                                // 2 floats per set of UV
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride - tightly packed
        (void*)0                          // array buffer offset
    );
}

void createRandomTexture()
{
    glGenTextures(1, &randomTexture);
    glBindTexture(GL_TEXTURE_2D, randomTexture);
}

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
    inputVal_ID = glGetUniformLocation(baseShader, "inputVal");

	//Create Process Shader
    vertexShader = CreateShader("process.vert", GL_VERTEX_SHADER);
    pixelShader = CreateShader("process.frag", GL_FRAGMENT_SHADER);

	processShader = glCreateProgram();
    glBindAttribLocation(processShader, 0, "position");
    glBindAttribLocation(processShader, 1, "uv");
    LinkProgram(processShader, vertexShader, pixelShader);


    //Create Texture Shader
    vertexShader = CreateShader("texture.vert", GL_VERTEX_SHADER);
    pixelShader = CreateShader("texture.frag", GL_FRAGMENT_SHADER);
    textureShader = glCreateProgram();
    glBindAttribLocation(textureShader, 0, "position");
    glBindAttribLocation(textureShader, 1, "uv");
    LinkProgram(textureShader, vertexShader, pixelShader);

    printActiveUniforms(baseShader);

    
    //World, View, Projection matrices
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 0, 1000),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );
    glm::mat4 projection = glm::perspective(
        85.0f,        //FOV
        4.0f / 3.0f,  //aspect ratio
        0.1f,         //near plane
        100000.0f        //far plane
    );
    viewProj =  projection * view;


    frame1.CreateFBO(textureWidth, textureHeight);
	createVertexArray();
    createSingleArray();
    createQuadArray();
    createULQuadArray();
    createRandomTexture();
}

void processingPass()
{
    glEnable(GL_POINT_SPRITE);

    glUseProgram(processShader);
    frame1.Bind();

    glViewport(0, 0, textureWidth, textureHeight);
    glClearColor( 0.0f, 1.0f, 0.0f, 1.0f ); //green texture
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize((float)textureWidth);


        //update matrix in shader
        glBindVertexArray(quad_AO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

    frame1.Unbind();
    glUseProgram(0);
    glDisable(GL_POINT_SPRITE);
}

void textureDraw()
{
    glUseProgram(textureShader);

        glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frame1.texture);
        int texture0ID = glGetUniformLocation(textureShader, "texture0");
		glUniform1i(texture0ID, 0); //bind texture 0 to GL_TEXTURE0
		
        

        //draw fbo texture
        glBindVertexArray(upleftQuad_AO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

    glUseProgram(0);
}

void particleDraw()
{
    glPointSize(2);

    static float angle = 0.5f;
    angle += 0.01f;
    
    glm::mat4 startTrans = glm::translate(glm::mat4(),glm::vec3(-0.5,-0.6,-1));
    glm::mat4 scaleMat = glm::scale(glm::mat4(),glm::vec3(8096));
    glm::mat4 rotMat = glm::rotate(glm::mat4(), angle*8, glm::vec3(0,1,0));
    glm::mat4 world = rotMat * scaleMat * startTrans;

    glm::mat4 result = viewProj * world;

    glUseProgram(baseShader);

        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frame1.texture);
        int baseShaderTexture0ID = glGetUniformLocation(baseShader, "texture0");
		glUniform1i(baseShaderTexture0ID, 0);

        int textureSizeID = glGetUniformLocation(baseShader, "textureSize");
        glUniform1i(textureSizeID, textureHeight);

        //draw 3 points
        glUniformMatrix4fv(MVP_Matrix_ID, 1, GL_FALSE, &(result[0][0]));
        glUniform1f(inputVal_ID, angle);
        //glBindVertexArray(vertex_AO); //Actually doesn't need this. neat
        int num = frame1.numPixels();
        glDrawArrays(GL_POINTS, 0, num);
        glBindVertexArray(0);
        
    glUseProgram(0);
}

void Draw()
{
    //--------------processing pass------------------
    processingPass();	

    //CLEAR SCREEN
    glViewport(0,0,ScreenWidth, ScreenHeight);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //----------------texture draw--------------------
    textureDraw();

    //--------------- point draw ------------------

    particleDraw();
	

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
