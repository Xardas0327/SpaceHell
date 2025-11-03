#include "SimpleSpriteRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{
		const std::string SimpleSpriteRenderComponent::id = "L2DE_SimpleSpriteRenderComponent";

		SimpleSpriteRenderComponent::SimpleSpriteRenderComponent(GameObject* gameObject, RendererMode mode, int layer, glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, color), Component(gameObject)
		{
		}

		SimpleSpriteRenderComponent::SimpleSpriteRenderComponent(GameObject* gameObject, RendererMode mode, const Texture2D& texture, int layer, glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, texture, color), Component(gameObject)
		{
		}

		const std::string& SimpleSpriteRenderComponent::GetId() const
		{
			return SimpleSpriteRenderComponent::id;
		}

		SimpleSpriteRenderer* SimpleSpriteRenderComponent::GetInitedRenderer()
		{
			auto& renderer = SimpleSpriteRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void SimpleSpriteRenderComponent::DestroyRenderer()
		{
			SimpleSpriteRenderer::GetInstance().Destroy();
		}
	}
}