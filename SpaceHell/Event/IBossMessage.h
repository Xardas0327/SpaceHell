#pragma once

class IBossMessage
{
public:
	virtual ~IBossMessage() = default;

	virtual void OnBossArrived() = 0;
	virtual void OnBossDestroyed() = 0;
};