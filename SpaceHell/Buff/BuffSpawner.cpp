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

void BuffSpawner::ResetLimits()
{
	limits = BUFF_LIMITS;
}

BaseBuff* BuffSpawner::SpawnBuff(const glm::vec2& position, int percentage)
{
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

	if(limits[availableBuffs[index]] > 0)
		--limits[availableBuffs[index]];

	switch (availableBuffs[index])
	{
	case BuffType::Shield:
		return ShieldBuff::Create(position);
	case BuffType::Speed:
		return SpeedBuff::Create(position);
	case BuffType::Weapon:
		return WeaponBuff::Create(position);
	default:
		break;
	}
}