#include "BuffSpawner.h"

#include <vector>
#include <Learning2DEngine/System/Random.h>

#include "ShieldBuff.h"
#include "SpeedBuff.h"
#include "WeaponBuff.h"

using namespace Learning2DEngine::System;

BuffSpawner::BuffSpawner()
	: limits(BUFF_LIMITS)
{
}

void BuffSpawner::Call(BaseBuff* usedBuff)
{
	if (limits[usedBuff->GetBuffType()] > 0)
		--limits[usedBuff->GetBuffType()];
}

void BuffSpawner::ResetLimits()
{
	limits = BUFF_LIMITS;
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

	switch (availableBuffs[index])
	{
	case BuffType::Shield:
		buff = ShieldBuff::Create(position);
		break;
	case BuffType::Speed:
		buff = SpeedBuff::Create(position);
		break;
	case BuffType::Weapon:
		buff = WeaponBuff::Create(position);
		break;
	default:
		break;
	}

	if(buff != nullptr)
		buff->Activated.Add(this);

	return buff;
}