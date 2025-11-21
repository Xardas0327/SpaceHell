#pragma once

class IHeroLeftMessage
{
public:
	virtual ~IHeroLeftMessage() = default;

	virtual void OnHeroLeft() = 0;
};
