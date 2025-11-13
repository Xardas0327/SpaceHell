#include "DiskEnemy.h"

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;


DiskEnemy::DiskEnemy(GameObject* gameObject, float loadTime)
	: BaseEnemy(
		gameObject,
		//It start to move to closer side.
		(Game::mainCamera.GetResolution().GetWidth() - gameObject->transform.GetPosition().x + DISK_SIZE.x / 2.0f) > Game::mainCamera.GetResolution().GetWidth() / 2.0f
			? DISK_MOVE_LEFT : DISK_MOVE_RIGHT,
		DISK_SPEED,
		DISK_LIFE,
		DISK_BUFF_SPAWN
	), Component(gameObject), loadTime(loadTime)
{
}

void DiskEnemy::Init()
{
	BaseEnemy::Init();

	auto& texture = ResourceManager::GetInstance().GetTexture("DiskEnemy");
	auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		texture
	);

	gameObject->AddComponent<CircleColliderComponent>(
		DISK_SIZE.x / 2.0f,
		ColliderType::KINEMATIC,
		ColliderMode::TRIGGER,
		glm::vec2(0.0f, 0.0f),
		ENEMY_COLLER_MASK
	);

	auto animation = gameObject->AddComponent<AnimationController>(&renderer->data, DISK_ANIMATION_NUMBER, true);
	for (int i = DISK_ANIMATION_NUMBER - 1; i >= 0; --i)
	{
		auto uvMatrix = glm::mat4x2{
			i / static_cast<float>(DISK_ANIMATION_NUMBER), 0.0f,
			(i + 1.0f) / static_cast<float>(DISK_ANIMATION_NUMBER), 0.0f,
			(i + 1.0f) / static_cast<float>(DISK_ANIMATION_NUMBER), 1.0f,
			i / static_cast<float>(DISK_ANIMATION_NUMBER), 1.0f
		};
		animation->Add(AnimationFrame{
			&texture,
			uvMatrix,
			0.2f
			});
	}
	animation->Play();
}

void DiskEnemy::Update()
{
	if(Load())
		Move();

	if (!CheckOutOfScreen())
	{
		CheckDirection();
		Rotate();
	}
}

bool DiskEnemy::CheckOutOfScreen()
{
	if (gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() + 10.0f)
	{
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
		return true;
	}
	return false;
}

void DiskEnemy::CheckDirection()
{
	if (direction == DISK_MOVE_LEFT && gameObject->transform.GetPosition().x < 0)
	{
		direction = DISK_MOVE_RIGHT;
	}
	else if (direction == DISK_MOVE_RIGHT && gameObject->transform.GetPosition().x + DISK_SIZE.x > Game::mainCamera.GetResolution().GetWidth())
	{
		direction = DISK_MOVE_LEFT;
	}
}

void DiskEnemy::Rotate()
{
	float newRotation = gameObject->transform.GetRotation() + 360.0f * Time::GetDeltaTime();
	if (newRotation >= 360.0f)
		newRotation -= 360.0f;

	gameObject->transform.SetRotation(newRotation);
}

bool DiskEnemy::Load()
{
	if(loadTime > 0.0f)
	{
		loadTime -= Time::GetDeltaTime();
		if (loadTime <= 0.0f)
			return true;

		return false;
	}

	return true;
}

DiskEnemy* DiskEnemy::Create(const glm::vec2& position, float loadTime)
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(Transform(position, DISK_SIZE));

	return go->AddComponent<DiskEnemy>(loadTime);
}