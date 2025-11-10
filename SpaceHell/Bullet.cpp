#include "Bullet.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

#include "Enemy/BaseEnemy.h"
#include "PlayerController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;


Bullet::Bullet(GameObject* gameObject, const glm::vec2& direction, float speed, int32_t mask)
    : UpdaterComponent(gameObject), Component(gameObject),
    CircleColliderComponent(gameObject, BULLET_SIZE.x / 2.0f, ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(0.0f, 0.0f), mask),
    direction(direction), speed(speed)
{

}

void Bullet::Init()
{
    UpdaterComponent::Init();
    CircleColliderComponent::Init();

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


void Bullet::Destroy()
{
    CircleColliderComponent::Destroy();
    UpdaterComponent::Destroy();
}

void Bullet::OnCollision(const Collision& collision)
{
    auto player = collision.collidedObject->GetComponent<PlayerController>();
    bool hitSomething = false;
    if (player != nullptr)
    {
        player->Hit();
        hitSomething = true;
    }

    auto enemy = collision.collidedObject->GetComponent<BaseEnemy>();
    if (enemy != nullptr)
    {
        enemy->Hit(1);
        hitSomething = true;
    }

    if (hitSomething)
    {
        GameObjectManager::GetInstance().DestroyGameObject(gameObject);
    }
}

void Bullet::CheckOutOfScreen()
{
    if (gameObject->transform.GetPosition().y < -10.0f
        || gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() + 10.0f
        || gameObject->transform.GetPosition().x < -10.0f
        || gameObject->transform.GetPosition().x > Game::mainCamera.GetResolution().GetWidth() + 10.0f)
    {
        GameObjectManager::GetInstance().DestroyGameObject(gameObject);
    }
}

Bullet* Bullet::Create(const glm::vec2& position, const glm::vec2& direction, float speed, int32_t mask)
{
    auto go = GameObjectManager::GetInstance().CreateGameObject(Transform(position, BULLET_SIZE));

    return go->AddComponent<Bullet>(direction, speed, mask);
}