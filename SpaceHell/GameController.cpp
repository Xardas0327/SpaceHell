#include "GameController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Object;
using namespace Learning2DEngine::UI;

GameController::GameController(GameObject* gameObject)
    : UpdaterComponent(gameObject), Component(gameObject), fpsShower(nullptr), player(nullptr),
	backgroundController(nullptr), enemySpawner(nullptr),
    fpsFont("Assets/Fonts/arial.ttf", 24)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

    //FPS
    fpsShower = FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(2.0f, Game::mainCamera.GetResolution().GetHeight() - 26)
        ),
        fpsFont,
        99);
    fpsShower->gameObject->isActive = false;

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

    //Enemy Spawner
    enemySpawner = GameObjectManager::GetInstance().CreateGameObject()->AddComponent<EnemySpawner>();

    //TEST
    /*std::vector<EnemySpawnerItem> enemiesToSpawn =
    {
        { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 2.0f },
        { EnemySpawnerItemType::Fighter, glm::vec2(500.0f, -50.0f), 0.0f },
        { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 300.0f), 0.5f },
        { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 500.0f), 0.5f },
        { EnemySpawnerItemType::Speedy, glm::vec2(200.0f, -50.0f), 2.0f },
        { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 1.5f },
        { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
        { EnemySpawnerItemType::Fighter, glm::vec2(600.0f, -50.0f), 1.0f },
	};
	enemySpawner->SetEnemies(enemiesToSpawn);
	enemySpawner->StartSpawning();*/
}

void GameController::Update()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_F) == InputStatus::KEY_DOWN)
    {
		fpsShower->gameObject->isActive = !fpsShower->gameObject->isActive;
    }
}