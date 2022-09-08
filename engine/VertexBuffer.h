#ifndef VERTEX_B_H
#define VERTEX_B_H
#include<glad/glad.h>

class VertexBuffer
{
    public:
        VertexBuffer(const float data[], unsigned int count);
        ~VertexBuffer();
        void Bind();
        void Unbind();

    private:
        unsigned int vbId;
};

#endif