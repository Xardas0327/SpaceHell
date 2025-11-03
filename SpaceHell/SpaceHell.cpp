#include "SpaceHell.h"

#include <Learning2DEngine/System/GameObjectManager.h>

//#include "GameController.h"

using namespace Learning2DEngine::System;

void SpaceHell::Init()
{
    Game::Init();

    // MSAA
    ActivateMSAA(4);

    //GameController
    //auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    //gameController->AddComponent<GameController>();
}