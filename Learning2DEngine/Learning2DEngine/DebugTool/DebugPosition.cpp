#include "DebugPosition.h"

#include <sstream>
#include <iomanip>

#include "../System/GameObjectManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace UI;

	namespace DebugTool
	{
		UI::FontSizePair DebugPosition::fontSizePair;
		bool DebugPosition::isInited = false;
		glm::vec2 DebugPosition::defaultScale = glm::vec2(1.0f, 1.0f);
		float DebugPosition::defaultPadding = L2DE_DEBUG_SHOW_POSITION_BOX_PADDING;

		DebugPosition::DebugPosition(GameObject* gameObject)
			:LateUpdaterComponent(gameObject), Component(gameObject),
			isFirstUpdate(true), lastPosition(),
			textComponent(nullptr), boxComponent(nullptr)
		{

		}

		void DebugPosition::Init(const UI::FontSizePair& fontSizePair, glm::vec2 defaultScale, float defaultPadding)
		{
			DebugPosition::fontSizePair = fontSizePair;
			DebugPosition::defaultScale = defaultScale;
			DebugPosition::defaultPadding = defaultPadding;
			DebugPosition::isInited = true;
		}

		void DebugPosition::Init()
		{
			LateUpdaterComponent::Init();

			if (!DebugPosition::isInited)
				return;

			auto& gameObjectManager = GameObjectManager::GetInstance();

			auto textGo = gameObjectManager.CreateGameObject(Transform(glm::vec2(0.0f, 0.0f), DebugPosition::defaultScale));
			textComponent = textGo->AddComponent<Text2DRenderComponent>(
				RendererMode::RENDER,
				DebugPosition::fontSizePair,
				L2DE_DEBUG_SHOW_POSITION_DEFAULT_TEXT_LAYER,
				L2DE_DEBUG_SHOW_POSITION_TEXT_COLOR
			);
			textComponent->data.isUseCameraView = true;

			auto boxGo = gameObjectManager.CreateGameObject();
			boxComponent = boxGo->AddComponent<TextBoxComponent>(
				*textComponent,
				TextBoxMode::MULTI,
				L2DE_DEBUG_SHOW_POSITION_DEFAULT_BOX_LAYER,
				L2DE_DEBUG_SHOW_POSITION_BOX_COLOR
			);
			boxComponent->SetPadding(DebugPosition::defaultPadding);

			SetActive(L2DE_DEBUG_SHOW_POSITION_DEFAULT_VALUE);
		}

		void DebugPosition::Destroy()
		{
			LateUpdaterComponent::Destroy();

			auto& gameObjectManager = GameObjectManager::GetInstance();

			if (textComponent != nullptr)
				gameObjectManager.DestroyGameObject(textComponent);

			if (boxComponent != nullptr)
				gameObjectManager.DestroyGameObject(boxComponent);

		}

		void DebugPosition::LateUpdate()
		{
			if(isFirstUpdate || 
				gameObject->transform.GetPosition() != lastPosition)
			{
				isFirstUpdate = false;
				lastPosition = gameObject->transform.GetPosition();

				std::ostringstream oss;
				oss << std::fixed << std::setprecision(2);
				oss << "X: " << lastPosition.x << " Y: " << lastPosition.y;

				textComponent->data.SetText(oss.str());
				textComponent->gameObject->transform.SetPosition(lastPosition);
			}
		}

		void DebugPosition::SetActive(bool value)
		{
			isActive = value;
			textComponent->gameObject->isActive = value;
			boxComponent->gameObject->isActive = value;
		}
	}
}