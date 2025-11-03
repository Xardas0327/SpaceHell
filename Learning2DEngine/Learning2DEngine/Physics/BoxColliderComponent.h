#pragma once

#include <glm/glm.hpp>

#include "../DebugTool/DebugMacro.h"
#if L2DE_DEBUG_SHOW_COLLIDER
    #include "../DebugTool/DebugBoxColliderRenderComponent.h"
#endif
#include "../System/GameObject.h"
#include "../System/ComponentManager.h"
#include "BaseBoxColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The BoxColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
        /// Please check for more information in the BaseColliderComponent and BaseBoxColliderComponent.
        /// </summary>
        class BoxColliderComponent : public BaseBoxColliderComponent
        {
            friend class System::GameObject;
        protected:
            BoxColliderComponent(
                System::GameObject* gameObject,
                glm::vec2 size,
                ColliderType type = ColliderType::DYNAMIC,
                ColliderMode mode = ColliderMode::TRIGGER,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseBoxColliderComponent(gameObject, size, type, mode, offset, maskLayer)
#if L2DE_DEBUG_SHOW_COLLIDER
                , debugTool(nullptr)
#endif
            {
            }

            /// <summary>
            /// If this function is override, it must call the BoxColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override
            {
                System::ComponentManager::GetInstance().AddToCollider(this);
#if L2DE_DEBUG_SHOW_COLLIDER
                debugTool = gameObject->AddComponent<DebugTool::DebugBoxColliderRenderComponent>(this);
                debugTool->isActive = L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_VALUE;
#endif
            }

            /// <summary>
            /// If this function is override, it must call the BoxColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override
            {
                System::ComponentManager::GetInstance().RemoveFromCollider(this);
            }
        public:
#if L2DE_DEBUG_SHOW_COLLIDER
            DebugTool::DebugBoxColliderRenderComponent* debugTool;
#endif
        };
    }
}
