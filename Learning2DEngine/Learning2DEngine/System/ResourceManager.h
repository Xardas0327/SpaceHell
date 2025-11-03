#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Singleton.h"
#include "../Render/Shader.h"
#include "../Render/Texture2D.h"
#include "../Render/Texture2DSettings.h"

namespace Learning2DEngine
{
	namespace System
	{
        class ResourceManager : public Singleton<ResourceManager>
        {
            friend class Singleton<ResourceManager>;
        private:
            std::map<std::string, Render::Shader> shaders;
            std::map<std::string, Render::Texture2D> textures;

            ResourceManager();
            /// <summary>
            /// Return false, if the std::ifstream or the std::stringstream thrown exception by std::ifstream::failbit or std::ifstream::badbit.
            /// Otherwise, it returns true.
            /// </summary>
            /// <param name="file"></param>
            /// <param name="outSource"></param>
            /// <returns></returns>
            bool LoadShaderFile(const char* filePath, std::string& outSource);
        public:
            // If the id is used, it will return the old one.
            Render::Shader& LoadShaderFromFile(
                const std::string& id, const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath = nullptr);
            // If the id is used, it will return the old one.
            Render::Shader& LoadShader(const std::string& id, const char* vertexText, const char* fragmentText, const char* geometryText = nullptr);
            Render::Shader& GetShader(const std::string& id);
            bool IsShaderExist(const std::string& id);
            void DestroyShader(const std::string& id);

            // If the id is used, it will return the old one.
            Render::Texture2D& LoadTextureFromFile(const std::string& id, const char* filePath, const Render::Texture2DSettings& settings);
            Render::Texture2D& GetTexture(const std::string& id);
            bool IsTextureExist(const std::string& id);
            void DestroyTexture(const std::string& id);
            void Clear();
        };
	}
}