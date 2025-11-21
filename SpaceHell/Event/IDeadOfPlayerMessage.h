#pragma once

class IDeadOfPlayerMessage
{
public:
	virtual ~IDeadOfPlayerMessage() = default;

	virtual void DeadOfPlayer() = 0;
};
