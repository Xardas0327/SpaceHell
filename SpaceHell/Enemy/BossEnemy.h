#pragma once

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

#include "BaseEnemy.h"

constexpr glm::vec2 BOSS_SIZE(384.0f, 384.0f);
constexpr float BOSS_SPEED = 100.0f;
constexpr glm::vec2 BOSS_COLLIDER_OFFSET(85.0f, 35.0f);
constexpr float ARRIVE_POINT_Y = 50.0f;
constexpr int BOSS_DESTRUCTION_NUMBER = 12;
constexpr float BOSS_ANIMATION_SPEED = 0.3f;

class BossEnemy : public BaseEnemy
{
	friend class Learning2DEngine::System::GameObject;
protected:
	Learning2DEngine::Physics::BoxColliderComponent* collider;
	Learning2DEngine::Animator::AnimationController* destructionAnimation;
	bool isArrived;
	bool isDying;
	float dyingTimer;

	BossEnemy(Learning2DEngine::System::GameObject* gameObject);

	void Init() override;
	void Update() override;
	bool CheckOutOfScreen() override;

	void MoveToPoint();
	void DieProcess();

public:
	void Hit(int damage = 1) override;
	void Kill() override;

	static BossEnemy* Create();
};

