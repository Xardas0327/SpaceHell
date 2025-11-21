#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IBossMessage.h"

class BossDestroyedEventItem final : public Learning2DEngine::EventSystem::EventItem<>
{
private:
	IBossMessage* sender;
public:
	BossDestroyedEventItem(IBossMessage* sender)
		: sender(sender)
	{

	}

	void Call() override
	{
		sender->OnBossDestroyed();
	}
};