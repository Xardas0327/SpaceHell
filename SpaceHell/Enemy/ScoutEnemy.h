#pragma once

#include "BaseEnemy.h"

#include <Learning2DEngine/System/GameObject.h>

constexpr glm::vec2 SCOUT_SIZE(64.0f, 64.0f);
constexpr float SCOUT_SPEED = 300.0f;
constexpr int SCOUT_LIFE = 1;
constexpr int SCOUT_BUFF_SPAWN = 65;
constexpr int SCOUT_POINT = 20;

enum class ScoutState
{
	MOVING_LEFT,
	MOVING_RIGHT
};

class ScoutEnemy : public BaseEnemy
{
	friend class Learning2DEngine::System::GameObject;
protected:
	ScoutEnemy(Learning2DEngine::System::GameObject* gameObject, ScoutState state);

	void Init() override;
	void Update() override;

	bool CheckOutOfScreen() override;

public:
	static ScoutEnemy* Create(const glm::vec2& position, ScoutState state);
};
