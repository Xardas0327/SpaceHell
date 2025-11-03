#pragma once

#include "DebugMacro.h"
#include "../System/GameObject.h"
#include "../System/LateUpdaterComponent.h"
#include "../UI/Text2DRenderComponent.h"
#include "../UI/TextBoxComponent.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		// It is used in Render Mode only.
		// The DebugPosition will be updated in a LateUpdate().
		// So, it can work well only if the position of the GameObject is updated in Update() function(s).
		// Moreover, it works well with renderers if the renderers use the camera view matrix.
		class DebugPosition : public System::LateUpdaterComponent
		{
			friend class System::GameObject;
		private:
			UI::Text2DRenderComponent* textComponent;
			UI::TextBoxComponent* boxComponent;
			bool isFirstUpdate;
			glm::vec2 lastPosition;

			static UI::FontSizePair fontSizePair;
			static bool isInited;
			static glm::vec2 defaultScale;
			static float defaultPadding;

			DebugPosition(System::GameObject* gameObject);

			void Init() override;
			void Destroy() override;
			void LateUpdate() override;
		public:

			static void Init(
				const UI::FontSizePair& fontSizePair,
				glm::vec2 defaultScale = glm::vec2(1.0f, 1.0f),
				float defaultPadding = L2DE_DEBUG_SHOW_POSITION_BOX_PADDING
			);

			void SetActive(bool value);

			inline void SetTextLayer(int value)
			{
				textComponent->SetLayer(value);
			}

			inline int GetTextLayer() const
			{
				return textComponent->GetLayer();
			}

			inline void SetBoxLayer(int value)
			{
				boxComponent->SetLayer(value);
			}

			inline int GetBoxLayer() const
			{
				return boxComponent->GetLayer();
			}

			inline void SetTextColor(glm::vec4 color)
			{
				textComponent->data.color = color;
			}

			inline glm::vec4 GetTextColor() const
			{
				return textComponent->data.color;
			}

			inline void SetBoxColor(glm::vec4 color)
			{
				boxComponent->SetColor(color);
			}

			inline glm::vec4 GetBoxColor() const
			{
				return boxComponent->GetColor();
			}

			inline void SetTextScale(glm::vec2 scale)
			{
				textComponent->gameObject->transform.SetScale(scale);
			}

			inline glm::vec2 GetTextScale() const
			{
				return textComponent->gameObject->transform.GetScale();
			}

			inline void SetBoxPadding(float padding)
			{
				boxComponent->SetPadding(padding);
			}

			inline void SetBoxLeftRightPadding(float padding)
			{
				boxComponent->paddingLeftRight = padding;
			}

			inline float GetBoxLeftRightPadding() const
			{
				return boxComponent->paddingLeftRight;
			}

			inline void SetBoxTopBottomPadding(float padding)
			{
				boxComponent->paddingTopBottom = padding;
			}

			inline float GetBoxTopBottomPadding() const
			{
				return boxComponent->paddingTopBottom;
			}
		};
	}
}