#include "IndexBuffer.h"
#include <glad/glad.h>
#include <iostream>

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
{
    glGenBuffers(1, &m_ID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*count, data, GL_STATIC_DRAW);
    m_Count = count;
}

IndexBuffer::~IndexBuffer()
{
    Unbind();
    glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::count() const
{
    return m_Count;
}