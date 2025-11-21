#include "HeroController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>

#include "Bullet.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

HeroController::HeroController(GameObject* gameObject)
	: UpdaterComponent(gameObject), Component(gameObject),
	isArrived(false), isLeaving(false), direction(1.0f, 0.0f), onLeftMap()
{

}

void HeroController::Init()
{
	UpdaterComponent::Init();

	gameObject->transform.SetScale(HERO_SIZE);

	gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("Hero")
	);
}

void HeroController::Update()
{
	MoveToPoint();
	LeaveMap();
}

void HeroController::MoveToPoint()
{
	if (isArrived)
		return;

	glm::vec2 newPosition = gameObject->transform.GetPosition() + direction * HERO_SPEED * Time::GetDeltaTime();

	if (newPosition.x > HERO_POINT_X)
	{
		newPosition.x = HERO_POINT_X;
		isArrived = true;
		Shoot();
	}

	gameObject->transform.SetPosition(newPosition);
}

void HeroController::Shoot()
{
	auto bullet = Bullet::Create(
		gameObject->transform.GetPosition() 
			+ glm::vec2(gameObject->transform.GetScale().x - HERO_BULLET_SIZE.x, gameObject->transform.GetScale().y / 2.0f - HERO_BULLET_SIZE.y / 2.0f),
		HERO_BULLET_SIZE,
		0.0f,
		"HeroBullet",
		glm::vec2(1.0f, 0.0f),
		HERO_BULLET_SPEED,
		PLAYER_BULLET_MASK,
		HERO_BULLET_ANIMATION_NUMBER
	);
	bullet->isHeroBullet = true;
}

void HeroController::LeaveMap()
{
	if (!isLeaving)
		return;


	glm::vec2 newPosition = gameObject->transform.GetPosition() + direction * HERO_SPEED * Time::GetDeltaTime();

	if (newPosition.x > Game::mainCamera.GetResolution().GetWidth())
	{
		onLeftMap.Invoke();
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
	}
	else
		gameObject->transform.SetPosition(newPosition);
}

void HeroController::StartToLeaveMap()
{
	isLeaving = true;
}

HeroController* HeroController::Create()
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(
		Transform(
			glm::vec2(-HERO_SIZE.x - 10.0f, 200.0f),
			HERO_SIZE,
			90
		)
	);

	return go->AddComponent<HeroController>();
}