#pragma once

#include <map>
#include <glm/glm.hpp>

#include <Learning2DEngine/System/Singleton.h>
#include <Learning2DEngine/EventSystem/EventItem.h>

#include "BaseBuff.h"
#include "BuffType.h"

// -1 means no limit
const std::map<BuffType, int> BUFF_LIMITS = {
	{ BuffType::Shield, -1 },
	{ BuffType::Speed, 3 },
	{ BuffType::Weapon, 3 }
};

class BuffSpawner final : public Learning2DEngine::System::Singleton<BuffSpawner>,
							public Learning2DEngine::EventSystem::EventItem<BuffType>
{
	friend class Learning2DEngine::System::Singleton<BuffSpawner>;
private:
	std::map<BuffType, int> limits;

	BuffSpawner();

	void Call(BuffType activatedItem) override;
public:
	~BuffSpawner() = default;

	void ResetLimits();

	//The percentage should be from 1 to 100
	//Returns nullptr if no buff is spawned
	BaseBuff* SpawnBuff(const glm::vec2& position, int percentage);
};