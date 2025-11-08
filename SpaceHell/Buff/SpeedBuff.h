#pragma once

#include "BaseBuff.h"

constexpr float SPEED_BUFF_LIFE_TIME = 10.0f;

class SpeedBuff : public BaseBuff
{
	friend class Learning2DEngine::System::GameObject;
protected:
	SpeedBuff(Learning2DEngine::System::GameObject* gameObject)
		: BaseBuff(gameObject, "SpeedBuff", SPEED_BUFF_LIFE_TIME, 4), Learning2DEngine::System::Component(gameObject)
	{

	}

	void BuffPlayer(PlayerController* player) override
	{
		player->IncreaseSpeed(50.0f);
	}

public:
	static SpeedBuff* Create(const glm::vec2& position)
	{
		auto go = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position)
		);

		return go->AddComponent<SpeedBuff>();
	}
};