#pragma once

#include <glm/glm.hpp>

enum class EnemySpawnerItemType
{
	Fighter,
	ScoutLeft,
	ScoutRight,
	Speedy,
	Disk
};

struct EnemySpawnerItem
{
	EnemySpawnerItemType type;
	glm::vec2 position;
	float delay;
};