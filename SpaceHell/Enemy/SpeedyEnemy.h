#pragma once

#include "BaseEnemy.h"

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

constexpr glm::vec2 SPEEDY_SIZE(64.0f, 64.0f);
constexpr float SPEEDY_SPEED = 425.0f;
constexpr int SPEEDY_LIFE = 2;
constexpr int SPEEDY_BUFF_SPAWN = 50;
constexpr int SPEEDY_SHIELD_NUMBER = 10;
constexpr glm::vec2 SPEEDY_SHIELD_OFFSET(-16.0f, -10.0f);
constexpr glm::vec2 SPEEDY_SHIELD_SIZE(96.0f, 96.0f);

class SpeedyEnemy : public BaseEnemy
{
	friend class Learning2DEngine::System::GameObject;
protected:
	Learning2DEngine::Render::SpriteRenderComponent* shieldSprite;
	Learning2DEngine::Animator::AnimationController* shieldAnimation;

	SpeedyEnemy(Learning2DEngine::System::GameObject* gameObject);

	void Init() override;
	void Update() override;

	bool CheckOutOfScreen() override;
	void RefreshShieldPosition();

public:
	void Hit(int damage = 1) override;
	void Kill() override;

	static SpeedyEnemy* Create(const glm::vec2& position);
};
