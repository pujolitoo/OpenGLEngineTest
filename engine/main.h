#ifndef MAIN_H
#define MAIN_H

#include<windows.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Core.h"

#if defined(_WIN64) || defined(WIN32)
    #define APIEXPORT __declspec(dllexport)
#endif


#endif //MAIN_H