#include "BaseBuff.h"

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

BaseBuff::BaseBuff(GameObject* gameObject, std::string&& textureId, float lifeInSeconds, int animationLength, float animationFrameLength, float speed)
	: LateUpdaterComponent(gameObject), Component(gameObject),
	CircleColliderComponent(gameObject, 16.0f, ColliderType::DYNAMIC, ColliderMode::TRIGGER),
	textureId(std::move(textureId)), animationLength(animationLength), animationFrameLength(animationFrameLength),
	lifeInSeconds(lifeInSeconds), speed(speed)
{
}

void BaseBuff::Init()
{
	LateUpdaterComponent::Init();
	CircleColliderComponent::Init();

	gameObject->transform.SetScale(glm::vec2(32.0f, 32.0f));

	auto& texture = ResourceManager::GetInstance().GetTexture(textureId);

	auto render = gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		texture,
		0
	);

	if(animationLength > 1)
	{
		auto animationController = gameObject->AddComponent<AnimationController>(
			&render->data,
			animationLength,
			true
		);
		for (int i = 0; i < animationLength; ++i)
		{
			auto uvMatrix = glm::mat4x2{
				i / static_cast<float>(animationLength), 0.0f,
				(i + 1.0f) / static_cast<float>(animationLength), 0.0f,
				(i + 1.0f) / static_cast<float>(animationLength), 1.0f,
				i / static_cast<float>(animationLength), 1.0f
			};
			animationController->Add(AnimationFrame{
				&texture,
				uvMatrix,
				animationFrameLength
			});
		}
		animationController->Play();
	}
}

void BaseBuff::LateUpdate()
{
	gameObject->transform.AddPosition(glm::vec2(0.0f, speed * Time::GetDeltaTime()));

	lifeInSeconds -= Time::GetDeltaTime();
	if (lifeInSeconds <= 0.0f)
	{
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
	}
}

void BaseBuff::Destroy()
{
	CircleColliderComponent::Destroy();
	LateUpdaterComponent::Destroy();
}

void BaseBuff::OnCollision(const Collision& collision)
{
	auto player = collision.collidedObject->GetComponent<PlayerController>();
	if (player != nullptr)
	{
		BuffPlayer(player);
		GameObjectManager::GetInstance().DestroyGameObject(gameObject);
	}
}