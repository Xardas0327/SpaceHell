#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>

class BaseEnemy : public Learning2DEngine::System::UpdaterComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
	glm::vec2 direction;
	float speed;
	int life;

	BaseEnemy(Learning2DEngine::System::GameObject* gameObject, glm::vec2 direction, float speed, int life);

public:
	void Hit(int damage);
};