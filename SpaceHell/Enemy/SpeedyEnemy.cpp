#include "SpeedyEnemy.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

SpeedyEnemy::SpeedyEnemy(GameObject* gameObject)
	: BaseEnemy(gameObject, glm::vec2(0.0f, 1.0f), SPEEDY_SPEED, SPEEDY_LIFE, SPEEDY_BUFF_SPAWN), Component(gameObject),
	shieldSprite(nullptr), shieldAnimation(nullptr)
{
}

void SpeedyEnemy::Init()
{
	BaseEnemy::Init();

	gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("SpeedyEnemy")
	);

	gameObject->AddComponent<CircleColliderComponent>(
		SPEEDY_SIZE.x / 2.0f,
		ColliderType::KINEMATIC,
		ColliderMode::TRIGGER,
		glm::vec2(0.0f, 0.0f),
		ENEMY_COLLER_MASK
	);

	//Shield
	auto shieldGO = GameObjectManager::GetInstance().CreateGameObject(
		Transform(
			gameObject->transform.GetPosition() + SPEEDY_SHIELD_OFFSET,
			SPEEDY_SHIELD_SIZE,
			180.0f
		)
	);
	auto& shieldTexture = ResourceManager::GetInstance().GetTexture("SpeedyEnemyShield");
	shieldSprite = shieldGO->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		shieldTexture,
		1
	);

	shieldAnimation = shieldGO->AddComponent<AnimationController>(&shieldSprite->data, SPEEDY_SHIELD_NUMBER, true);
	for (int i = 0; i < SPEEDY_SHIELD_NUMBER; ++i)
	{
		auto uvMatrix = glm::mat4x2{
			i / static_cast<float>(SPEEDY_SHIELD_NUMBER), 0.0f,
			(i + 1.0f) / static_cast<float>(SPEEDY_SHIELD_NUMBER), 0.0f,
			(i + 1.0f) / static_cast<float>(SPEEDY_SHIELD_NUMBER), 1.0f,
			i / static_cast<float>(SPEEDY_SHIELD_NUMBER), 1.0f
		};
		shieldAnimation->Add(AnimationFrame{
			&shieldTexture,
			uvMatrix,
			0.25f
			});
	}
	shieldAnimation->Play();
}

void SpeedyEnemy::Update()
{
	Move();
	if (!CheckOutOfScreen())
		RefreshShieldPosition();
}

bool SpeedyEnemy::CheckOutOfScreen()
{
	// Change direction and reduce life when going out from the bottom
	if (direction == glm::vec2(0.0f, 1.0f) && gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() + 500.0f)
	{
		direction = glm::vec2(0.0f, -1.0f);
		if (life > 1)
		{
			Hit(life - 1);
		}
		gameObject->transform.SetRotation(0.0f);
		return false;
	}

	if (direction == glm::vec2(0.0f, -1.0f) && gameObject->transform.GetPosition().y < -SPEEDY_SIZE.y - 20.0f)
	{
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
		return true;
	}
	return false;
}

void SpeedyEnemy::RefreshShieldPosition()
{
	shieldSprite->gameObject->transform.SetPosition(
		gameObject->transform.GetPosition() + SPEEDY_SHIELD_OFFSET
	);
}

void SpeedyEnemy::Hit(int damage)
{
	BaseEnemy::Hit(damage);
	if(life < 2)
	{
		shieldSprite->isActive = false;
		shieldAnimation->isActive = false;
	}
}

void SpeedyEnemy::Kill()
{
	GameObjectManager::GetInstance().DestroyGameObject(shieldSprite);
	BaseEnemy::Kill();
}

SpeedyEnemy* SpeedyEnemy::Create(const glm::vec2& position)
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(Transform(position, SPEEDY_SIZE, 180.0f));

	return go->AddComponent<SpeedyEnemy>();
}