#pragma once

#include <glad/glad.h>

#include "Texture2DSettings.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class Texture2D
        {
        private:
            GLuint id;
            // Width of the loaded image in pixels
            unsigned int width;
            // Height of the loaded image in pixels
            unsigned int height;
            Texture2DSettings settings;

        public:
            Texture2D();
            Texture2D(const Texture2DSettings& settings);
            void Create(unsigned int width, unsigned int height, const unsigned char* data);
            void Destroy();
            void Bind(int textureUnitNumber = 0) const;

            inline GLuint GetId() const
            {
                return id;
            }

            inline unsigned int GetWidth() const
            {
                return width;
            }

            inline unsigned int GetHeight() const
            {
                return height;
            }

            inline const Texture2DSettings& GetSettings() const
            {
                return settings;
            }
        };
    }
}