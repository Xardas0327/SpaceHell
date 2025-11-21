#include "GameController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/UI/TextBoxComponent.h>

#include "Buff/BuffSpawner.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Object;
using namespace Learning2DEngine::UI;

GameController::GameController(GameObject* gameObject)
    : UpdaterComponent(gameObject), Component(gameObject), fpsShower(nullptr), player(nullptr),
    backgroundController(nullptr), enemySpawner(nullptr), scoreText(nullptr), waveText(nullptr),
    controlText(nullptr), pressText(nullptr), startText(nullptr), finishText(nullptr),
    gameOverText(nullptr), font("Assets/Fonts/arial.ttf", 24),
    refreshScoreEventItem(this), endOfWaveEventItem(this), deadOfPlayerEventItem(this),
    bossArrivedEventItem(this), bossDestroyedEventItem(this), heroLeftEventItem(this),
    score(0), waveNumber(0), timer(0.0f), isWaveStarted(false), status(GameStatus::Menu),
    playerStartPosition(0.0f, 0.0f), boss(nullptr), hero(nullptr)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

    InitTexts();

    //Player
    playerStartPosition = glm::vec2(
        Game::mainCamera.GetResolution().GetWidth() / 2.0f - PLAYER_SIZE.x,
        Game::mainCamera.GetResolution().GetHeight() - 100.0f
    );
    player = PlayerController::Create(playerStartPosition);
    player->onDead.Add(&deadOfPlayerEventItem);

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

    ShowControl();
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

    //Control Text
    auto controlGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2.0f - 125.0f, Game::mainCamera.GetResolution().GetHeight() / 2.0f - 100.0f)
        )
    );
    controlText = controlGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font, CONTROL_TEXT);
    auto controlBox = controlGameObject->AddComponent<TextBoxComponent>(*controlText, TextBoxMode::SIMPLE, -1, glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    controlBox->SetPadding(10.0f);

    //Press Text
    auto pressGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2.0f - 75.0f, Game::mainCamera.GetResolution().GetHeight() / 2.0f + 30.0f)
        )
    );
    pressText = pressGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font, PRESS_TEXT);
    auto pressBox = pressGameObject->AddComponent<TextBoxComponent>(*pressText, TextBoxMode::SIMPLE, -1, glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    pressBox->SetPadding(10.0f);

    //Start Text
    auto startGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2.0f - 225.0f, Game::mainCamera.GetResolution().GetHeight() / 2.0f - 100.0f)
        )
    );
    startText = startGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font, START_TEXT);
    auto startBox = startGameObject->AddComponent<TextBoxComponent>(*startText, TextBoxMode::SIMPLE, -1, glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    startBox->SetPadding(10.0f);

    //Finish Text
    auto finishGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(150.0f, Game::mainCamera.GetResolution().GetHeight() / 2.0f - 120.0f)
        )
    );
    finishText = finishGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font, FINISH_TEXT);
    auto finish = finishGameObject->AddComponent<TextBoxComponent>(*finishText, TextBoxMode::SIMPLE, -1, glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    finish->SetPadding(10.0f);

    //Game Over Text
    auto gameOverGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2.0f - 110.0f, Game::mainCamera.GetResolution().GetHeight() / 2.0f - 20.0f),
            glm::vec2(1.5f, 1.5f)
        )
    );
    gameOverText = gameOverGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, font, GAME_OVER_TEXT);
    auto gameOver = gameOverGameObject->AddComponent<TextBoxComponent>(*gameOverText, TextBoxMode::SIMPLE, -1, glm::vec4(0.1f, 0.1f, 0.1f, 0.9f));
    gameOver->SetPadding(10.0f);

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

void GameController::ShowControl()
{
    status = GameStatus::Menu;
    enemySpawner->StopSpawning();
    enemySpawner->ClearSpawnedEnemies();
    controlText->isActive = true;
    pressText->isActive = true;
    startText->isActive = false;
    gameOverText->isActive = false;
    finishText->isActive = false;
    player->SetFrozen(true);
    player->Reset(playerStartPosition);
    player->gameObject->isActive = true;

    auto& buffSpawner = BuffSpawner::GetInstance();
    buffSpawner.ClearActiveBuffs();
    buffSpawner.ResetLimits();
}

void GameController::ShowIntro()
{
    status = GameStatus::Intro;
    waveNumber = 0;
    RefreshWaves();
    score = 0;
    RefreshScore();
    controlText->isActive = false;
    startText->isActive = true;
}

void GameController::StartGame()
{
    status = GameStatus::Play;
    pressText->isActive = false;
    startText->isActive = false;
    StartTimer();
    player->SetFrozen(false);
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

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
    {
        switch (status)
        {
        case GameStatus::Menu:
            ShowIntro();
            break;
        case GameStatus::Intro:
            StartGame();
            break;
        case GameStatus::Ended:
            ShowControl();
            break;
        default:
            break;
        }
    }
}

void GameController::StartTimer()
{
    timer = 0.0f;
    isWaveStarted = false;
}

