#pragma once

#include "../System/GameObject.h"
#include "../System/LateUpdaterComponent.h"
#include "../Render/SimpleSpriteRenderComponent.h"
#include "../Render/SpriteRenderComponent.h"
#include "SimpleText2DRenderComponent.h"
#include "Text2DRenderComponent.h"
#include "TextBoxMode.h"

namespace Learning2DEngine
{
    namespace UI
    {
		/// <summary>
		/// The TextBoxComponent will create a new GameObject with a SimpleSpriteRenderComponent or with a SpriteRenderComponent.
		/// The TextBox will be updated in a LateUpdate(). So, the text should be updated in Update() function(s).
		/// </summary>
		class TextBoxComponent : public System::LateUpdaterComponent
		{
			friend class System::GameObject;
		protected:
			Text2DRenderData* const textRenderData;
			Render::SimpleSpriteRenderComponent* simpleRenderComponent;
			Render::SpriteRenderComponent* multiRenderComponent;
			const int initLayer;
			const glm::vec4 initColor;
			const Render::RendererMode initRendererMode;
			const TextBoxMode textBoxMode;

			TextBoxComponent(
				System::GameObject* gameObject,
				SimpleText2DRenderComponent& textComponent,
				TextBoxMode textBoxMode = TextBoxMode::SIMPLE,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			TextBoxComponent(
				System::GameObject* gameObject,
				Text2DRenderComponent& textComponent,
				TextBoxMode textBoxMode = TextBoxMode::SIMPLE,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;
			void LateUpdate() override;

		public:
			float paddingTopBottom = 0.0f;
			float paddingLeftRight= 0.0f;

			//It set the padding for all sides.
			void SetPadding(float padding);
			int GetLayer() const;
			void SetLayer(int layer);
			glm::vec4 GetColor() const;
			void SetColor(const glm::vec4& color);
		};
    }
}