#pragma once

class IEndOfEnemyWaveMessage
{
public:
	virtual ~IEndOfEnemyWaveMessage() = default;

	virtual void EndOfWave() = 0;
};
