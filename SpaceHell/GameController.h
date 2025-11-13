#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>
#include <Learning2DEngine/Object/FpsShower.h>

#include "BackgroundController.h"
#include "PlayerController.h"
#include "Enemy/EnemySpawner.h"

enum class GameStatus { Menu, Play, Ended };

class GameController : public Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Object::FpsShower* fpsShower;
    PlayerController* player;
    BackgroundController* backgroundController;
    EnemySpawner* enemySpawner;
    const Learning2DEngine::UI::FontSizePair fpsFont;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;
};

