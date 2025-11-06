#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>

#include "BackgroundController.h"
#include "PlayerController.h"

enum class GameStatus { Menu, Play, Ended };

class GameController : protected Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    PlayerController* player;
    BackgroundController* backgroundController;
    const Learning2DEngine::UI::FontSizePair gameFont;
    const Learning2DEngine::UI::FontSizePair fpsFont;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;
};

