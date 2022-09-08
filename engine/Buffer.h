#ifndef BUFFER_H
#define BUFFER_H

#pragma once

#include <cstdint>

class Buffer
{
public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};

class VertexBuffer : public Buffer
{
    public:
        static VertexBuffer* Create(float* vertices, size_t size);
};

class IndexBuffer : public Buffer
{
    public:
        static IndexBuffer* Create(uint32_t* vertices, size_t size);
};

#endif