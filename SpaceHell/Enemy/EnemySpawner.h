#pragma once

#include <vector>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>

#include "EnemyDestroyEventItem.h"
#include "EnemyKilledByPlayerEvenItem.h"
#include "EnemySpawnerItem.h"
#include "BaseEnemy.h"

#include "FighterEnemy.h"
#include "ScoutEnemy.h"
#include "SpeedyEnemy.h"
#include "DiskEnemy.h"


class EnemySpawner final : public Learning2DEngine::System::UpdaterComponent,
	public IEnemyKilledByPlayerMessage, public IEnemyDestroyMessage
{
	friend class Learning2DEngine::System::GameObject;
protected:
	std::vector<EnemySpawnerItem> enemies;
	std::vector<BaseEnemy*> spawnedEnemy;
	bool isRunning;
	size_t currentEnemyIndex;
	float timer;
	int waveEnemyNumber;
	EnemyDestroyEventItem enemyDestroyEventItem;
	EnemyKilledByPlayerEvenItem enemyKilledByPlayerEvenItem;

	EnemySpawner(Learning2DEngine::System::GameObject* gameObject);

	void Update() override;
	void Spawn();

	void EnemyDestroyed(BaseEnemy* enemy) override;
	void EnemyKilled(int point) override;

public:
	~EnemySpawner() = default;

	void StartSpawning();
	void StopSpawning();
	void ClearSpawnedEnemies();

	//It works only when the spawner is stopped, otherwise it does nothing
	void SetEnemies(const std::vector<EnemySpawnerItem>& enemiesToSpawn);
};

