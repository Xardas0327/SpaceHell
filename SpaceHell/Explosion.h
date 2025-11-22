#pragma once

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>

constexpr glm::vec2 EXPLOSION_SIZE(64.0f, 64.0f);
constexpr int EXPLOSION_ANIMATION_NUMBER = 11;
constexpr float EXPLOSION_ANIMATION_FRAME_TIME = 0.1f;
constexpr const char* EXPLOSION_TEXTURE_IDS[] = {
    "Explosion1",
    "Explosion2",
    "Explosion3",
    "Explosion4",
    "Explosion5",
    "Explosion6",
    "Explosion7",
    "Explosion8",
    "Explosion9",
    "Explosion10",
    "Explosion11",
};

class Explosion : public Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    float timer;

    Explosion(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;

public:
    static Explosion* Create(const glm::vec2& position);
};

