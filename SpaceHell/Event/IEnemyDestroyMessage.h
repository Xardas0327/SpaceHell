#pragma once

#include "../Enemy/BaseEnemy.h"

class IEnemyDestroyMessage
{
public:
	virtual ~IEnemyDestroyMessage() = default;

	virtual void EnemyDestroyed(BaseEnemy* enemy) = 0;
};