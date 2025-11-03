#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "BaseColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The BaseBoxColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
        /// Please use BoxColliderComponent instead of this.
        /// </summary>
        class BaseBoxColliderComponent : public BaseColliderComponent
        {
        protected:
            BaseBoxColliderComponent(
                System::GameObject* gameObject,
                glm::vec2 size,
                ColliderType type = ColliderType::DYNAMIC,
                ColliderMode mode = ColliderMode::TRIGGER,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseColliderComponent(gameObject, type, mode, offset, maskLayer),
                colliderSize(size)
            {

            }
        public:
            glm::vec2 colliderSize;

            glm::vec2 GetColliderCenter() const override
            {
                glm::vec2 position = gameObject->transform.GetPosition() + colliderOffset;
                position.x += colliderSize.x / 2;
                position.y += colliderSize.y / 2;

                return position;
            }
        };
    }
}
