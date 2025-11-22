#pragma once

#include <string>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/System/LateUpdaterComponent.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>

#include "../PlayerController.h"

#include "BuffType.h"

constexpr glm::vec2 BUFF_SIZE(32.0f, 32.0f);

class BaseBuff : public Learning2DEngine::System::LateUpdaterComponent,
					public Learning2DEngine::Physics::CircleColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	const std::string textureId;
	const int animationLength;
	const float animationFrameLength;
	float lifeInSeconds;
	float speed;
	BuffType buffType;

	BaseBuff(
		Learning2DEngine::System::GameObject* gameObject,
		std::string&& textureId,
		BuffType buffType,
		float lifeInSeconds,
		int animationLength,
		float animationFrameLength = 0.1f,
		float speed = 25.0f
	);

	void Init() override;
	void LateUpdate() override;
	void Destroy() override;

	virtual void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;

	virtual void BuffPlayer(PlayerController* player) = 0;
public:
	//The bool is true, when the player activated the buff, false when it just disappeared
	Learning2DEngine::EventSystem::EventHandler<BaseBuff*, bool> disappeared;

	virtual ~BaseBuff() = default;

	inline BuffType GetBuffType() const
	{
		return buffType;
	}
};

