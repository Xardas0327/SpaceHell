#pragma once

#include <glad/glad.h>

#include "../Render/Blend.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		struct ParticleSystemSettings
		{
			bool isUseBlend;
			Render::BlendFuncFactor blendFuncFactor;
			/// <summary>
			/// In seconds. The negative number is same as 0.
			/// If it is 0, it will spawn particles in every frame, which is not recommended.
			/// </summary>
			float spawnTime;
			unsigned int newParticlesPerSpawn;
			//In seconds. The negative number is same as 0.
			float delayBeforeStart;

			ParticleSystemSettings(
				bool isUseBlend = false,
				Render::BlendFuncFactor blendFuncFactor = Render::BlendFuncFactor(GL_ONE, GL_ZERO),
				float spawnTime = 1.0f, unsigned int newParticlesPerSpawn = 1, float delayBeforeStart = 0.0f)
				: isUseBlend(isUseBlend), blendFuncFactor(blendFuncFactor), spawnTime(spawnTime),
				newParticlesPerSpawn(newParticlesPerSpawn), delayBeforeStart(delayBeforeStart)
			{

			}
		};
	}
}