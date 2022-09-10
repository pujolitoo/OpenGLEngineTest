#include "GLContext.h"

#include "Core.h"

void GLContext::Init(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        E_ASSERT(false, "OpenGL context error.");
    }

    LOG("OpenGL Version: " << glGetString(GL_VERSION));
}