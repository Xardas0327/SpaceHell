#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "../DebugTool/Log.h"
#include "../System/Component.h"
#include "../System/GameObject.h"

#include "Collision.h"
#include "ColliderType.h"
#include "ColliderMode.h"
#include "BaseRigidbody.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The classes, which are inherited from BaseColliderComponent,
        /// have to have a constructor, which first parameter is GameObject* for gameObject member.
        /// Moreover, it is recommended, that the constructor, the Init() and Destroy() of the inherited class is protected
        /// and only the GameObject can use them.
        /// </summary>
        class BaseColliderComponent : public virtual System::Component
        {
        private:
            BaseRigidbody* rigidbody;
        protected:
            BaseColliderComponent(
                System::GameObject* gameObject,
                ColliderType type = ColliderType::DYNAMIC,
                ColliderMode mode = ColliderMode::TRIGGER,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), type(type), mode(mode), colliderOffset(offset), maskLayer(maskLayer), rigidbody(nullptr)
            {

            }
        public:
            const ColliderType type;
            ColliderMode mode;
            glm::vec2 colliderOffset;
            int32_t maskLayer;

            virtual glm::vec2 GetColliderCenter() const = 0;

            virtual void OnCollision(const Collision& collision)
            {

            }

            //If the rigidbody is inited, the collider can use it in collision.
            void InitRigidbody()
            {
                rigidbody = gameObject->GetComponent<BaseRigidbody>();
                if (rigidbody == nullptr)
                {
                    L2DE_LOG_ERROR("COLLIDER: The Rigidbody is not found.");
                }
            }

            inline void ClearRigidbody()
            {
                rigidbody = nullptr;
            }

            inline BaseRigidbody* GetRigidbody() const
            {
                return rigidbody;
            }
        };
    }
}
