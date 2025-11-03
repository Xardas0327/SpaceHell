#pragma once

#include <glm/glm.hpp>
#include <string>
#include <mutex>

#include "../System/GameObject.h"
#include "RendererComponent.h"
#include "MultiSpriteRenderer.h"
#include "SpriteRenderData.h"
#include "Texture2D.h"

namespace Learning2DEngine
{
	namespace Render
	{
		//It supports the multi instance rendering
		class SpriteRenderComponent : public RendererComponent<SpriteRenderData, MultiSpriteRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;

		protected:
			SpriteRenderComponent(System::GameObject* gameObject, RendererMode mode, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
			SpriteRenderComponent(System::GameObject* gameObject, RendererMode mode, const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));

			const std::string& GetId() const override;
			MultiSpriteRenderer* GetInitedRenderer() override;
			void DestroyRenderer() override;
		};
	}
}