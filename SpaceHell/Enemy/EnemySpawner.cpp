#include "EnemySpawner.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>

using namespace Learning2DEngine::System;
using namespace irrklang;

EnemySpawner::EnemySpawner(GameObject* gameObject)
	: UpdaterComponent(gameObject), Component(gameObject),
	enemies(), spawnedEnemy(), isRunning(false), currentEnemyIndex(0), timer(0.0f), waveEnemyNumber(0),
	enemyDestroyEventItem(this), enemyKilledByPlayerEvenItem(this), soundEngine(nullptr)
{
}

void EnemySpawner::Update()
{
	if (!isRunning)
		return;

	if (currentEnemyIndex >= enemies.size())
	{
		StopSpawning();
		return;
	}

	timer += Time::GetDeltaTime();
	while (currentEnemyIndex < enemies.size() && timer >= enemies[currentEnemyIndex].delay)
	{
		Spawn();
		timer -= enemies[currentEnemyIndex].delay;
		++currentEnemyIndex;
	}
}

void EnemySpawner::Spawn()
{
	if (currentEnemyIndex >= enemies.size())
		return;

	BaseEnemy* enemy = nullptr;
	switch (enemies[currentEnemyIndex].type)
	{
	case EnemySpawnerItemType::Fighter:
		enemy = FighterEnemy::Create(enemies[currentEnemyIndex].position, soundEngine);
		break;
	case EnemySpawnerItemType::ScoutLeft:
		enemy = ScoutEnemy::Create(enemies[currentEnemyIndex].position, ScoutState::MOVING_LEFT);
		break;
	case EnemySpawnerItemType::ScoutRight:
		enemy = ScoutEnemy::Create(enemies[currentEnemyIndex].position, ScoutState::MOVING_RIGHT);
		break;
	case EnemySpawnerItemType::Speedy:
		enemy = SpeedyEnemy::Create(enemies[currentEnemyIndex].position);
		break;
	case EnemySpawnerItemType::Disk:
		enemy = DiskEnemy::Create(enemies[currentEnemyIndex].position);
		break;
	default:
		break;
	}
	if (enemy != nullptr)
	{
		spawnedEnemy.push_back(enemy);
		enemy->onDestroy.Add(&enemyDestroyEventItem);
		enemy->onKilled.Add(&enemyKilledByPlayerEvenItem);
	}
}

void EnemySpawner::EnemyDestroyed(BaseEnemy* enemy)
{
	auto it = std::find(spawnedEnemy.begin(), spawnedEnemy.end(), enemy);
	if (it != spawnedEnemy.end())
	{
		spawnedEnemy.erase(it);
		--waveEnemyNumber;
		if (waveEnemyNumber <= 0)
		{
			destroyedAllEnemies.Invoke();
		}
	}
}

void EnemySpawner::EnemyKilled(int point)
{
	refreshScore.Invoke(point);
}

void EnemySpawner::StartSpawning()
{
	if (enemies.size() > 0)
	{
		isRunning = true;
		currentEnemyIndex = 0;
		timer = 0.0f;
		waveEnemyNumber = static_cast<int>(enemies.size());
	}
}

void EnemySpawner::StopSpawning()
{
	isRunning = false;
}

void EnemySpawner::ClearSpawnedEnemies()
{
	for (BaseEnemy* enemy : spawnedEnemy)
	{
		enemy->onDestroy.Remove(&enemyDestroyEventItem);
		enemy->onKilled.Remove(&enemyKilledByPlayerEvenItem);
		GameObjectManager::GetInstance().DestroyGameObject(enemy);
	}
	spawnedEnemy.clear();
}

void EnemySpawner::SetEnemies(const std::vector<EnemySpawnerItem>& enemiesToSpawn)
{
	if (isRunning)
		return;

	enemies = enemiesToSpawn;
}