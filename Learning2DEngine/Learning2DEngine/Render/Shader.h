#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Render
    {
        class Shader
        {
        private:
            enum class ShaderType {VERTEX, FRAGMENT, GEOMETRY};
            static std::string ToString(ShaderType type);

            GLuint id;
            void CheckShaderErrors(unsigned int shaderId, ShaderType type);
            void CheckProgramErrors(unsigned int programId);
        public:
            Shader();
            void Create(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
            void Destroy();
            void Use() const;

            inline GLuint GetId()
            {
                return id;
            }
            void SetFloat(const char* name, float value);
            void SetInteger(const char* name, int value);
            void SetVector2f(const char* name, float x, float y);
            void SetVector2f(const char* name, const glm::vec2& value);
            void SetVector3f(const char* name, float x, float y, float z);
            void SetVector3f(const char* name, const glm::vec3& value);
            void SetVector4f(const char* name, float x, float y, float z, float w);
            void SetVector4f(const char* name, const glm::vec4& value);
            void SetMatrix4(const char* name, const glm::mat4& matrix);
            void SetArray1f(const char* name, float* value, unsigned int length);
            void SetArray1i(const char* name, int* value, unsigned int length);
            void SetArray2f(const char* name, float* value, unsigned int length);
            void SetArray2i(const char* name, int* value, unsigned int length);
        };
    }
}