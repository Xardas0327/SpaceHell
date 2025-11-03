#include "SpriteRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{
		const std::string SpriteRenderComponent::id = "L2DE_SpriteRenderComponent";

		SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject, RendererMode mode, int layer, glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, color), Component(gameObject)
		{
		}

		SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject, RendererMode mode, const Texture2D& texture, int layer, glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, texture, color), Component(gameObject)
		{
		}

		const std::string& SpriteRenderComponent::GetId() const
		{
			return SpriteRenderComponent::id;
		}

		MultiSpriteRenderer* SpriteRenderComponent::GetInitedRenderer()
		{
			auto& renderer = MultiSpriteRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void SpriteRenderComponent::DestroyRenderer()
		{
			MultiSpriteRenderer::GetInstance().Destroy();
		}
	}
}