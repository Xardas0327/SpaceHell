#include "BackgroundController.h"


#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

BackgroundController::BackgroundController(GameObject* gameObject)
	: UpdaterComponent(gameObject), Component(gameObject),
	background1(nullptr), background2(nullptr), isRunning(false),
	background1Speed(DEFAULT_BACKGROUND1_SPEED), background2Speed(DEFAULT_BACKGROUND2_SPEED)
{
}
void BackgroundController::Init()
{
	UpdaterComponent::Init();
	background1 = this->gameObject->AddComponent<SimpleSpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("Background1"),
		-10
	);

	background2 = this->gameObject->AddComponent<SimpleSpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("Background2"),
		-11
	);

	ResetBackgrounds();
}

void BackgroundController::Update()
{
	if (isRunning)
	{
		UpdateBackground(background1, background1Speed);
		UpdateBackground(background2, background2Speed);
	}
}

void BackgroundController::UpdateBackground(SimpleSpriteRenderComponent* background, float speed)
{
	//Reset the texture coordinates if they are too big.
	float fixer = background->data.uvMatrix[0][0] > 1.0f ? 1.0f : 0.0f;

	const float deltaMove = speed * Time::GetDeltaTime() - fixer;

	background->data.uvMatrix[0][0] += deltaMove;
	background->data.uvMatrix[1][0] += deltaMove;
	background->data.uvMatrix[2][0] += deltaMove;
	background->data.uvMatrix[3][0] += deltaMove;
}

void BackgroundController::ResetBackgrounds()
{
	// They are roated with 90 degrees.

	background1->data.uvMatrix = glm::mat4x2{
		BACKGROUND1_SCREEN_RANGE, 0.0f,
		BACKGROUND1_SCREEN_RANGE, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
	};

	background2->data.uvMatrix = glm::mat4x2{
		BACKGROUND2_SCREEN_RANGE, 0.0f,
		BACKGROUND2_SCREEN_RANGE, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
	};
}

void BackgroundController::Start(bool reset)
{
	if (reset)
		ResetBackgrounds();

	isRunning = true;
}

void BackgroundController::Stop()
{
	isRunning = false;
}