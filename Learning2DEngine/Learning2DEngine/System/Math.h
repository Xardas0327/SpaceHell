#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
	namespace System
	{
        struct Math
        {
            static inline glm::mat4x2 FlipByX(const glm::mat4x2& matrix)
            {
                return glm::mat4x2{
                    matrix[1].x, matrix[1].y,
                    matrix[0].x, matrix[0].y,
                    matrix[3].x, matrix[3].y,
                    matrix[2].x, matrix[2].y,
                };
            }

            static inline glm::mat4x2 FlipByY(const glm::mat4x2& matrix)
            {
                return glm::mat4x2{
                    matrix[2].x, matrix[2].y,
                    matrix[3].x, matrix[3].y,
                    matrix[0].x, matrix[0].y,
                    matrix[1].x, matrix[1].y,
                };
            }
        };
	}
}