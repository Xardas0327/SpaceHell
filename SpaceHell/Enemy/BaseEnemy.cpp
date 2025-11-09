#include "BaseEnemy.h"

#include <Learning2DEngine/System/GameObjectManager.h>

using namespace Learning2DEngine::System;

BaseEnemy::BaseEnemy(GameObject* gameObject, glm::vec2 direction, float speed, int life)
	: UpdaterComponent(gameObject), Component(gameObject),
	direction(direction), speed(speed), life(life)
{
}

void BaseEnemy::Hit(int damage)
{
	life -= damage;
	if (life <= 0)
	{
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
	}
}