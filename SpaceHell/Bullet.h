#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

constexpr int32_t PLAYER_BULLET_MASK = 0B01;
constexpr int32_t ENEMY_BULLET_MASK = 0B10;

class Bullet : public Learning2DEngine::System::UpdaterComponent,
	public Learning2DEngine::Physics::CircleColliderComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
	glm::vec2 direction;
	float speed;
	std::string textureId;
	int animationLength;
	float animationFrameLength;

	Bullet(Learning2DEngine::System::GameObject* gameObject,
		const std::string& textureId,
		const glm::vec2& bulletScale,
		const glm::vec2& direction, 
		float speed, 
		int32_t mask,
		int animationLength,
		float animationFrameLength = 0.1f);

	void Init() override;
	void Update() override;
	void Destroy() override;
	void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;

	void CheckOutOfScreen();

public:
	bool isHeroBullet;

	static Bullet* Create(
		const glm::vec2& position,
		const glm::vec2& scale,
		float rotation,
		const std::string& textureId,
		const glm::vec2& direction,
		float speed,
		int32_t mask,
		int animationLength,
		float animationFrameLength = 0.1f);
};

