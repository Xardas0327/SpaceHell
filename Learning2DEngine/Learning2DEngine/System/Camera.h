#pragma once

#include <glm/glm.hpp>

#include "../Render/Resolution.h"

namespace Learning2DEngine
{
    namespace System
    {
        class Camera
        {
        private:
            glm::vec2 position;
            float rotation;

            glm::mat4 viewMatrix;
            glm::mat4 projection;
            Render::Resolution resolution;

            void RecalcViewMatrix();
        public:
            Camera(glm::vec2 position = glm::vec2(0.0f, 0.0f), float rotation = 0.0f);

            inline glm::vec2 GetPosition()
            {
                return position;
            }
            //After the position update, it will recalculate the viewMatrix.
            void SetPosition(glm::vec2 position);

            //If it is a isWorldMoving, the direction will be rotated by the actual rotation.
            //After the position update, it will recalculate the viewMatrix.
            void Move(glm::vec2 direction, bool isWorldMoving = false);

            inline float GetRotation()
            {
                return rotation;
            }
            //After the rotation update, it will recalculate the viewMatrix.
            void SetRotation(float rotation);

            //After the rotation update, it will recalculate the viewMatrix.
            void Rotate(float angle);

            //After the position and the rotation update, it will recalculate the viewMatrix.
            void SetPositionRotation(glm::vec2 position, float rotation);

            inline glm::mat4 GetViewMatrix()
            {
                return viewMatrix;
            }

            inline glm::mat4 GetProjection()
            {
                return projection;
            }

            inline Render::Resolution GetResolution()
            {
                return resolution;
            }

            //After the resolution update, it will recalculate the projection.
            void SetResolution(const Render::Resolution& resolution);
        };
    }
}