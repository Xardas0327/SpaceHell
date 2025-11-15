#pragma once

#include "BaseEnemy.h"

class IEnemyDestroyMessage
{
public:
	virtual ~IEnemyDestroyMessage() = default;

	virtual void EnemyDestroyed(BaseEnemy* enemy) = 0;
};