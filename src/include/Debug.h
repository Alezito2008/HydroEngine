#pragma once
#include <glad/glad.h>
#include <csignal>

#define DEBUG 1

#if defined(_WIN32) || defined(_WIN64)
    #define DEBUG_BREAK() __debugbreak()
#else
    #define DEBUG_BREAK() raise(SIGTRAP)
#endif

#define ASSERT(x) if (!(x)) DEBUG_BREAK();

#if DEBUG == 1
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
    #define GLCall(x) x;
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
