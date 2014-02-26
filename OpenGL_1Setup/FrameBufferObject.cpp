#include "FrameBufferObject.h"
#include <glload/gl_all.h>
#include <GL/freeglut.h>
#include <iostream>

FrameBufferObject::FrameBufferObject(void)
{
}


FrameBufferObject::~FrameBufferObject(void)
{
}

void FrameBufferObject::CreateFBO(const int w, const int h)
{
    width = w;
    height = h;

    glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

    // Create a render buffer, and attach it to FBO's depth attachment
    unsigned int depthBufferID;
    glGenRenderbuffersEXT(1, &depthBufferID);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferID);
    glRenderbufferStorageEXT(
        GL_RENDERBUFFER_EXT, 
        GL_DEPTH_COMPONENT,
        width, height
    );

    glFramebufferRenderbufferEXT(
        GL_FRAMEBUFFER_EXT, 
        GL_DEPTH_ATTACHMENT_EXT,
        GL_RENDERBUFFER_EXT, 
        depthBufferID
    );

    // Create texture and attach FBO's color 0 attachment
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA32F_ARB, 
        width, height,
        0,
        GL_RGBA,
        GL_FLOAT,
        NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                              GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, texture, 0);

    // Check for completeness/correctness
    int status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        printf("ERROR ON FRAME BUFFER OBJECT: %d\n", status);
    }

    // Unbind the fbo.
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void FrameBufferObject::Bind() { glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); }
void FrameBufferObject::Unbind() { glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); }