#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

constexpr glm::vec2 PLAYER_SIZE(64.0f, 64.0f);
constexpr float PLAYER_SPEED = 300.0f;
constexpr glm::vec2 PLAYER_SHIELD_SIZE(96.0f, 96.0f);
constexpr glm::vec2 PLAYER_SHIELD_OFFSET(-16.0f, -10.0f);
constexpr int PLAYER_SHIELD_NUMBER = 12;
constexpr int PLAYER_BULLET_SPEED = 500;
constexpr float PLAYER_RELOAD = 1.0f;
constexpr int PLAYER_BULLET_DEFAULT_NUMBER = 1;

class PlayerController : protected Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Render::SpriteRenderComponent* shieldSprite;
    Learning2DEngine::Animator::AnimationController* shieldAnimation;
	bool isFrozen;
	float reloadTimer;
	int bulletNumber;
    int maxBulletNumber;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;

    void CheckKeyboard();
    void RefreshShieldPosition();
    void Reload();
    void Shoot();
public:
    void Reset(glm::vec2 position);
	void SetFrozen(bool frozen);

	static PlayerController* Create(const glm::vec2& position);
};

