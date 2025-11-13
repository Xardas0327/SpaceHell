#pragma once

#include <Learning2DEngine/System/GameObject.h>

#include "BaseEnemy.h"

constexpr glm::vec2 DISK_SIZE(80.0f, 80.0f);
constexpr float DISK_SPEED = 400.0f;
constexpr int DISK_LIFE = 2;
constexpr int DISK_BUFF_SPAWN = 50;
constexpr int DISK_ANIMATION_NUMBER = 8;
constexpr glm::vec2 DISK_MOVE_LEFT(-1.0f, 0.5f);
constexpr glm::vec2 DISK_MOVE_RIGHT(1.0f, 0.5f);

class DiskEnemy : public BaseEnemy
{
	friend class Learning2DEngine::System::GameObject;
protected:
	float loadTime;

	DiskEnemy(Learning2DEngine::System::GameObject* gameObject, float loadTime);

	void Init() override;
	void Update() override;

	bool CheckOutOfScreen() override;
	void CheckDirection();
	void Rotate();
	//Return true if load complete
	bool Load();

public:
	static DiskEnemy* Create(const glm::vec2& position, float loadTime = 2.0f);
};