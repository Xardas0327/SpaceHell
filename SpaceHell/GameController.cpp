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
    : UpdaterComponent(gameObject), Component(gameObject), player(nullptr), backgroundController(nullptr),
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
    player = PlayerController::Create(
        glm::vec2(
            Game::mainCamera.GetResolution().GetWidth() / 2.0f - PLAYER_SIZE.x,
            Game::mainCamera.GetResolution().GetHeight() - 100.0f
        )
    );
    player->SetFrozen(false);

    //Background
    backgroundController = BackgroundController::Create(Game::mainCamera.GetResolution().ToVec2());
    backgroundController->Start();
}

void GameController::Update()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }
}