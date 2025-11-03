#pragma once

#include <glm/glm.hpp>

#include "../System/Transform.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
        class Particle
        {
        private:
            static constexpr const glm::mat4x2 DefaultUV = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

        public:
            System::Transform transform;
            glm::vec2 velocity;
            glm::vec4 color;
            //In seconds. The negative number is same as 0.
            float lifeTime;
            float speed;

            /// <summary>
            /// The texture coordinate order:
            /// Top Left,
            /// Top Right,
            /// Bottom Right,
            /// Bottom Left
            /// </summary>
            glm::mat4x2 uvMatrix;

            Particle() 
                : transform(), velocity(0.0f), color(1.0f), lifeTime(0.0f), speed(1.0f),
                uvMatrix(Particle::DefaultUV)
            {
            }
        };
	}
}