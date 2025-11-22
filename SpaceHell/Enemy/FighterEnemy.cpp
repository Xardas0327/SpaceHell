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
using namespace irrklang;

FighterEnemy::FighterEnemy(GameObject* gameObject, ISoundEngine* soundEngine)
	: BaseEnemy(gameObject, glm::vec2(0.0f, 1.0f), FIGHTER_SPEED, FIGHTER_LIFE, FIGHTER_BUFF_SPAWN, FIGHTER_POINT), Component(gameObject),
	canShoot(true), reloadTimer(0.0f), soundEngine(soundEngine)
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
				gameObject->transform.GetPosition() + glm::vec2(gameObject->transform.GetScale().x / 2.0f - FIGHTER_BULLET_SIZE.x / 2.0f, gameObject->transform.GetScale().y),
				FIGHTER_BULLET_SIZE,
				180.0f,
				"EnemyBullet",
				glm::vec2(0.0f, 1.0f),
				FIGHTER_BULLET_SPEED,
				ENEMY_BULLET_MASK,
				8,
				0.1f
			);
			if (soundEngine)
				soundEngine->play2D("Assets/Sounds/enemy_shoot.mp3");
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

FighterEnemy* FighterEnemy::Create(const glm::vec2& position, ISoundEngine* soundEngine)
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(Transform(position, FIGHTER_SIZE, 180.0f));

	return go->AddComponent<FighterEnemy>(soundEngine);
}