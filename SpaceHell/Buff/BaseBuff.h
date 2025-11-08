#pragma once

#include <string>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/System/LateUpdaterComponent.h>

#include "../PlayerController.h"

class BaseBuff : public Learning2DEngine::System::LateUpdaterComponent,
					public Learning2DEngine::Physics::CircleColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	const std::string textureId;
	const int animationLength;
	const float animationFrameLength;
	float lifeInSeconds;

	BaseBuff(
		Learning2DEngine::System::GameObject* gameObject,
		std::string&& textureId,
		float lifeInSeconds,
		int animationLength,
		float animationFrameLength = 0.1f
	);

	void Init() override;
	void LateUpdate() override;
	void Destroy() override;

	virtual void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;

	virtual void BuffPlayer(PlayerController* player) = 0;
};

