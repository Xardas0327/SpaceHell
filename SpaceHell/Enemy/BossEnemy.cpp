#include "BossEnemy.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;
using namespace irrklang;

BossEnemy::BossEnemy(GameObject* gameObject, ISoundEngine* soundEngine)
	: BaseEnemy(gameObject, glm::vec2(0.0f, 1.0f), BOSS_SPEED, 1, 0, 0), Component(gameObject),
	collider(nullptr), destructionAnimation(nullptr), isArrived(false), isDying(false), dyingTimer(0.0f),
	soundEngine(soundEngine), onDead(), onArrived()
{
}

void BossEnemy::Init()
{
	BaseEnemy::Init();

	auto& texture = ResourceManager::GetInstance().GetTexture("BossEnemy");
	auto render = gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		texture
	);
	render->data.uvMatrix = {
		0.0f, 0.0f,
		1.0f / static_cast<float>(BOSS_DESTRUCTION_NUMBER), 0.0f,
		1.0f / static_cast<float>(BOSS_DESTRUCTION_NUMBER), 1.0f,
		0.0f, 1.0f
	};

	collider = gameObject->AddComponent<BoxColliderComponent>(
		BOSS_SIZE - BOSS_COLLIDER_OFFSET * 2.0f,
		ColliderType::KINEMATIC,
		ColliderMode::TRIGGER,
		BOSS_COLLIDER_OFFSET,
		ENEMY_COLLER_MASK
	);

	destructionAnimation = gameObject->AddComponent<AnimationController>(&render->data, BOSS_DESTRUCTION_NUMBER, false);
	for (int i = 0; i < BOSS_DESTRUCTION_NUMBER; ++i)
	{
		auto uvMatrix = glm::mat4x2{
			i / static_cast<float>(BOSS_DESTRUCTION_NUMBER), 0.0f,
			(i + 1.0f) / static_cast<float>(BOSS_DESTRUCTION_NUMBER), 0.0f,
			(i + 1.0f) / static_cast<float>(BOSS_DESTRUCTION_NUMBER), 1.0f,
			i / static_cast<float>(BOSS_DESTRUCTION_NUMBER), 1.0f
		};
		destructionAnimation->Add(AnimationFrame{
			&texture,
			uvMatrix,
			BOSS_ANIMATION_SPEED
			});
	}
}

void BossEnemy::Update()
{
	MoveToPoint();
	DieProcess();
}

bool BossEnemy::CheckOutOfScreen()
{
	return false;
}

void BossEnemy::MoveToPoint()
{
	if(isArrived)
		return;

	glm::vec2 newPosition = gameObject->transform.GetPosition() + direction * speed * Time::GetDeltaTime();

	if (newPosition.y > ARRIVE_POINT_Y)
	{
		newPosition.y = ARRIVE_POINT_Y;
		onArrived.Invoke();
		isArrived = true;
	}

	gameObject->transform.SetPosition(newPosition);
}

void BossEnemy::DieProcess()
{
	if (!isDying)
		return;

	dyingTimer += Time::GetDeltaTime();
	if (dyingTimer >= (BOSS_DESTRUCTION_NUMBER * BOSS_ANIMATION_SPEED))
	{
		isDying = false;
		onDead.Invoke();
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
	}
}

void BossEnemy::Hit(int damage)
{
	return;
}

void BossEnemy::Kill()
{
	destructionAnimation->Play();
	isDying = true;
	collider->isActive = false;
	soundEngine->play2D("Assets/Sounds/8bit_expl_long_02.wav");
}

BossEnemy* BossEnemy::Create(ISoundEngine* soundEngine)
{
	auto go = GameObjectManager::GetInstance().CreateGameObject(
		Transform(
			glm::vec2(
				Game::mainCamera.GetResolution().GetWidth() / 2.0f - BOSS_SIZE.x / 2.0f,
				- BOSS_SIZE.y - 20.0f
			),
			BOSS_SIZE,
			180.0f
		)
	);
	return go->AddComponent<BossEnemy>(soundEngine);
}