#pragma once

#include <glm/glm.hpp>

#include "../DebugTool/DebugMacro.h"
#if L2DE_DEBUG_SHOW_COLLIDER
#include "../DebugTool/DebugCircleColliderRenderComponent.h"
#endif
#include "../System/GameObject.h"
#include "../System/ComponentManager.h"
#include "BaseCircleColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The CircleColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
		/// Please check for more information in the BaseColliderComponent and BaseCircleColliderComponent.
        /// </summary>
        class CircleColliderComponent : public BaseCircleColliderComponent
        {
            friend class System::GameObject;
        protected:
            CircleColliderComponent(
                System::GameObject* gameObject,
                float radius,
                ColliderType type = ColliderType::DYNAMIC,
                ColliderMode mode = ColliderMode::TRIGGER,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseCircleColliderComponent(gameObject, radius, type, mode, offset, maskLayer)
#if L2DE_DEBUG_SHOW_COLLIDER
                , debugTool(nullptr)
#endif
            {

            }

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override
            {
				System::ComponentManager::GetInstance().AddToCollider(this);
#if L2DE_DEBUG_SHOW_COLLIDER
                debugTool = gameObject->AddComponent<DebugTool::DebugCircleColliderRenderComponent>(this);
                debugTool->isActive = L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_VALUE;
#endif
            }

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override
            {
				System::ComponentManager::GetInstance().RemoveFromCollider(this);
            }
        public:
#if L2DE_DEBUG_SHOW_COLLIDER
            DebugTool::DebugCircleColliderRenderComponent* debugTool;
#endif
        };
    }
}
