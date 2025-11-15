#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>
#include <Learning2DEngine/Object/FpsShower.h>
#include <Learning2DEngine/UI/SimpleText2DRenderComponent.h>

#include "BackgroundController.h"
#include "PlayerController.h"
#include "Enemy/EnemySpawner.h"
#include "Enemy/EnemyKilledByPlayerEvenItem.h"
#include "Enemy/EndOfEnemyWaveEventItem.h"


enum class GameStatus { Menu, Play, Ended };



class GameController : public Learning2DEngine::System::UpdaterComponent,
    public IEnemyKilledByPlayerMessage, public IEndOfEnemyWaveMessage
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Object::FpsShower* fpsShower;
    PlayerController* player;
    BackgroundController* backgroundController;
    EnemySpawner* enemySpawner;
    Learning2DEngine::UI::SimpleText2DRenderComponent* scoreText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* waveText;
    const Learning2DEngine::UI::FontSizePair font;
    EnemyKilledByPlayerEvenItem refreshScoreEventItem;
    EndOfEnemyWaveEventItem endOfWaveEventItem;
    int score;
    int waveNumber;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void InitTexts();
    void Update() override;
    void Destroy() override;

    void EnemyKilled(int point) override;
    void EndOfWave() override;
    void RefreshScore();
    void RefreshWaves();
};

