#pragma once
class FrameBufferObject
{
    unsigned int fbo; //generated fbo GLint id
public:
    unsigned int texture; //GL int id for texture
    int width;  // width of texture
    int height; //height of texture


    FrameBufferObject(void);
    ~FrameBufferObject(void);

    void FrameBufferObject::CreateFBO(const int w, const int h);
    void FrameBufferObject::Bind() ;
    void FrameBufferObject::Unbind();

    size_t FrameBufferObject::sizeOfTexture();
};

