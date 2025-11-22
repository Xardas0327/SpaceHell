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
    resourceManager.LoadTextureFromFile("Hero", "Assets/Images/Player/Hero.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("EnemyBullet", "Assets/Images/Weapon/EnemyBullet.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("PlayerBullet", "Assets/Images/Weapon/PlayerBullet.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("HeroBullet", "Assets/Images/Weapon/HeroBullet.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("ShieldBuff", "Assets/Images/Buff/Shield.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpeedBuff", "Assets/Images/Buff/Speed.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("WeaponBuff", "Assets/Images/Buff/Weapon.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("FighterEnemy", "Assets/Images/Enemy/Fighter.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("ScoutEnemy", "Assets/Images/Enemy/Scout.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpeedyEnemy", "Assets/Images/Enemy/Speedy.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("SpeedyEnemyShield", "Assets/Images/Enemy/SpeedyShield.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("DiskEnemy", "Assets/Images/Enemy/Disk.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("BossEnemy", "Assets/Images/Enemy/Boss.png", Texture2DSettings(true));

    resourceManager.LoadTextureFromFile("Explosion1", "Assets/Images/Explosion/Explosion2_1.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion2", "Assets/Images/Explosion/Explosion2_2.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion3", "Assets/Images/Explosion/Explosion2_3.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion4", "Assets/Images/Explosion/Explosion2_4.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion5", "Assets/Images/Explosion/Explosion2_5.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion6", "Assets/Images/Explosion/Explosion2_6.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion7", "Assets/Images/Explosion/Explosion2_7.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion8", "Assets/Images/Explosion/Explosion2_8.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion9", "Assets/Images/Explosion/Explosion2_9.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion10", "Assets/Images/Explosion/Explosion2_10.png", Texture2DSettings(true));
    resourceManager.LoadTextureFromFile("Explosion11", "Assets/Images/Explosion/Explosion2_11.png", Texture2DSettings(true));

    //GameController
    auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    gameController->AddComponent<GameController>();
}