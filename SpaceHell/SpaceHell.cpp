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
    resourceManager.LoadTextureFromFile("Background1", "Assets/Images/Background/Background1.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Background2", "Assets/Images/Background/Background2.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("Player", "Assets/Images/Player/PlayerShip.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("PlayerShield", "Assets/Images/Player/Shield.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("PlayerShot1", "Assets/Images/Player/Shot1.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("PlayerShot2", "Assets/Images/Player/Shot2.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("PlayerShot3", "Assets/Images/Player/Shot3.png", Texture2DSettings(true));

    //GameController
    auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    gameController->AddComponent<GameController>();
}