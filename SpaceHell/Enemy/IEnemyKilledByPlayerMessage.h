#pragma once

class IEnemyKilledByPlayerMessage
{
public:
	virtual ~IEnemyKilledByPlayerMessage() = default;

	virtual void EnemyKilled(int point) = 0;
};