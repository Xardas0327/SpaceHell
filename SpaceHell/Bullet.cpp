#include "Bullet.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;


Bullet::Bullet(GameObject* gameObject, const glm::vec2& direction, float speed)
	: UpdaterComponent(gameObject), Component(gameObject), direction(direction), speed(speed)
{

}

void Bullet::Init()
{
	UpdaterComponent::Init();

	auto render = gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("Bullet"),
		-1
	);
}

void Bullet::Update()
{
    //Move 
	gameObject->transform.AddPosition(direction * speed * Time::GetDeltaTime());

    CheckOutOfScreen();
}

void Bullet::CheckOutOfScreen()
{
    if (gameObject->transform.GetPosition().y < -100.f
        || gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() + 100.0f
        || gameObject->transform.GetPosition().x < -100.f
        || gameObject->transform.GetPosition().x > Game::mainCamera.GetResolution().GetWidth() + 100.0f)
    {
        GameObjectManager::GetInstance().DestroyGameObject(gameObject);
    }
}

Bullet* Bullet::Create(const glm::vec2& position, const glm::vec2& direction, float speed)
{
    auto go = GameObjectManager::GetInstance().CreateGameObject(Transform(position, BULLET_SIZE));

    return go->AddComponent<Bullet>(direction, speed);
}