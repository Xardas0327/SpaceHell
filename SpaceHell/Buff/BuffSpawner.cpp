#include "BuffSpawner.h"

#include <vector>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/GameObjectManager.h>

#include "ShieldBuff.h"
#include "SpeedBuff.h"
#include "WeaponBuff.h"

using namespace Learning2DEngine::System;

BuffSpawner::BuffSpawner()
	: limits(BUFF_LIMITS), activeBuffs()
{
}

void BuffSpawner::Call(BaseBuff* usedBuff, bool activated)
{
	if (activated && limits[usedBuff->GetBuffType()] > 0)
		--limits[usedBuff->GetBuffType()];

	auto it = std::find(activeBuffs.begin(), activeBuffs.end(), usedBuff);
	if (it != activeBuffs.end())
	{
		activeBuffs.erase(it);
	}
}

void BuffSpawner::ResetLimits()
{
	limits = BUFF_LIMITS;
}

void BuffSpawner::ClearActiveBuffs()
{
	auto& manager = GameObjectManager::GetInstance();
	for (BaseBuff* buff : activeBuffs)
	{
		manager.DestroyGameObject(buff);
	}
	activeBuffs.clear();
}

BaseBuff* BuffSpawner::SpawnBuff(const glm::vec2& position, int percentage)
{
	if(percentage <= 0)
		return nullptr;

	int number = Random::GetNumber(0, 100);
	if(number >= percentage)
		return nullptr;

	std::vector<BuffType> availableBuffs;
	availableBuffs.reserve(limits.size());

	for(const auto& pair : limits)
	{
		if(pair.second != 0)
			availableBuffs.push_back(pair.first);
	}

	if(availableBuffs.empty())
		return nullptr;

	int index = Random::GetNumber(0, static_cast<int>(availableBuffs.size()));

	BaseBuff* buff = nullptr;

	auto fixedPosition = position - BUFF_SIZE / 2.0f;

	switch (availableBuffs[index])
	{
	case BuffType::Shield:
		buff = ShieldBuff::Create(fixedPosition);
		break;
	case BuffType::Speed:
		buff = SpeedBuff::Create(fixedPosition);
		break;
	case BuffType::Weapon:
		buff = WeaponBuff::Create(fixedPosition);
		break;
	default:
		break;
	}

	if (buff != nullptr)
	{
		activeBuffs.push_back(buff);
		buff->disappeared.Add(this);
	}

	return buff;
}