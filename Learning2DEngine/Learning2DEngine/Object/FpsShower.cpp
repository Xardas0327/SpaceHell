#include "FpsShower.h"

#include <GLFW/glfw3.h>
#include <string>

#include "../System/GameObjectManager.h"

namespace Learning2DEngine
{
	using namespace Render;
	using namespace System;
	using namespace UI;

	namespace Object
	{
		FpsShower::FpsShower(
			GameObject* gameObject,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec4 color)
			: UpdaterComponent(gameObject), Component(gameObject),
			SimpleText2DRenderComponent(gameObject, RendererMode::LATERENDER, fontSizePair, layer, color),
			previousTime(0.0f), fps(0)
		{

		}

		void FpsShower::Init()
		{
			UpdaterComponent::Init();
			SimpleText2DRenderComponent::Init();
			previousTime = static_cast<float>(glfwGetTime());
			data.SetText("FPS: " + std::to_string(fps));
		}

		void FpsShower::Destroy()
		{
			UpdaterComponent::Destroy();
			SimpleText2DRenderComponent::Destroy();
		}

		void FpsShower::Update()
		{
			++fps;
			// The Time::deltaTime is not good here, because it is multiplided with Time::timeScale
			float currentTime = static_cast<float>(glfwGetTime());
			if (currentTime - previousTime > 1.0f)
			{
				data.SetText("FPS: " + std::to_string(fps));
				fps = 0;
				previousTime = currentTime;
			}
		}

		FpsShower* FpsShower::CreateFpsShowerObject(
			const System::Transform& transform,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec4 color)
		{
			auto gameObject = GameObjectManager::GetInstance().CreateGameObject(transform);
			return gameObject->AddComponent<FpsShower>(fontSizePair, layer, color);
		}
	}
}