#pragma once

#include <string>

#include "Singleton.h"
#include "GameObjectManager.h"
#include "UpdaterComponentHandler.h"
#include "LateUpdaterComponentHandler.h"
#include "BaseUpdaterComponent.h"
#include "BaseLateUpdaterComponent.h"
#include "../DebugTool/Log.h"
#include "../Render/IRenderer.h"
#include "../Render/RenderData.h"
#include "../Render/RendererComponentHandler.h"
#include "../Render/RendererMode.h"
#include "../Physics/ColliderComponentHandler.h"
#include "../Physics/BaseBoxColliderComponent.h"
#include "../Physics/BaseCircleColliderComponent.h"

namespace Learning2DEngine
{

    namespace System
    {
        class ComponentManager final : public Singleton<ComponentManager>
        {
            friend class Singleton<ComponentManager>;
        private:
            UpdaterComponentHandler updaterComponentHandler;
            LateUpdaterComponentHandler lateUpdaterComponentHandler;
            Physics::ColliderComponentHandler colliderComponentHandler;
            Render::RendererComponentHandler rendererComponentHandler;

            bool isThreadSafe;

            ComponentManager()
                : updaterComponentHandler(), lateUpdaterComponentHandler(), colliderComponentHandler(),
                rendererComponentHandler(), isThreadSafe(false)
            {

            }
        public:
            //Update

            inline void AddToUpdate(BaseUpdaterComponent* component)
            {
                updaterComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromUpdate(BaseUpdaterComponent* component)
            {
                updaterComponentHandler.Remove(component, isThreadSafe);
            }

            inline void Update()
            {
                updaterComponentHandler.Run();
            }

            // If it is bigger then 0, than every component handlers and the GameObjectManager will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetUpdateMaxComponentPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                updaterComponentHandler.SetMaxComponentPerThread(value);
            }

            //LateUpdate

            inline void AddToLateUpdate(BaseLateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromLateUpdate(BaseLateUpdaterComponent* component)
            {
                lateUpdaterComponentHandler.Remove(component, isThreadSafe);
            }

            inline void LateUpdate()
            {
                lateUpdaterComponentHandler.Run();
            }

            // If it is bigger then 0, than every component handlers and the GameObjectManager will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetLateUpdateMaxComponentPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                lateUpdaterComponentHandler.SetMaxComponentPerThread(value);
            }

            //Collider

            inline void AddToCollider(Physics::BaseBoxColliderComponent* component)
            {
                colliderComponentHandler.Add(component, isThreadSafe);
            }

            inline void AddToCollider(Physics::BaseCircleColliderComponent* component)
            {
                colliderComponentHandler.Add(component, isThreadSafe);
            }

            inline void RemoveFromCollider(Physics::BaseBoxColliderComponent* component)
            {
                colliderComponentHandler.Remove(component, isThreadSafe);
            }

            inline void RemoveFromCollider(Physics::BaseCircleColliderComponent* component)
            {
                colliderComponentHandler.Remove(component, isThreadSafe);
            }

            inline void CheckCollision()
            {
                colliderComponentHandler.Run();
            }

            // If it is bigger then 0, than every component handlers and the GameObjectManager will be thread safe.
            // But if it is 0, the thread safe will not be turn off automatically.
            void SetMaxColliderPerThread(unsigned int value)
            {
                if (value > 0)
                    SetThreadSafe(true);

                colliderComponentHandler.SetMaxColliderPerThread(value);
            }

            //Render

            inline bool IsRendererExist(const std::string& id)
            {
                return rendererComponentHandler.IsRendererExist(id, isThreadSafe);
            }

            inline void AddRenderer(const std::string& id, Render::IRenderer* renderer)
            {
                rendererComponentHandler.AddRenderer(id, renderer, isThreadSafe);
            }

            inline void RemoveRenderer(const std::string& id)
            {
                rendererComponentHandler.RemoveRenderer(id, isThreadSafe);
            }

            inline void AddRenderData(Render::RendererMode mode, const std::string& id, Render::RenderData* data, int layer)
            {
                rendererComponentHandler.AddData(id, mode, data, layer, isThreadSafe);
            }

            inline void ChangeRenderLayer(Render::RenderData* data, int newLayer)
            {
                rendererComponentHandler.ChangeLayer(data, newLayer, isThreadSafe);
            }

            inline void RemoveRenderData(Render::RenderData* data)
            {
                rendererComponentHandler.RemoveData(data, isThreadSafe);
            }

            inline void SetDataToRenderers()
            {
                rendererComponentHandler.SetDataToRenderers();
            }

            inline void Render()
            {
                rendererComponentHandler.Run(Render::RendererMode::RENDER);
            }

            inline void LateRender()
            {
                rendererComponentHandler.Run(Render::RendererMode::LATERENDER);
            }

            //All

            //It set the thread safe mode the ComponentManager and the GameObjectManager too.
            inline void SetThreadSafe(bool value)
            {
                isThreadSafe = value;
                GameObjectManager::GetInstance().SetThreadSafe(value);
#if L2DE_DEBUG
                if (!isThreadSafe && (
                    updaterComponentHandler.GetMaxComponentPerThread() > 0
                    || lateUpdaterComponentHandler.GetMaxComponentPerThread() > 0
                    || colliderComponentHandler.GetMaxColliderPerThread() > 0
                    ))
                {
                    L2DE_LOG_WARNING("COMPONENT MANAGER: The thread safe is turned off. But the component handlers still use threads.");
                }
#endif
            }

            inline bool GetThreadSafe()
            {
                return isThreadSafe;
            }

            void Clear()
            {
                updaterComponentHandler.Clear();
                lateUpdaterComponentHandler.Clear();
                colliderComponentHandler.Clear();
                rendererComponentHandler.Clear();
            }
        };
    }
}