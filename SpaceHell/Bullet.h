#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

constexpr glm::vec2 BULLET_SIZE(12.0f, 12.0f);
constexpr int BULLET_NUMBER = 4;
constexpr int32_t PLAYER_BULLET_MASK = 0B01;
constexpr int32_t ENEMY_BULLET_MASK = 0B10;

class Bullet : public Learning2DEngine::System::UpdaterComponent,
	public Learning2DEngine::Physics::CircleColliderComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
	static constexpr const glm::mat4x2 DefaultUV = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

	glm::vec2 direction;
	float speed;

	Bullet(Learning2DEngine::System::GameObject* gameObject, const glm::vec2& direction, float speed, int32_t mask);

	void Init() override;
	void Update() override;
	void Destroy() override;
	void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;

	void CheckOutOfScreen();

public:
	static Bullet* Create(const glm::vec2& position, const glm::vec2& direction, float speed, int32_t mask);
};

