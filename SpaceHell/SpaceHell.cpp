#include "SpaceHell.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/RenderManager.h>

#include "GameController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

void SpaceHell::Init()
{
    Game::Init();

    auto& resourceManager = ResourceManager::GetInstance();

	//Default Settings

    //Camera
    Game::mainCamera.SetResolution(RenderManager::GetInstance().GetResolution());

    // Textures
    resourceManager.LoadTextureFromFile("Player", "Assets/Images/PlayerShip.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Background1", "Assets/Images/Background1.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Background2", "Assets/Images/Background2.png", Texture2DSettings(true));

    //GameController
    auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    gameController->AddComponent<GameController>();
}