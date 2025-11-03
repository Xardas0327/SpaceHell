#include "DebugCircleColliderRenderComponent.h"

#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		const std::string DebugCircleColliderRenderComponent::id = "L2DE_DebugCircleColliderRenderComponent";

		DebugCircleColliderRenderComponent::DebugCircleColliderRenderComponent(GameObject* gameObject, const BaseCircleColliderComponent* collider)
			: RendererComponent(gameObject, RendererMode::RENDER, L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_LAYER, collider), Component(gameObject)
		{
		}

		const std::string& DebugCircleColliderRenderComponent::GetId() const
		{
			return DebugCircleColliderRenderComponent::id;
		}

		DebugCircleColliderRenderer* DebugCircleColliderRenderComponent::GetInitedRenderer()
		{
			auto& renderer = DebugCircleColliderRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void DebugCircleColliderRenderComponent::DestroyRenderer()
		{
			DebugCircleColliderRenderer::GetInstance().Destroy();
		}
	}
}