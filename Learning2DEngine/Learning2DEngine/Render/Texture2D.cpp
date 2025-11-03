#include "Texture2D.h"

namespace Learning2DEngine
{
    namespace Render
    {
        Texture2D::Texture2D()
            : id(0), width(0), height(0), settings()
        {
        }

        Texture2D::Texture2D(const Texture2DSettings& texture2DSettings)
            : id(0), width(0), height(0), settings(texture2DSettings)
        {

        }

        void Texture2D::Create(unsigned int width, unsigned int height, const unsigned char* data)
        {
            this->width = width;
            this->height = height;

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, settings.internalFormat, width, height, 0, settings.imageFormat, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrapT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filterMin);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filterMag);

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture2D::Destroy()
        {
            glDeleteTextures(1, &id);
        }

        void Texture2D::Bind(int textureUnitNumber) const
        {
            glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
            glBindTexture(GL_TEXTURE_2D, id);
        }
    }
}