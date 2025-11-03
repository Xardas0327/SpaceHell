#include "GameController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Object/FpsShower.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Object;
using namespace Learning2DEngine::UI;

GameController::GameController(GameObject* gameObject)
    : UpdaterComponent(gameObject), Component(gameObject), player(nullptr),
    gameFont("Assets/Fonts/ViperSpikes.ttf", 24), fpsFont("Assets/Fonts/arial.ttf", 24)
{

}

void GameController::Init()
{
	UpdaterComponent::Init();

    //FPS
    FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(2.0f, Game::mainCamera.GetResolution().GetHeight() - 26)
        ),
        fpsFont,
        99);

    //Player
	player = GameObjectManager::GetInstance().CreateGameObject(
        Transform(
            glm::vec2(
                Game::mainCamera.GetResolution().GetWidth() / 2.0f - PLAYER_SIZE.x,
				Game::mainCamera.GetResolution().GetHeight() - 100.0f
            ),
            PLAYER_SIZE
        )
    );

	player->AddComponent<SpriteRenderComponent>(
        RendererMode::RENDER,
        ResourceManager::GetInstance().GetTexture("Player")
    );
}

void GameController::Update()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_A) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_LEFT) > 0)
        && player->transform.GetPosition().x > 0.0f)
    {
		player->transform.AddPosition(glm::vec2(-PLAYER_SPEED * Time::GetDeltaTime(), 0.0f));
        if(player->transform.GetPosition().x < 0.0f)
			player->transform.SetPosition(glm::vec2(0.0f, player->transform.GetPosition().y));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_D) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_RIGHT) > 0)
        && player->transform.GetPosition().x < Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x)
    {
        player->transform.AddPosition(glm::vec2(PLAYER_SPEED * Time::GetDeltaTime(), 0.0f));
        if (player->transform.GetPosition().x > Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x)
            player->transform.SetPosition(glm::vec2(Game::mainCamera.GetResolution().GetWidth() - PLAYER_SIZE.x, player->transform.GetPosition().y));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_W) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_UP) > 0)
        && player->transform.GetPosition().y > 0.0f)
    {
        player->transform.AddPosition(glm::vec2(0.0f, -PLAYER_SPEED * Time::GetDeltaTime()));
        if (player->transform.GetPosition().y < 0.0f)
            player->transform.SetPosition(glm::vec2(player->transform.GetPosition().x, 0.0f));
    }

    if ((Game::GetKeyboardButtonStatus(GLFW_KEY_A) > 0 || Game::GetKeyboardButtonStatus(GLFW_KEY_DOWN) > 0)
        && player->transform.GetPosition().x < Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y)
    {
        player->transform.AddPosition(glm::vec2(0.0f, PLAYER_SPEED * Time::GetDeltaTime()));
        if (player->transform.GetPosition().y > Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y)
            player->transform.SetPosition(glm::vec2(player->transform.GetPosition().x, Game::mainCamera.GetResolution().GetHeight() - PLAYER_SIZE.y));
    }
}