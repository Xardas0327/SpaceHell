#include "PlayerController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>

#include "Bullet.h"

using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

PlayerController::PlayerController(GameObject* gameObject)
	: UpdaterComponent(gameObject), Component(gameObject),
	shieldSprite(nullptr), shieldAnimation(nullptr),
	isFrozen(true), reloadTimer(0.0f), bulletNumber(PLAYER_BULLET_DEFAULT_NUMBER), maxBulletNumber(PLAYER_BULLET_DEFAULT_NUMBER)
{
	
}

void PlayerController::Init()
{
	UpdaterComponent::Init();

	gameObject->transform.SetScale(PLAYER_SIZE);

	gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture("Player"),
        0
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
            i / 12.0f, 0.0f,
            (i + 1.0f) / 12.0f, 0.0f,
            (i + 1.0f) / 12.0f, 1.0f,
            i / 12.0f, 1.0f
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
}

void PlayerController::CheckKeyboard()
{
    if (isFrozen)
        return;

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_A) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_LEFT) > 0)
        && gameObject->transform.GetPosition().x > 0.0f)
    {
        gameObject->transform.AddPosition(glm::vec2(-PLAYER_SPEED * Time::GetDeltaTime(), 0.0f));
        if (gameObject->transform.GetPosition().x < 0.0f)
            gameObject->transform.SetPosition(glm::vec2(0.0f, gameObject->transform.GetPosition().y));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_D) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_RIGHT) > 0)
        && gameObject->transform.GetPosition().x < Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x)
    {
        gameObject->transform.AddPosition(glm::vec2(PLAYER_SPEED * Time::GetDeltaTime(), 0.0f));
        if (gameObject->transform.GetPosition().x > Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x)
            gameObject->transform.SetPosition(glm::vec2(Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x, gameObject->transform.GetPosition().y));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_W) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_UP) > 0)
        && gameObject->transform.GetPosition().y > 0.0f)
    {
        gameObject->transform.AddPosition(glm::vec2(0.0f, -PLAYER_SPEED * Time::GetDeltaTime()));
        if (gameObject->transform.GetPosition().y < 0.0f)
            gameObject->transform.SetPosition(glm::vec2(gameObject->transform.GetPosition().x, 0.0f));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_S) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_DOWN) > 0)
        && gameObject->transform.GetPosition().x < Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y)
    {
        gameObject->transform.AddPosition(glm::vec2(0.0f, PLAYER_SPEED * Time::GetDeltaTime()));
        if (gameObject->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y)
            gameObject->transform.SetPosition(glm::vec2(gameObject->transform.GetPosition().x, Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y));
    }

    if(Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
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
        if(reloadTimer >= PLAYER_RELOAD)
        {
            ++bulletNumber;
            reloadTimer = 0.0f;
		}
    }
}

void PlayerController::Shoot()
{
    if (bulletNumber)
    {
        Bullet::Create(
            gameObject->transform.GetPosition() + glm::vec2(gameObject->transform.GetScale().x / 2.0f - BULLET_SIZE.x / 2.0f, 0.0f),
            glm::vec2(0.0f, -1.0f),
            PLAYER_BULLET_SPEED
        );

		--bulletNumber;
    }
}

void PlayerController::Reset(glm::vec2 position)
{
	gameObject->transform.SetPosition(position);
}

void PlayerController::SetFrozen(bool frozen)
{
	isFrozen = frozen;
}

PlayerController* PlayerController::Create(const glm::vec2& position)
{
    auto object = GameObjectManager::GetInstance().CreateGameObject(Transform(position));

    return object->AddComponent<PlayerController>();
}