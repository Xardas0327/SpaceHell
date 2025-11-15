#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IEnemyDestroyMessage.h"


class EnemyDestroyEventItem final : public Learning2DEngine::EventSystem::EventItem<BaseEnemy*>
{
private:
	IEnemyDestroyMessage* sender;
public:
	EnemyDestroyEventItem(IEnemyDestroyMessage* sender)
		: sender(sender)
	{

	}

	void Call(BaseEnemy* enemy) override
	{
		sender->EnemyDestroyed(enemy);
	}
};