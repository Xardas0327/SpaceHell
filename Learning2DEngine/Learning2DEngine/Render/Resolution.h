#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Learning2DEngine
{
    namespace Render
    {
        class Resolution
        {
        private:
            int width;
            int height;
        public:
            Resolution(int w, int h)
                : width(w), height(h)
            {

            }

            inline int GetWidth() const
            {
                return width;
            }

            inline int GetHeight() const
            {
                return height;
            }

            inline std::string ToString() const
            {
                return std::to_string(width) + "x" + std::to_string(height);
            }

            inline glm::vec2 ToVec2() const
            {
                return glm::vec2(width, height);
            }
        };
    }
}