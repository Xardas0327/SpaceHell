#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
    namespace Render
    {
        Shader::Shader()
            : id(0)
        {

        }

        void Shader::Create(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
        {
            unsigned int vertexId, fragmentId, geometryId;

            // Vertex Shader
            vertexId = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexId, 1, &vertexSource, NULL);
            glCompileShader(vertexId);
            CheckShaderErrors(vertexId, ShaderType::VERTEX);

            // Fragment Shader
            fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentId, 1, &fragmentSource, NULL);
            glCompileShader(fragmentId);
            CheckShaderErrors(fragmentId, ShaderType::FRAGMENT);

            // Geometry Shader
            if (geometrySource != nullptr)
            {
                geometryId = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometryId, 1, &geometrySource, NULL);
                glCompileShader(geometryId);
                CheckShaderErrors(geometryId, ShaderType::GEOMETRY);
            }

            // Shader Program
            id = glCreateProgram();
            glAttachShader(id, vertexId);
            glAttachShader(id, fragmentId);
            if (geometrySource != nullptr)
                glAttachShader(id, geometryId);
            glLinkProgram(id);
            CheckProgramErrors(id);
            
            // Clear
            glDeleteShader(vertexId);
            glDeleteShader(fragmentId);
            if (geometrySource != nullptr)
                glDeleteShader(geometryId);
        }

        void Shader::Destroy()
        {
            glDeleteProgram(id);
        }

        std::string Shader::ToString(ShaderType type)
        {
            switch (type)
            {
            case ShaderType::VERTEX:
                return "Vertex";
            case ShaderType::FRAGMENT:
                return "Fragment";
            case ShaderType::GEOMETRY:
                return "Geometry";
            }

            return "Unknow";
        }

        void Shader::CheckShaderErrors(unsigned int shaderId, ShaderType type)
        {
            int success;
            char infoLog[1024];

            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
                L2DE_LOG_ERROR("SHADER::" + Shader::ToString(type) + ": " + infoLog);
            }
        }

        void Shader::CheckProgramErrors(unsigned int programId)
        {
            int success;
            char infoLog[1024];
            glGetProgramiv(programId, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(programId, 1024, NULL, infoLog);
                L2DE_LOG_ERROR(std::string("SHADER::PROGRAM: ") + infoLog);
            }
        }

        void Shader::Use() const
        {
            glUseProgram(id);
        }

        void Shader::SetFloat(const char* name, float value)
        {
            glUniform1f(glGetUniformLocation(id, name), value);
        }

        void Shader::SetInteger(const char* name, int value)
        {
            glUniform1i(glGetUniformLocation(id, name), value);
        }

        void Shader::SetVector2f(const char* name, float x, float y)
        {
            glUniform2f(glGetUniformLocation(id, name), x, y);
        }

        void Shader::SetVector2f(const char* name, const glm::vec2& value)
        {
            glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
        }

        void Shader::SetVector3f(const char* name, float x, float y, float z)
        {
            glUniform3f(glGetUniformLocation(id, name), x, y, z);
        }

        void Shader::SetVector3f(const char* name, const glm::vec3& value)
        {
            glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
        }

        void Shader::SetVector4f(const char* name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
        }

        void Shader::SetVector4f(const char* name, const glm::vec4& value)
        {
            glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);
        }

        void Shader::SetMatrix4(const char* name, const glm::mat4& matrix)
        {
            glUniformMatrix4fv(glGetUniformLocation(id, name), 1, false, glm::value_ptr(matrix));
        }

        void Shader::SetArray1f(const char* name, float* value, unsigned int length)
        {
            glUniform1fv(glGetUniformLocation(id, name), length, value);
        }

        void Shader::SetArray1i(const char* name, int* value, unsigned int length)
        {
            glUniform1iv(glGetUniformLocation(id, name), length, value);
        }

        void Shader::SetArray2f(const char* name, float* value, unsigned int length)
        {
            glUniform2fv(glGetUniformLocation(id, name), length, value);
        }

        void Shader::SetArray2i(const char* name, int* value, unsigned int length)
        {
            glUniform2iv(glGetUniformLocation(id, name), length, value);
        }
    }
}