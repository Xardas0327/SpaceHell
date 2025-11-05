#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>

#include "BackgroundController.h"

enum class GameStatus { Menu, Play, Ended };

const glm::vec2 PLAYER_SIZE(96.0f, 96.0f);
const float PLAYER_SPEED = 500.0f;

class GameController : protected Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::System::GameObject* player;
    BackgroundController* backgroundController;
    const Learning2DEngine::UI::FontSizePair gameFont;
    const Learning2DEngine::UI::FontSizePair fpsFont;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;
};

