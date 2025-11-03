#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Physics
    {
        class BaseRigidbody
        {
        public:
            glm::vec2 velocity;

            BaseRigidbody(glm::vec2 velocity)
                :velocity(velocity)
            {

            }

            virtual ~BaseRigidbody() = default;
        };
    }
}
