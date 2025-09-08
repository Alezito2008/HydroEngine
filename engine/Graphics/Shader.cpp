#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Hydro
{

// Constructor
    Shader::Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Destructor
    Shader::~Shader() {
        glDeleteProgram(ID);
    }

    // Activar shader
    void Shader::use() const {
        glUseProgram(ID);
    }

    // Uniform location cache
    unsigned int Shader::GetUniformLocation(const std::string& name) const {
        auto it = m_LocationCache.find(name);
        if (it != m_LocationCache.end())
            return it->second;

        unsigned int loc = glGetUniformLocation(ID, name.c_str());
        m_LocationCache[name] = loc;
        return loc;
    }

    // Setters básicos
    void Shader::setBool(const std::string& name, bool value) const {
        glUniform1i(GetUniformLocation(name), (int)value);
    }
    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(GetUniformLocation(name), value);
    }
    void Shader::setFloat(const std::string& name, float value) const {
        glUniform1f(GetUniformLocation(name), value);
    }

    // Vec2
    void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }
    void Shader::setVec2(const std::string& name, float x, float y) const {
        glUniform2f(GetUniformLocation(name), x, y);
    }

    // Vec3
    void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }
    void Shader::setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    // Vec4
    void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }
    void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }

    // Matrices
    void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    // MVP
    void Shader::setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const {
        setMat4("mvp.model", model);
        setMat4("mvp.view", view);
        setMat4("mvp.projection", projection);
    }

    // Error check
    void Shader::checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                          << "\n" << infoLog << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                          << "\n" << infoLog << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
    }

} // Hydro