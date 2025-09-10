
#include <iostream>

#include "Debug.h"

#include "Debug.h"

#include "glad/glad.h"

namespace Hydro
{
    void GLClearError() {
        while (glGetError() != GL_NO_ERROR);
    }

    bool GLLogCall(const char* function, const char* file, int line) {
        while (GLenum error = glGetError()) {
            std::cout << "[OpenGL Error] (" << error << ") " << function <<
                "\n" << file << ":" << line << std::endl;
            return false;
        }
        return true;
    }
}
