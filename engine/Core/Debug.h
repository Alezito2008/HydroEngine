#pragma once
#define DEBUG 1

#define ASSERT(x) if (!(x)) __debugbreak();

#if DEBUG == 1
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x;
#endif

namespace Hydro
{
    void GLClearError();
    bool GLLogCall(const char* function, const char* file, int line);
}


