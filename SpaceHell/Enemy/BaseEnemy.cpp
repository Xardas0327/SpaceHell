#include "BaseEnemy.h"

#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/System/GameObjectManager.h>

#include "../Buff/BuffSpawner.h"

using namespace Learning2DEngine::System;

BaseEnemy::BaseEnemy(GameObject* gameObject, glm::vec2 direction, float speed, int life, int buffSpawnPercentage, int point)
	: UpdaterComponent(gameObject), Component(gameObject),
	direction(direction), speed(speed), life(life), buffSpawnPercentage(buffSpawnPercentage), point(point)
{
}

void BaseEnemy::Destroy()
{
	onDestroy.Invoke(this);
	onKilled.Clear();
	onDestroy.Clear();
	UpdaterComponent::Destroy();
}

void BaseEnemy::Move()
{
	gameObject->transform.SetPosition(gameObject->transform.GetPosition() + direction * speed * Time::GetDeltaTime());
}

void BaseEnemy::Hit(int damage)
{
	life -= damage;
	if (life <= 0)
	{
		onKilled.Invoke(point);
		BuffSpawner::GetInstance().SpawnBuff(gameObject->transform.GetPosition(), buffSpawnPercentage);
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
	}
}

void BaseEnemy::Kill()
{
	GameObjectManager::GetInstance().DestroyGameObject(gameObject);
}