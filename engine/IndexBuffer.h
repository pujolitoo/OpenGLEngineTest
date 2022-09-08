#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <cstdint>

#pragma once

class IndexBuffer
{
public:
    IndexBuffer(unsigned int* data, unsigned int count);
    ~IndexBuffer();
    void Unbind();
    void Bind();
    unsigned int count() const;


private:
    unsigned int m_ID;
    unsigned int m_Count;
};

#endif