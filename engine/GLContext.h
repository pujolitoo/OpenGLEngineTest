#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

class GLContext
{
public:
    static void Init(GLFWwindow* window);
    static void Shutdown();
};

#endif