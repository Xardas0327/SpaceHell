#include "ScoutEnemy.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

ScoutEnemy::ScoutEnemy(GameObject* gameObject, ScoutState state)
	: BaseEnemy(
		gameObject,
		glm::vec2(state == ScoutState::MOVING_RIGHT ? 1.0f : -1.0f, 0.0f),
		SCOUT_SPEED,
		SCOUT_LIFE,
		SCOUT_BUFF_SPAWN
	),
	Component(gameObject)
{
}

void ScoutEnemy::Init()
{
	BaseEnemy::Init();

	gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("EnemyScout")
	);

	gameObject->AddComponent<CircleColliderComponent>(
		SCOUT_SIZE.x / 2.0f,
		ColliderType::KINEMATIC,
		ColliderMode::TRIGGER,
		glm::vec2(0.0f, 0.0f),
		ENEMY_COLLER_MASK
	);
}

void ScoutEnemy::Update()
{
	Move();
	CheckOutOfScreen();
}

bool ScoutEnemy::CheckOutOfScreen()
{
	if (gameObject->transform.GetPosition().x > Game::mainCamera.GetResolution().GetWidth() + 10.0f
		|| gameObject->transform.GetPosition().x < -SCOUT_SIZE.x - 10.0f)
	{
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
		return true;
	}
	return false;
}

ScoutEnemy* ScoutEnemy::Create(const glm::vec2& position, ScoutState state)
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(
		Transform(
			position,
			SCOUT_SIZE,
			state == ScoutState::MOVING_RIGHT ? 90.0f : -90.0f
		)
	);

	return go->AddComponent<ScoutEnemy>(state);
}