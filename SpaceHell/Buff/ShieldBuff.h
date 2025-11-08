#pragma once

#include "BaseBuff.h"

constexpr float SHIELD_BUFF_LIFE_TIME = 10.0f;

class ShieldBuff : public BaseBuff
{
	friend class Learning2DEngine::System::GameObject;
protected:
	ShieldBuff(Learning2DEngine::System::GameObject* gameObject)
		: BaseBuff(gameObject, "ShieldBuff", SHIELD_BUFF_LIFE_TIME, 15), Learning2DEngine::System::Component(gameObject)
	{

	}

	void BuffPlayer(PlayerController* player) override
	{
		player->ResetLife();
	}

public:
	static ShieldBuff* Create(const glm::vec2& position)
	{
		auto go = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position)
		);

		return go->AddComponent<ShieldBuff>();
	}
};