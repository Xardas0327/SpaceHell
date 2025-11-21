#include "PlayerController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>

#include "Bullet.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/BossEnemy.h"

using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

PlayerController::PlayerController(GameObject* gameObject)
    : UpdaterComponent(gameObject), Component(gameObject),
    CircleColliderComponent(gameObject, PLAYER_SIZE.x / 2.0f, ColliderType::DYNAMIC, ColliderMode::TRIGGER, PLAYER_COllIDER_OFFSET, PLAYER_COLLER_MASK),
    sprite(nullptr), shieldSprite(nullptr), shieldAnimation(nullptr),
    isFrozen(true), reloadTimer(0.0f), bulletNumber(PLAYER_BULLET_DEFAULT_NUMBER), maxBulletNumber(PLAYER_BULLET_DEFAULT_NUMBER),
	life(PLAYER_DEFAULT_LIFE), speed(PLAYER_DEFAULT_SPEED), isImmortal(false), immortalTimer(0.0f), onDead()
{

}

void PlayerController::Init()
{
    UpdaterComponent::Init();
    CircleColliderComponent::Init();

    gameObject->transform.SetScale(PLAYER_SIZE);

    sprite = gameObject->AddComponent<SpriteRenderComponent>(
        RendererMode::RENDER,
        ResourceManager::GetInstance().GetTexture("Player")
    );

    //Shield
    auto shieldGO = GameObjectManager::GetInstance().CreateGameObject(
        Transform(
            gameObject->transform.GetPosition() + PLAYER_SHIELD_OFFSET,
            PLAYER_SHIELD_SIZE
        )
    );

    auto& shieldTexture = ResourceManager::GetInstance().GetTexture("PlayerShield");
    shieldSprite = shieldGO->AddComponent<SpriteRenderComponent>(
        RendererMode::RENDER,
        shieldTexture,
        1,
        glm::vec4(1.0f, 1.0f, 1.0f, 0.9f)
    );

    shieldAnimation = gameObject->AddComponent<AnimationController>(&shieldSprite->data, PLAYER_SHIELD_NUMBER, true);
    for (int i = 0; i < PLAYER_SHIELD_NUMBER; ++i)
    {
        auto uvMatrix = glm::mat4x2{
            i / static_cast<float>(PLAYER_SHIELD_NUMBER), 0.0f,
            (i + 1.0f) / static_cast<float>(PLAYER_SHIELD_NUMBER), 0.0f,
            (i + 1.0f) / static_cast<float>(PLAYER_SHIELD_NUMBER), 1.0f,
            i / static_cast<float>(PLAYER_SHIELD_NUMBER), 1.0f
        };
        shieldAnimation->Add(AnimationFrame{
            &shieldTexture,
            uvMatrix,
            0.25f
            });
    }
    shieldAnimation->Play();
}

void PlayerController::Update()
{
    CheckKeyboard();
    RefreshShieldPosition();
    Reload();
    RefreshImmortal();
}

void PlayerController::Destroy()
{
    CircleColliderComponent::Destroy();
    UpdaterComponent::Destroy();
}

void PlayerController::OnCollision(const Collision& collision)
{
    auto boss = collision.collidedObject->GetComponent<BossEnemy>();
    if (boss != nullptr)
        return;

    auto enemy = collision.collidedObject->GetComponent<BaseEnemy>();
    if (enemy != nullptr)
    {
        Hit(enemy->GetLife());
        enemy->Kill();
    }
}

