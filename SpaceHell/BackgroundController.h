#pragma once

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Render/SimpleSpriteRenderComponent.h>

constexpr float DEFAULT_BACKGROUND1_SPEED = 0.005f;
constexpr float DEFAULT_BACKGROUND2_SPEED = 0.001f;
constexpr float BACKGROUND1_SCREEN_RANGE = 0.05f;
constexpr float BACKGROUND2_SCREEN_RANGE = 0.05f;

class BackgroundController : protected Learning2DEngine::System::UpdaterComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Render::SimpleSpriteRenderComponent* background1;
	Learning2DEngine::Render::SimpleSpriteRenderComponent* background2;
	bool isRunning;

    BackgroundController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;
	void UpdateBackground(Learning2DEngine::Render::SimpleSpriteRenderComponent* background, float speed);
	void ResetBackgrounds();
public:
    float background1Speed;
	float background2Speed;

	~BackgroundController() = default;

    void Start(bool reset = false);
	void Stop();

	static BackgroundController* Create(const glm::vec2& scale);
};