void GameController::RunTimer()
{
    if (isWaveStarted || status != GameStatus::Play)
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
    ++waveNumber;
    RefreshWaves();

    if (waveNumber > WAVE_COUNT)
    {
        boss = BossEnemy::Create();
        boss->onArrived.Add(&bossArrivedEventItem);
        boss->onDead.Add(&bossDestroyedEventItem);
        return;
    }

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
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 1.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(175.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Fighter, glm::vec2(250.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Fighter, glm::vec2(325.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(100.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(175.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(325.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(325.0f, -50.0f), 1.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(600.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(525.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 0.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 550.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 250.0f), 1.5f },
            });
        break;
    case 7:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Speedy, glm::vec2(150.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(450.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(300.0f, -50.0f), 0.5f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 2.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 550.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 500.0f), 0.2f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 450.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 400.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 350.0f), 0.2f },
            { EnemySpawnerItemType::Fighter, glm::vec2(300.0f, -50.0f), 0.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(300.0f, -50.0f), 0.8f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 350.0f), 1.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(550.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 400.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 450.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 500.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(300.0f, -50.0f), 0.0f },
            });
        break;
    case 8:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(DISK_SIZE.x, 0.0f), 0.3f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.5f},
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - 2 * DISK_SIZE.x, 0.0f), 0.3f},
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 600.0f), 2.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(550.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.8f },
            { EnemySpawnerItemType::Speedy, glm::vec2(450.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(175.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Disk, glm::vec2(DISK_SIZE.x, 0.0f), 0.0f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 450.0f), 2.5f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 400.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 350.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(325.0f, -50.0f), 0.5f },
            });
        break;
    case 9:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(250.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(400.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(550.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 500.0f), 0.5f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 550.0f), 0.5f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - 2 * DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 550.0f), 0.5f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 500.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(175.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Speedy, glm::vec2(475.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 250.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 250.0f), 0.0f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 300.0f), 0.2f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 300.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(350.0f, -50.0f), 0.2f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(400.0f, -50.0f), 0.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(250.0f, -50.0f), 0.5f },
            });
        break;
    case 10:
        enemySpawner->SetEnemies({
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 650.0f), 0.5f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 600.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 550.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 500.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 450.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 400.0f), 0.2f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 350.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 300.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 250.0f), 0.2f },
            { EnemySpawnerItemType::Disk, glm::vec2(2 * DISK_SIZE.x, 0.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - 3 * DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 225.0f), 0.2f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 50.0f), 0.2f },
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 1.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(350.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(600.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(250.0f, -50.0f), 1.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(450.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(100.0f, -50.0f), 2.5f },
            { EnemySpawnerItemType::Speedy, glm::vec2(350.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Speedy, glm::vec2(600.0f, -50.0f), 0.0f },
            { EnemySpawnerItemType::Disk, glm::vec2(0.0f, 0.0f), 0.0f },
            { EnemySpawnerItemType::Fighter, glm::vec2(100.0f, -50.0f), 0.5f },
            { EnemySpawnerItemType::Fighter, glm::vec2(175.0f, -50.0f), 0.3f },
            { EnemySpawnerItemType::Speedy, glm::vec2(250.0f, -50.0f), 0.3f },
            { EnemySpawnerItemType::Fighter, glm::vec2(325.0f, -50.0f), 0.3f },
            { EnemySpawnerItemType::Speedy, glm::vec2(400.0f, -50.0f), 0.3f },
            { EnemySpawnerItemType::Fighter, glm::vec2(475.0f, -50.0f), 0.3f },
            { EnemySpawnerItemType::Disk, glm::vec2(Game::mainCamera.GetResolution().GetWidth() - DISK_SIZE.x, 0.0f), 0.0f},
            { EnemySpawnerItemType::Fighter, glm::vec2(400.0f, -50.0f), 1.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 550.0f), 0.0f },
            { EnemySpawnerItemType::ScoutLeft, glm::vec2(700.0f, 350.0f), 0.0f },
            { EnemySpawnerItemType::ScoutRight, glm::vec2(-SCOUT_SIZE.x, 400.0f), 0.5f },
            });
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

void GameController::DeadOfPlayer()
{
    status = GameStatus::Ended;
    player->gameObject->isActive = false;
    pressText->isActive = true;
    gameOverText->isActive = true;
}

void GameController::OnBossArrived()
{
    hero = HeroController::Create();
    hero->onLeftMap.Add(&heroLeftEventItem);
}

void GameController::OnBossDestroyed()
{
    boss = nullptr;
    hero->StartToLeaveMap();
}

void GameController::OnHeroLeft()
{
    status = GameStatus::Ended;
    hero = nullptr;
    pressText->isActive = true;
    finishText->isActive = true;
}

void GameController::RefreshScore()
{
    scoreText->data.SetText("Score: " + std::to_string(score));
}

void GameController::RefreshWaves()
{
    std::string waveStr = std::to_string(waveNumber);
    if (waveNumber > WAVE_COUNT)
        waveStr += "?";

    waveText->data.SetText("Waves: " + waveStr + "/" + std::to_string(WAVE_COUNT));
}