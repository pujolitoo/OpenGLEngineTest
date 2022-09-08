#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float data[], unsigned int count)
{
    glGenBuffers(1, &vbId);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*count, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbId);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
    glDeleteBuffers(1, &vbId);
}