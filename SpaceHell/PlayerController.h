#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>

constexpr glm::vec2 PLAYER_SIZE(96.0f, 96.0f);
constexpr float PLAYER_SPEED = 500.0f;

class PlayerController : protected Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	bool isFrozen;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;

public:
    void Reset(glm::vec2 position);
	void SetFrozen(bool frozen);

	static PlayerController* Create(const glm::vec2& position);
};

