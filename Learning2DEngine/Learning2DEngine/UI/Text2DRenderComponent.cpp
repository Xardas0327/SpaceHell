#include "Text2DRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

    namespace UI
    {
		const std::string Text2DRenderComponent::id = "L2DE_Text2DRenderComponent";

		Text2DRenderComponent::Text2DRenderComponent(
			GameObject* gameObject,
			RendererMode mode,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, fontSizePair, color),
			Component(gameObject)
		{
		}

		Text2DRenderComponent::Text2DRenderComponent(
			GameObject* gameObject,
			RendererMode mode,
			const FontSizePair& fontSizePair,
			const std::string& text,
			int layer,
			glm::vec4 color
		) : RendererComponent(gameObject, mode, layer, fontSizePair, text, color),
			Component(gameObject)
		{

		}

		const std::string& Text2DRenderComponent::GetId() const
		{
			return Text2DRenderComponent::id;
		}

		MultiText2DRenderer* Text2DRenderComponent::GetInitedRenderer()
		{
			auto& renderer = MultiText2DRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void Text2DRenderComponent::DestroyRenderer()
		{
			MultiText2DRenderer::GetInstance().Destroy();
		}
    }
}