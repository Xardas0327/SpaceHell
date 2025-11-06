#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

constexpr glm::vec2 PLAYER_SIZE(96.0f, 96.0f);
constexpr float PLAYER_SPEED = 500.0f;
constexpr glm::vec2 PLAYER_SHIELD_OFFSET(0.0f, 5.0f);
constexpr int PLAYER_SHIELD_NUMBER = 12;

class PlayerController : protected Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	bool isFrozen;
	Learning2DEngine::Render::SpriteRenderComponent* shieldSprite;
    Learning2DEngine::Animator::AnimationController* shieldAnimation;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;

    void Move();
    void RefreshShieldPosition();
public:
    void Reset(glm::vec2 position);
	void SetFrozen(bool frozen);

	static PlayerController* Create(const glm::vec2& position);
};

