#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>


constexpr int32_t ENEMY_COLLER_MASK = 0B101;

class BaseEnemy : public Learning2DEngine::System::UpdaterComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
	glm::vec2 direction;
	float speed;
	int life;
	int buffSpawnPercentage;

	BaseEnemy(Learning2DEngine::System::GameObject* gameObject, glm::vec2 direction, float speed, int life, int buffSpawnPercentage);

	void Move();
	virtual bool CheckOutOfScreen() = 0;

public:
	virtual void Hit(int damage = 1);

	//It just destroy the enemy without spawn buff
	virtual void Kill();

	inline int GetLife() const
	{
		return life;
	}
};