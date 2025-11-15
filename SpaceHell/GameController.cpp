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
    backgroundController(nullptr), enemySpawner(nullptr), scoreText(nullptr), waveText(nullptr),
    font("Assets/Fonts/arial.ttf", 24),
    refreshScoreEventItem(this), endOfWaveEventItem(this),
    score(0), waveNumber(0), timer(0.0f), isWaveStarted(false)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

    InitTexts();

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

    //Score
    RefreshScore();

    //Waves
    RefreshWaves();

    //Enemy Spawner
    enemySpawner = GameObjectManager::GetInstance().CreateGameObject()->AddComponent<EnemySpawner>();
    enemySpawner->destroyedAllEnemies.Add(&endOfWaveEventItem);
    enemySpawner->refreshScore.Add(&refreshScoreEventItem);

    //TEST
    //waveNumber = 4; // next wave is waveNumber + 1
    StartTimer();
}

void GameController::InitTexts()
{
    auto& gameObjectManager = GameObjectManager::GetInstance();

    //Score Text
    auto scoreGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(10.0f, 10.0f)
        )
    );
    scoreText = scoreGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font);

    //Waves Text
    auto wavesGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() - 175.0f, 10.0f)
        )
    );
    waveText = wavesGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font);

    //FPS
    fpsShower = FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(10.0f, Game::mainCamera.GetResolution().GetHeight() - 30.0f)
        ),
        font,
        99);
    fpsShower->gameObject->isActive = false;
}

void GameController::Update()
{
    CheckKeyboard();
    RunTimer();
}

void GameController::Destroy()
{
    enemySpawner->ClearSpawnedEnemies();
    UpdaterComponent::Destroy();
}

void GameController::CheckKeyboard()
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

void GameController::StartTimer()
{
    timer = 0.0f;
    isWaveStarted = false;
}

void GameController::RunTimer()
{
    if (isWaveStarted)
        return;

    timer += Time::GetDeltaTime();
    if (timer >= TIME_BETWEEN_WAVES)
    {
        isWaveStarted = true;
        SpawnNextWave();
    }
}

void GameController::SpawnNextWave()
{
    if (waveNumber >= WAVE_COUNT)
        return;

    ++waveNumber;
    RefreshWaves();

    switch (waveNumber)
    {
    case 1:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 2.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(500.0f, -50.0f), 1.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(300.0f, -50.0f), 1.0f },
            });
        break;
    case 2:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Fighter, glm::vec2(350.0f, -50.0f), 2.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(500.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 300.0f), 0.5f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 350.0f), 0.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 400.0f), 0.5f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 4.5f },
            });
        break;
    case 3:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Speedy, glm::vec2(200.0f, -50.0f), 2.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(400.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 1.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(350.0f, -50.0f), 2.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(150.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(550.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(350.0f, -50.0f), 2.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(350.0f, -50.0f), 2.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 550.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 500.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 450.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 400.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 350.0f), 0.2f },
            });
        break;
    case 4:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 1.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 2.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 300.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(525.0f - FIGHTER_SIZE.x, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(175.0f - FIGHTER_SIZE.x, -50.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2.0f - SPEEDY_SIZE.x, -50.0f), 0.0f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 450.0f), 0.5f },
            });
        break;
    case 5:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 1.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.5f},
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(200.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(300.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(400.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(500.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(600.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 3.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(150.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(350.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(600.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 300.0f), 2.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 350.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 450.0f), 0.2f },
            });
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    default:
        break;
    }

    enemySpawner->StartSpawning();
}

void GameController::EnemyKilled(int point)
{
    score += point;
    RefreshScore();
}

void GameController::EndOfWave()
{
    StartTimer();
}

void GameController::RefreshScore()
{
    scoreText->data.SetText("Score: " + std::to_string(score));
}

void GameController::RefreshWaves()
{
    waveText->data.SetText("Waves: " + std::to_string(waveNumber) + "/10");
}