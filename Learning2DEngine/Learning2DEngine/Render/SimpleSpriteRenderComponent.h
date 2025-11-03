#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "RendererComponent.h"
#include "SimpleSpriteRenderer.h"
#include "SpriteRenderData.h"
#include "Texture2D.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class SimpleSpriteRenderComponent : public RendererComponent<SpriteRenderData, SimpleSpriteRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;

		protected:
			SimpleSpriteRenderComponent(System::GameObject* gameObject, RendererMode mode, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
			SimpleSpriteRenderComponent(System::GameObject* gameObject, RendererMode mode, const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));

			const std::string& GetId() const override;
			SimpleSpriteRenderer* GetInitedRenderer() override;
			void DestroyRenderer() override;
		};
	}
}