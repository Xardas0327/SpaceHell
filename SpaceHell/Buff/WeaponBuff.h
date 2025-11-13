#pragma once

#include "BaseBuff.h"

constexpr float WEAPON_BUFF_LIFE_TIME = 10.0f;

class WeaponBuff : public BaseBuff
{
	friend class Learning2DEngine::System::GameObject;
protected:
	WeaponBuff(Learning2DEngine::System::GameObject* gameObject)
		: BaseBuff(gameObject, "WeaponBuff", BuffType::Weapon, WEAPON_BUFF_LIFE_TIME, 10), Learning2DEngine::System::Component(gameObject)
	{

	}

	void BuffPlayer(PlayerController* player) override
	{
		player->IncreaseMaxBulletNumber(1);
	}

public:
	static WeaponBuff* Create(const glm::vec2& position)
	{
		auto go = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position)
		);

		return go->AddComponent<WeaponBuff>();
	}
};