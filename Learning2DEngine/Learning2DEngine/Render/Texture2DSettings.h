#pragma once

#include <glad/glad.h>

namespace Learning2DEngine
{
    namespace Render
    {
        struct Texture2DSettings
        {
            // Format of the texture object
            unsigned int internalFormat;
            // Format of the loaded image
            unsigned int imageFormat;
            // Wrapping mode on S axis
            unsigned int wrapS;
            // Wrapping mode on T axis
            unsigned int wrapT;
            // Filtering mode if texture pixels < window pixels
            unsigned int filterMin;
            // Filtering mode if texture pixels > window pixels
            unsigned int filterMag;

            Texture2DSettings(bool useAlpha = false)
                : internalFormat(useAlpha ? GL_RGBA : GL_RGB), imageFormat(useAlpha ? GL_RGBA : GL_RGB),
                    wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMag(GL_LINEAR)
            {

            }
        };
    }
}