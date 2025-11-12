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

    resourceManager.LoadTextureFromFile("EnemyBullet", "Assets/Images/Weapon/EnemyBullet.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("PlayerBullet", "Assets/Images/Weapon/PlayerBullet.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Ray", "Assets/Images/Weapon/Ray.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpinningBullet", "Assets/Images/Weapon/SpinningBullet.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Wave", "Assets/Images/Weapon/Wave.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("ShieldBuff", "Assets/Images/Buff/Shield.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpeedBuff", "Assets/Images/Buff/Speed.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("WeaponBuff", "Assets/Images/Buff/Weapon.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("FighterEnemy", "Assets/Images/Enemy/Fighter.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("ScoutEnemy", "Assets/Images/Enemy/Scout.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpeedyEnemy", "Assets/Images/Enemy/Speedy.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpeedyEnemyShield", "Assets/Images/Enemy/SpeedyShield.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("DiskEnemy", "Assets/Images/Enemy/Disk.png", Texture2DSettings(true));

    //GameController
    auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    gameController->AddComponent<GameController>();
}