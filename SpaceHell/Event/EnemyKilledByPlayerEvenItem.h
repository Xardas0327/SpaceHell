#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IEnemyKilledByPlayerMessage.h"


class EnemyKilledByPlayerEvenItem final : public Learning2DEngine::EventSystem::EventItem<int>
{
private:
	IEnemyKilledByPlayerMessage* sender;
public:
	EnemyKilledByPlayerEvenItem(IEnemyKilledByPlayerMessage* sender)
		: sender(sender)
	{

	}

	void Call(int point) override
	{
		sender->EnemyKilled(point);
	}
};