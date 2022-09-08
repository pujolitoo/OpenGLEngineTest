#include<iostream>

#define E_ASSERT(x, message) if(!x){std::cout << message << std::endl;}

#if defined(_WIN64) || defined(WIN32)
    #define E_PLATFORM_WINDOWS
#endif


#if defined(E_PLATFORM_WINDOWS)
    #define APIEXPORT __declspec(dllexport)

#else
    #error "PLATFORM NOT SUPPORTED"
#endif