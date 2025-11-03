#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Learning2DEngine
{
    namespace System
    {
        Camera::Camera(glm::vec2 position, float rotation)
            : position(position), rotation(rotation),
                viewMatrix(0.0f), projection(0.0f), resolution(0,0)
        {
            RecalcViewMatrix();
        }

        void Camera::SetPosition(glm::vec2 position)
        {
            this->position = position;
            RecalcViewMatrix();
        }

        void Camera::Move(glm::vec2 direction, bool isWorldMoving)
        {
            if (!isWorldMoving)
            {
                float angle = glm::radians(-rotation);

                glm::mat2 rotationMatrix = glm::mat2(
                    cos(angle), -sin(angle),
                    sin(angle), cos(angle));

                direction = rotationMatrix * direction;
            }

            position += direction;
            RecalcViewMatrix();
        }

        void Camera::SetRotation(float rotation)
        {
            this->rotation = rotation;
            RecalcViewMatrix();
        }

        void Camera::Rotate(float angle)
        {
            rotation += angle;
            RecalcViewMatrix();
        }

        void Camera::SetPositionRotation(glm::vec2 position, float rotation)
        {
            this->position = position;
            this->rotation = rotation;
            RecalcViewMatrix();
        }

        void Camera::RecalcViewMatrix()
        {
            viewMatrix = glm::mat4(1.0f);
            //It uses the inverse of the position, because the camera look at the scene.
            viewMatrix = glm::translate(viewMatrix, -1.0f * glm::vec3(position, 0.0f));

            float halfWidth = 0.5f * static_cast<float>(resolution.GetWidth());
            float halfHeight = 0.5f * static_cast<float>(resolution.GetHeight());

            // move origin of rotation to center
            viewMatrix = glm::translate(viewMatrix,  glm::vec3( halfWidth + position.x, halfHeight + position.y, 0.0f));
            // then rotate
            //It uses the inverse of the rotation, because the camera look at the scene.
            viewMatrix = glm::rotate(viewMatrix, glm::radians(-rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            // move origin back
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-halfWidth - position.x, -halfHeight - position.y, 0.0f));
            
        }

        void Camera::SetResolution(const Render::Resolution& resolution)
        {
            this->resolution = resolution;
            projection = glm::ortho(
                0.0f,
                static_cast<float>(resolution.GetWidth()),
                static_cast<float>(resolution.GetHeight()),
                0.0f,
                -1.0f,
                1.0f
            );
        }
    }
}