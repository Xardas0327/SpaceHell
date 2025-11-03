#include "SimpleText2DRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace UI
	{
		const std::string SimpleText2DRenderComponent::id = "L2DE_SimpleText2DRenderComponent";

		SimpleText2DRenderComponent::SimpleText2DRenderComponent(
			GameObject* gameObject,
			RendererMode mode,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, fontSizePair, color),
			Component(gameObject)
		{
		}

		SimpleText2DRenderComponent::SimpleText2DRenderComponent(
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

		const std::string& SimpleText2DRenderComponent::GetId() const
		{
			return SimpleText2DRenderComponent::id;
		}

		SimpleText2DRenderer* SimpleText2DRenderComponent::GetInitedRenderer()
		{
			auto& renderer = SimpleText2DRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void SimpleText2DRenderComponent::DestroyRenderer()
		{
			SimpleText2DRenderer::GetInstance().Destroy();
		}
	}
}