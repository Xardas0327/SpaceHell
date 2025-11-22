#pragma once

#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>
#include <Learning2DEngine/Object/FpsShower.h>
#include <Learning2DEngine/UI/Text2DRenderComponent.h>

#include "BackgroundController.h"
#include "PlayerController.h"
#include "Enemy/EnemySpawner.h"
#include "Event/DeadOfPlayerEventItem.h"
#include "Event/EnemyKilledByPlayerEvenItem.h"
#include "Event/EndOfEnemyWaveEventItem.h"
#include "Event/BossArrivedEventItem.h"
#include "Event/BossDestroyedEventItem.h"
#include "Event/HeroLeftEventItem.h"
#include "Enemy/BossEnemy.h"
#include "HeroController.h"


enum class GameStatus { Menu, Intro, Play, Ended };

constexpr float TIME_BETWEEN_WAVES = 5.0f;
constexpr int WAVE_COUNT = 10;
constexpr const char* CONTROL_TEXT = "Control:\nMove: WASD or Arrows\nShoot: SPACE\nShow/Hide FPS: F";
constexpr const char* PRESS_TEXT = "Press ENTER";
constexpr const char* START_TEXT = "The Akruh Empire attack our galaxy and\nthere is only ONE person, who can save us.\nBut the hero is out of office.\nSo...\nGood luck Rookie!";
constexpr const char* GAME_OVER_TEXT = "GAME OVER";
constexpr const char* FINISH_TEXT = "Have you heard?\nThe hero came back and killed\nthe leader of Akruh Empire!\nWe are saved again by the great hero!\nAnyway, where are you?\nA lot of paper work is waiting for you.";

class GameController : public Learning2DEngine::System::UpdaterComponent,
	public IEnemyKilledByPlayerMessage, public IEndOfEnemyWaveMessage, public IDeadOfPlayerMessage,
    public IBossMessage, IHeroLeftMessage
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Object::FpsShower* fpsShower;
    PlayerController* player;
    BackgroundController* backgroundController;
    EnemySpawner* enemySpawner;
    Learning2DEngine::UI::Text2DRenderComponent* scoreText;
    Learning2DEngine::UI::Text2DRenderComponent* waveText;
    Learning2DEngine::UI::Text2DRenderComponent* controlText;
    Learning2DEngine::UI::Text2DRenderComponent* pressText;
    Learning2DEngine::UI::Text2DRenderComponent* startText;
    Learning2DEngine::UI::Text2DRenderComponent* finishText;
    Learning2DEngine::UI::Text2DRenderComponent* gameOverText;
    const Learning2DEngine::UI::FontSizePair font;
    EnemyKilledByPlayerEvenItem refreshScoreEventItem;
    EndOfEnemyWaveEventItem endOfWaveEventItem;
    DeadOfPlayerEventItem deadOfPlayerEventItem;
    BossArrivedEventItem bossArrivedEventItem;
    BossDestroyedEventItem bossDestroyedEventItem;
    HeroLeftEventItem heroLeftEventItem;
    int score;
    int waveNumber;
    float timer;
    bool isWaveStarted;
    GameStatus status;
    glm::vec2 playerStartPosition;
    BossEnemy* boss;
    HeroController* hero;
    irrklang::ISoundEngine* soundEngine;
    irrklang::ISound* backgroundMusic;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void InitTexts();
    void Update() override;
    void Destroy() override;

    void ShowControl();
    void ShowIntro();
    void StartGame();

    void CheckKeyboard();
    void StartTimer();
    void RunTimer();
    void SpawnNextWave();

    void EnemyKilled(int point) override;
    void EndOfWave() override;
    void DeadOfPlayer() override;
    void OnBossArrived() override;
    void OnBossDestroyed() override;
    void OnHeroLeft() override;
    void RefreshScore();
    void RefreshWaves();
    void StopBackgroundMusic();
};