void PlayerController::CheckKeyboard()
{
    if (isFrozen)
        return;

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_A) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_LEFT) > 0)
        && gameObject->transform.GetPosition().x > 0.0f)
    {
        gameObject->transform.AddPosition(glm::vec2(-speed * Time::GetDeltaTime(), 0.0f));
        if (gameObject->transform.GetPosition().x < 0.0f)
            gameObject->transform.SetPosition(glm::vec2(0.0f, gameObject->transform.GetPosition().y));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_D) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_RIGHT) > 0)
        && gameObject->transform.GetPosition().x < Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x)
    {
        gameObject->transform.AddPosition(glm::vec2(speed * Time::GetDeltaTime(), 0.0f));
        if (gameObject->transform.GetPosition().x > Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x)
            gameObject->transform.SetPosition(glm::vec2(Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x, gameObject->transform.GetPosition().y));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_W) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_UP) > 0)
        && gameObject->transform.GetPosition().y > 0.0f)
    {
        gameObject->transform.AddPosition(glm::vec2(0.0f, -speed * Time::GetDeltaTime()));
        if (gameObject->transform.GetPosition().y < 0.0f)
            gameObject->transform.SetPosition(glm::vec2(gameObject->transform.GetPosition().x, 0.0f));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_S) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_DOWN) > 0)
        && gameObject->transform.GetPosition().x < Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y)
    {
        gameObject->transform.AddPosition(glm::vec2(0.0f, speed * Time::GetDeltaTime()));
        if (gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y)
            gameObject->transform.SetPosition(glm::vec2(gameObject->transform.GetPosition().x, Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y));
    }

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
        Shoot();
}

void PlayerController::RefreshShieldPosition()
{
    shieldSprite->gameObject->transform.SetPosition(
        gameObject->transform.GetPosition() + PLAYER_SHIELD_OFFSET
    );
}

void PlayerController::Reload()
{
    if (bulletNumber < maxBulletNumber)
    {
        reloadTimer += Time::GetDeltaTime();
        if (reloadTimer >= (PLAYER_BULLET_RELOAD / maxBulletNumber))
        {
            ++bulletNumber;
            reloadTimer = 0.0f;
        }
    }
}

void PlayerController::RefreshImmortal()
{
    if (isImmortal)
    {
        immortalTimer += Time::GetDeltaTime();
        sprite->isActive = ((int)(immortalTimer / PLAYER_IMMORTAL_VIBRATION_FREQUENCY) % 2) == 0;
        if (immortalTimer >= PLAYER_IMMORTAL_AFTER_HIT)
        {
            isImmortal = false;
            sprite->isActive = true;
            immortalTimer = 0.0f;
        }
    }
}

void PlayerController::Shoot()
{
    if (bulletNumber)
    {
        Bullet::Create(
            gameObject->transform.GetPosition() + glm::vec2(gameObject->transform.GetScale().x / 2.0f - PLAYER_BULLET_SIZE.x / 2.0f, 0.0f),
            PLAYER_BULLET_SIZE,
            0.0f,
            "PlayerBullet",
            glm::vec2(0.0f, -1.0f),
            PLAYER_BULLET_SPEED,
            PLAYER_BULLET_MASK,
            1
        );

        --bulletNumber;
    }
}

void PlayerController::Reset(glm::vec2 position)
{
    gameObject->transform.SetPosition(position);
    speed = PLAYER_DEFAULT_SPEED;
    bulletNumber = PLAYER_BULLET_DEFAULT_NUMBER;
    maxBulletNumber = PLAYER_BULLET_DEFAULT_NUMBER;
    ResetLife();
}

void PlayerController::SetFrozen(bool frozen)
{
    isFrozen = frozen;
}

void PlayerController::Hit(int damage)
{
    if (isImmortal || isFrozen)
        return;

    life -= damage;
    RefreshLifeShield();

    if(life <= 0)
    {
        onDead.Invoke();
        return;
    }
    isImmortal = true;
}

void PlayerController::ResetLife()
{
    life = PLAYER_DEFAULT_LIFE;
    RefreshLifeShield();
}

void PlayerController::RefreshLifeShield()
{
    shieldSprite->isActive = life > 1;
    shieldAnimation->isActive = life > 1;
    shieldSprite->data.color.a = (life - 1) * 0.3f;
}

void PlayerController::IncreaseMaxBulletNumber(int increase)
{
    maxBulletNumber += increase;
    bulletNumber += increase;
    if (bulletNumber > maxBulletNumber)
        bulletNumber = maxBulletNumber;
}

void PlayerController::IncreaseSpeed(float increase)
{
    speed += increase;
}

PlayerController* PlayerController::Create(const glm::vec2& position)
{
    auto object = GameObjectManager::GetInstance().CreateGameObject(Transform(position));

    return object->AddComponent<PlayerController>();
}