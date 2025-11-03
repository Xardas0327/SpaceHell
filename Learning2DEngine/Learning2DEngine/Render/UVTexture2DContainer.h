#pragma once

#include <glm/glm.hpp>

#include "Texture2DContainer.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class UVTexture2DContainer : public Texture2DContainer
        {
        private:
            static constexpr const glm::mat4x2 DefaultUV = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

        public:
            /// <summary>
            /// The texture coordinate order:
            /// Top Left,
            /// Top Right,
            /// Bottom Right,
            /// Bottom Left
            /// </summary>
            glm::mat4x2 uvMatrix;

            UVTexture2DContainer(glm::mat4x2 uvMatrix = UVTexture2DContainer::DefaultUV)
                : Texture2DContainer(), uvMatrix(uvMatrix)
            {

            }

            // It will not copy the texture, it will use texture reference.
            UVTexture2DContainer(const Texture2D& texture, glm::mat4x2 uvMatrix = UVTexture2DContainer::DefaultUV)
                : Texture2DContainer(texture), uvMatrix(uvMatrix)
            {

            }
        };
    }
}