#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IBossMessage.h"

class BossArrivedEventItem final : public Learning2DEngine::EventSystem::EventItem<>
{
private:
	IBossMessage* sender;
public:
	BossArrivedEventItem(IBossMessage* sender)
		: sender(sender)
	{

	}

	void Call() override
	{
		sender->OnBossArrived();
	}
};