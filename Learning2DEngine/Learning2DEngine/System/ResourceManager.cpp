#include "ResourceManager.h"

#include <sstream>
#include <fstream>
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../DebugTool/Log.h"

using namespace Learning2DEngine::Render;

namespace Learning2DEngine
{
	namespace System
	{
        ResourceManager::ResourceManager()
            : shaders(), textures()
        {
        }

        bool ResourceManager::LoadShaderFile(const char* filePath, std::string& outSource)
        {
            std::ifstream shaderFile;
            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            std::stringstream stream;
            stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                shaderFile.open(filePath);
                stream << shaderFile.rdbuf();
                shaderFile.close();
            }
            catch (std::exception e)
            {
                if (shaderFile.is_open())
                    shaderFile.close();

                L2DE_LOG_ERROR(std::string("SHADER: Failed to read shader file.\n File: ") + filePath + "\n Message: " + e.what());
                return false;
            }

            outSource = stream.str();
            return true;
        }

        Shader& ResourceManager::LoadShaderFromFile(
            const std::string& id, const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath)
        {
            Shader& shader = shaders[id];
            //it was inited before
            if (shader.GetId() > 0)
            {
                L2DE_LOG_WARNING(std::string("TEXTURE: this id was already used: ") + id);
                return shader;
            }

            std::string vertexSource;
            std::string fragmentSource;
            std::string geometrySource;

            //It can show all files' errors.
            bool isFine = true;
            isFine = LoadShaderFile(vertexFilePath, vertexSource) && isFine;
            isFine = LoadShaderFile(fragmentFilePath, fragmentSource) && isFine;
            if (geometryFilePath != nullptr)
            {
                isFine = LoadShaderFile(geometryFilePath, geometrySource) && isFine;
            }

            if (isFine)
            {
                shader.Create(vertexSource.c_str(), fragmentSource.c_str(), geometryFilePath != nullptr ? geometrySource.c_str() : nullptr);
            }

            return shader;
        }

        Shader& ResourceManager::LoadShader(
            const std::string& id, const char* vertexText, const char* fragmentText, const char* geometryText)
        {
            Shader& shader = shaders[id];
            //it was inited before
            if (shader.GetId() > 0)
            {
                L2DE_LOG_WARNING(std::string("TEXTURE: this id was already used: ") + id);
                return shader;
            }

            shader.Create(vertexText, fragmentText, geometryText != nullptr ? geometryText : nullptr);
            return shader;
        }

        Shader& ResourceManager::GetShader(const std::string& id)
        {
            return shaders[id];
        }

        bool ResourceManager::IsShaderExist(const std::string& id)
        {
            return shaders.count(id);
        }

        void ResourceManager::DestroyShader(const std::string& id)
        {
            shaders[id].Destroy();
            shaders.erase(id);
        }

        Texture2D& ResourceManager::LoadTextureFromFile(const std::string& id, const char* filePath, const Texture2DSettings& settings)
        {
            auto response = textures.emplace(id, settings);
            if (!response.second)
            {
                L2DE_LOG_WARNING(std::string("TEXTURE: this id was already used: ") + id);
                return response.first->second;
            }

            int width;
            int height;
            int nrChannels;
            unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
            response.first->second.Create(width, height, data);

            return response.first->second;
        }

        Texture2D& ResourceManager::GetTexture(const std::string& id)
        {
            return textures[id];
        }

        bool ResourceManager::IsTextureExist(const std::string& id)
        {
            return textures.count(id);
        }

        void ResourceManager::DestroyTexture(const std::string& id)
        {
            textures[id].Destroy();
            textures.erase(id);
        }

        void ResourceManager::Clear()
        {	
            for (auto& shader : shaders)
            {
                shader.second.Destroy();
            }
            shaders.clear();

            if (textures.size() > 0)
            {
                unsigned int* textureIds = new unsigned int[textures.size()];
                GLsizei index = 0;
                for (auto i = textures.begin(); i != textures.end(); ++i)
                {
                    textureIds[index] = i->second.GetId();
                    ++index;
                }

                glDeleteTextures(index, textureIds);
                delete[] textureIds;
                textures.clear();
            }
        }
	}
}