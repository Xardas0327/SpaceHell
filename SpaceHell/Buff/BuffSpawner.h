#pragma once

#include <map>
#include <glm/glm.hpp>

#include <Learning2DEngine/System/Singleton.h>

#include "BaseBuff.h"

enum class BuffType
{
	Shield,
	Speed,
	Weapon
};

// -1 means no limit
const std::map<BuffType, int> BUFF_LIMITS = {
	{ BuffType::Shield, -1 },
	{ BuffType::Speed, 3 },
	{ BuffType::Weapon, 3 }
};


class BuffSpawner final : public Learning2DEngine::System::Singleton<BuffSpawner>
{
	friend class Learning2DEngine::System::Singleton<BuffSpawner>;
private:
	std::map<BuffType, int> limits;

	BuffSpawner();
public:
	void ResetLimits();

	//The percentage should be from 1 to 100
	//Returns nullptr if no buff is spawned
	BaseBuff* SpawnBuff(const glm::vec2& position, int percentage);
};