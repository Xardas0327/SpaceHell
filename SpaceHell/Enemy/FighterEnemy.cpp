#include "FighterEnemy.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

#include "../Bullet.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

FighterEnemy::FighterEnemy(GameObject* gameObject)
	: BaseEnemy(gameObject, glm::vec2(0.0f, 1.0f), FIGHTER_SPEED, FIGHTER_LIFE, FIGHTER_BUFF_SPAWN), Component(gameObject),
	canShoot(true), reloadTimer(0.0f)
{
}

void FighterEnemy::Init()
{
	BaseEnemy::Init();

	gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("FighterEnemy")
	);

	gameObject->AddComponent<CircleColliderComponent>(
		FIGHTER_SIZE.x / 2.0f,
		ColliderType::KINEMATIC,
		ColliderMode::TRIGGER,
		glm::vec2(0.0f, 0.0f),
		ENEMY_COLLER_MASK
	);
}

void FighterEnemy::Update()
{
	Move();
	Reload();
	if (!CheckOutOfScreen())
		Shoot();
}

bool FighterEnemy::CheckOutOfScreen()
{
	if (gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() + 5.0f)
	{
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
		return true;
	}
	return false;
}

void FighterEnemy::Shoot()
{
	if (canShoot)
	{
		canShoot = false;
		if (Random::GetNumber(0, 100) < FIGHTER_BULLET_PERCENTAGE)
		{
			Bullet::Create(
				gameObject->transform.GetPosition() + glm::vec2(gameObject->transform.GetScale().x / 2.0f - BULLET_SIZE.x / 2.0f, gameObject->transform.GetScale().y),
				glm::vec2(0.0f, 1.0f),
				FIGHTER_BULLET_SPEED,
				ENEMY_BULLET_MASK
			);
		}
	}
}

void FighterEnemy::Reload()
{
	if (!canShoot)
	{
		reloadTimer += Time::GetDeltaTime();
		if (reloadTimer >= FIGHTER_BULLET_RELOAD)
		{
			canShoot = true;
			reloadTimer = 0.0f;
		}
	}
}

FighterEnemy* FighterEnemy::Create(const glm::vec2& position)
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(Transform(position, FIGHTER_SIZE, 180.0f));

	return go->AddComponent<FighterEnemy>();
}