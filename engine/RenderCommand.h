#ifndef __RENDERCOMMAND_H__
#define __RENDERCOMMAND_H__

#include <glad/glad.h>


class RenderCommand
{
    public:
        static void Clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(1.0f, 0.5f, 1.0f, 0.0f);
        }
};

#endif // __RENDERCOMMAND_H__