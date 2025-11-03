#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "../Render/RendererComponent.h"
#include "../Render/RendererMode.h"

#include "Text2DRenderData.h"
#include "SimpleText2DRenderer.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class SimpleText2DRenderComponent : public Render::RendererComponent<Text2DRenderData, SimpleText2DRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;

		protected:
			SimpleText2DRenderComponent(
				System::GameObject* gameObject,
				Render::RendererMode mode,
				const FontSizePair& fontSizePair,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			SimpleText2DRenderComponent(
				System::GameObject* gameObject,
				Render::RendererMode mode,
				const FontSizePair& fontSizePair,
				const std::string& text,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			const std::string& GetId() const override;
			SimpleText2DRenderer* GetInitedRenderer() override;
			void DestroyRenderer() override;
		};
	}
}