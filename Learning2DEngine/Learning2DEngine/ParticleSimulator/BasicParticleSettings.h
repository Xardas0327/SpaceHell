#pragma once

#include <glm/glm.hpp>

#include "ParticleSettings.h"
#include "../System/Transform.h"
#include "../System/Random.h"
#include "../System/Time.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		struct BasicParticleSettings : public ParticleSettings
		{
			glm::vec2 fixPositionOffset;
			/// <summary>
			/// First is the min.
			/// Second is the max.
			/// </summary>
			std::pair<glm::vec2, glm::vec2> randomPositionOffset;
			glm::vec2 scale;
			float rotation;
			glm::vec2 velocity;
			glm::vec4 color;
			//In seconds.
			float lifeTime;
			float speed;

			BasicParticleSettings() :
				fixPositionOffset(0.0f), randomPositionOffset(glm::vec2(0.0f), glm::vec2(0.0f)),
				scale(1.0f, 1.0f), rotation(0.0f), velocity(0.0f), color(1.0f), lifeTime(1.0f), speed(1.0f)
			{

			}

			void SpawnParticle(Particle& particle, const System::GameObject& gameObject) override
			{
				float positionRandomX = randomPositionOffset.first.x != randomPositionOffset.second.x
					? System::Random::GetNumber(randomPositionOffset.first.x, randomPositionOffset.second.x)
					: randomPositionOffset.first.x;

				float positionRandomY = randomPositionOffset.first.y != randomPositionOffset.second.y
					? System::Random::GetNumber(randomPositionOffset.first.y, randomPositionOffset.second.y)
					: randomPositionOffset.first.y;

				particle.transform = System::Transform(
					gameObject.transform.GetPosition() + fixPositionOffset + glm::vec2(positionRandomX, positionRandomY),
					scale,
					rotation
				);
				particle.velocity = velocity;
				particle.color = color;
				particle.lifeTime = lifeTime;
				particle.speed = speed;
			}

			void UpdateParticle(Particle& particle, const System::GameObject& gameObject) override
			{
				particle.transform.AddPosition(
					particle.velocity * particle.speed * System::Time::GetDeltaTime()
				);
			}
		};
	}
}