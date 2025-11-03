#pragma once

#include "Texture2D.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class Texture2DContainer
        {
        private:
            const Texture2D* texture;
        public:
            Texture2DContainer()
                : texture(nullptr)
            {

            }

            // It will not copy the texture, it will use texture reference.
            Texture2DContainer(const Texture2D& texture)
                : texture(&texture)
            {

            }

            virtual ~Texture2DContainer() = default;

            // It will not copy the texture, it will use texture reference.
            void SetTexture(const Texture2D& texture)
            {
                this->texture = &texture;
            }

            inline const Texture2D* GetTexture() const
            {
                return texture;
            }

            inline void ClearTexture()
            {
                texture = nullptr;
            }

            inline bool IsUseTexture() const
            {
                return texture != nullptr;
            }
        };
    }
}