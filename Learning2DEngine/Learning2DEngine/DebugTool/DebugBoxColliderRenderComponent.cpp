#include "DebugBoxColliderRenderComponent.h"

#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		const std::string DebugBoxColliderRenderComponent::id = "L2DE_DebugBoxColliderRenderComponent";

		DebugBoxColliderRenderComponent::DebugBoxColliderRenderComponent(GameObject* gameObject, const BaseBoxColliderComponent* collider)
			: RendererComponent(gameObject, RendererMode::RENDER, L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_LAYER, collider), Component(gameObject)
		{
		}

		const std::string& DebugBoxColliderRenderComponent::GetId() const
		{
			return DebugBoxColliderRenderComponent::id;
		}

		DebugBoxColliderRenderer* DebugBoxColliderRenderComponent::GetInitedRenderer()
		{
			auto& renderer = DebugBoxColliderRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void DebugBoxColliderRenderComponent::DestroyRenderer()
		{
			DebugBoxColliderRenderer::GetInstance().Destroy();
		}
	}
}