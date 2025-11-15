#pragma once

#include "BaseEnemy.h"

#include <Learning2DEngine/System/GameObject.h>

constexpr glm::vec2 FIGHTER_SIZE(64.0f, 64.0f);
constexpr float FIGHTER_SPEED = 100.0f;
constexpr int FIGHTER_LIFE = 1;
constexpr int FIGHTER_BUFF_SPAWN = 15;
constexpr float FIGHTER_BULLET_RELOAD = 1.0f; // it will used even the enemy didn't shoot
constexpr glm::vec2 FIGHTER_BULLET_SIZE(24.0f, 24.0f);
constexpr int FIGHTER_BULLET_SPEED = 500;
constexpr int FIGHTER_BULLET_PERCENTAGE = 30;
constexpr int FIGHTER_POINT = 10;

class FighterEnemy : public BaseEnemy
{
	friend class Learning2DEngine::System::GameObject;
protected:
	bool canShoot;
	float reloadTimer;

	FighterEnemy(Learning2DEngine::System::GameObject* gameObject);

	void Init() override;
	void Update() override;

	bool CheckOutOfScreen() override;
	void Shoot();
	void Reload();

public:
	static FighterEnemy* Create(const glm::vec2& position);
};

