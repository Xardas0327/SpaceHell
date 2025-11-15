#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IEndOfEnemyWaveMessage.h"


class EndOfEnemyWaveEventItem final : public Learning2DEngine::EventSystem::EventItem<>
{
private:
	IEndOfEnemyWaveMessage* sender;
public:
	EndOfEnemyWaveEventItem(IEndOfEnemyWaveMessage* sender)
		: sender(sender)
	{

	}

	void Call() override
	{
		sender->EndOfWave();
	}
};