#pragma once

#include <Learning2DEngine/EventSystem/EventItem.h>

#include "IDeadOfPlayerMessage.h"


class DeadOfPlayerEventItem final : public Learning2DEngine::EventSystem::EventItem<>
{
private:
	IDeadOfPlayerMessage* sender;
public:
	DeadOfPlayerEventItem(IDeadOfPlayerMessage* sender)
		: sender(sender)
	{

	}

	void Call() override
	{
		sender->DeadOfPlayer();
	}
};