#pragma once

#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>

constexpr glm::vec2 PLAYER_SIZE(64.0f, 64.0f);
constexpr float PLAYER_DEFAULT_SPEED = 300.0f;
constexpr glm::vec2 PLAYER_SHIELD_SIZE(96.0f, 96.0f);
constexpr glm::vec2 PLAYER_SHIELD_OFFSET(-16.0f, -10.0f);
constexpr glm::vec2 PLAYER_COllIDER_OFFSET(0.0f, 5.0f);
constexpr int PLAYER_SHIELD_NUMBER = 12;
constexpr float PLAYER_BULLET_SPEED = 500.0f;
constexpr glm::vec2 PLAYER_BULLET_SIZE(12.0f, 12.0f);
constexpr float PLAYER_BULLET_RELOAD = 1.0f;
constexpr int PLAYER_BULLET_DEFAULT_NUMBER = 1;
constexpr int PLAYER_DEFAULT_LIFE = 4;
constexpr int32_t PLAYER_COLLER_MASK = 0B110;
constexpr float PLAYER_IMMORTAL_AFTER_HIT = 1.0f;
constexpr float PLAYER_IMMORTAL_VIBRATION_FREQUENCY = 0.2f;

class PlayerController : public Learning2DEngine::System::UpdaterComponent,
                            public Learning2DEngine::Physics::CircleColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::Render::SpriteRenderComponent* sprite;
    Learning2DEngine::Render::SpriteRenderComponent* shieldSprite;
    Learning2DEngine::Animator::AnimationController* shieldAnimation;
	bool isFrozen;
	float reloadTimer;
	int bulletNumber;
    int maxBulletNumber;
    int life;
    float speed;
    bool isImmortal;
    float immortalTimer;
    irrklang::ISoundEngine* soundEngine;

    PlayerController(Learning2DEngine::System::GameObject* gameObject, irrklang::ISoundEngine* soundEngine);

    void Init() override;
    void Update() override;
    void Destroy() override;
    void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;

    void CheckKeyboard();
    void RefreshShieldPosition();
    void Reload();
    void RefreshImmortal();
    void Shoot();
public:
    Learning2DEngine::EventSystem::EventHandler<> onDead;

    void Reset(glm::vec2 position);
	void SetFrozen(bool frozen);
    void Hit(int damage = 1);

    void ResetLife();
	void RefreshLifeShield();
	void IncreaseMaxBulletNumber(int increase);
	void IncreaseSpeed(float increase);

	static PlayerController* Create(const glm::vec2& position, irrklang::ISoundEngine* soundEngine);
};

