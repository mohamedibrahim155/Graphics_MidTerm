#include "VertexBuffer.h"
#include <glad/glad.h>  

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{

    glGenBuffers(1, &RendId);
    glBindBuffer(GL_ARRAY_BUFFER, RendId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &RendId);
}
void VertexBuffer::Bind() const
{

    glBindBuffer(GL_ARRAY_BUFFER, RendId);  // at the moment this code is pointless lol

}


void VertexBuffer::UnBind() const
{

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}