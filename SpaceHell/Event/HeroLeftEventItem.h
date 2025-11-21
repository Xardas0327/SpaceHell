#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IHeroLeftMessage.h"


class HeroLeftEventItem final : public Learning2DEngine::EventSystem::EventItem<>
{
private:
	IHeroLeftMessage* sender;
public:
	HeroLeftEventItem(IHeroLeftMessage* sender)
		: sender(sender)
	{

	}

	void Call() override
	{
		sender->OnHeroLeft();
	}
};