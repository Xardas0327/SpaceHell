#pragma once

#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>

constexpr glm::vec2 HERO_SIZE(128.0f, 128.0f);
constexpr float HERO_SPEED = 300.0f;
constexpr float HERO_POINT_X = 20.0f;
constexpr float HERO_BULLET_SPEED = 100.0f;
constexpr glm::vec2 HERO_BULLET_SIZE(24.0f, 24.0f);
constexpr int HERO_BULLET_ANIMATION_NUMBER = 8;

class HeroController : public Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    bool isArrived;
    bool isLeaving;
    glm::vec2 direction;
    irrklang::ISoundEngine* soundEngine;

    HeroController(Learning2DEngine::System::GameObject* gameObject, irrklang::ISoundEngine* soundEngine);

    void Init() override;
    void Update() override;

    void MoveToPoint();
    void Shoot();
    void LeaveMap();

public:
    Learning2DEngine::EventSystem::EventHandler<> onLeftMap;

    void StartToLeaveMap();

    static HeroController* Create(irrklang::ISoundEngine* soundEngine);
};

