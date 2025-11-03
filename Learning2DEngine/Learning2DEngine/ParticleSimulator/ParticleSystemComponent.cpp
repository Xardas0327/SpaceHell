#include "ParticleSystemComponent.h"

#include "BasicParticleSettings.h"

#include "../System/Time.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace ParticleSimulator
	{
		const std::string ParticleSystemComponent::id = "L2DE_ParticleSystemComponent";

		ParticleSystemComponent::ParticleSystemComponent(
			GameObject* gameObject,
			RendererMode mode,
			unsigned int particleAmount,
			ParticleSettings* particleSettings,
			bool isUseCameraView,
			unsigned int minAllocateSize,
			int renderLayer)
			: RendererComponent(gameObject, mode, renderLayer, particleAmount, isUseCameraView, minAllocateSize),
			UpdaterComponent(gameObject), Component(gameObject),
			isRunning(false), delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{
		}

		ParticleSystemComponent::ParticleSystemComponent(
			GameObject* gameObject,
			RendererMode mode,
			unsigned int particleAmount,
			const Texture2D& texture,
			const ParticleSystemSettings& systemSettings,
			ParticleSettings* particleSettings,
			bool isUseCameraView,
			unsigned int minAllocateSize,
			int renderLayer)
			: RendererComponent(gameObject, mode, renderLayer, particleAmount, systemSettings, texture, isUseCameraView, minAllocateSize),
			UpdaterComponent(gameObject), Component(gameObject),
			isRunning(false), delayTime(0.0f), nextSpawnTime(0.0f), lastUsedParticleIndex(0),
			particleSettings(particleSettings == nullptr ? new BasicParticleSettings() : particleSettings)
		{

		}

		ParticleSystemComponent::~ParticleSystemComponent()
		{
			if (IsRunning())
			{
				Stop();
			}
			if (particleSettings != nullptr)
			{
				delete particleSettings;
			}
		}

		void ParticleSystemComponent::Init()
		{
			RendererComponent::Init();
			UpdaterComponent::Init();
		}

		void ParticleSystemComponent::Destroy()
		{
			if (IsRunning())
			{
				Stop();
			}

			RendererComponent::Destroy();
			UpdaterComponent::Destroy();

		}

		void ParticleSystemComponent::Update()
		{

			if (!isRunning)
				return;

			if (delayTime > 0.0f)
			{
				delayTime -= Time::GetDeltaTime();
				return;
			}

			data.isRenderable = isRunning && delayTime <= 0.0f;

			UpdateActiveParticles();
			TryToSpawnNewParticles();
		}

		const std::string& ParticleSystemComponent::GetId() const
		{
			return ParticleSystemComponent::id;
		}

		ParticleRenderer* ParticleSystemComponent::GetInitedRenderer()
		{
			auto& renderer = ParticleRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void ParticleSystemComponent::DestroyRenderer()
		{
			ParticleRenderer::GetInstance().Destroy();
		}

		void ParticleSystemComponent::UpdateActiveParticles()
		{
			for (int i = 0; i < data.GetParticleAmount(); ++i)
			{
				data.particles[i].lifeTime -= Time::GetDeltaTime();
				if (data.particles[i].lifeTime > 0.0f)
				{
					particleSettings->UpdateParticle(data.particles[i], *gameObject);
				}
			}
		}

		void ParticleSystemComponent::TryToSpawnNewParticles()
		{
			nextSpawnTime -= Time::GetDeltaTime();

			float spawnTime = data.systemSettings.spawnTime < 0.0f ? 0.0f : data.systemSettings.spawnTime;

			if (spawnTime == 0.0f)
			{
				nextSpawnTime = 0.0f;
				SpawnNewParticles();
			}
			else
			{
				while (nextSpawnTime <= 0.0f)
				{
					nextSpawnTime += spawnTime;

					SpawnNewParticles();
				}
			}
		}

		void ParticleSystemComponent::SpawnNewParticles()
		{
			for (unsigned int i = 0; i < data.systemSettings.newParticlesPerSpawn; ++i)
			{
				unsigned int index = GetUnusedParticleIndex();
				particleSettings->SpawnParticle(data.particles[index], *gameObject);
			}
		}

		unsigned int ParticleSystemComponent::GetUnusedParticleIndex()
		{
			for (unsigned int i = lastUsedParticleIndex; i < data.GetParticleAmount(); ++i) {
				if (data.particles[i].lifeTime <= 0.0f) {
					lastUsedParticleIndex = i;
					return i;
				}
			}

			for (unsigned int i = 0; i < lastUsedParticleIndex; ++i) {
				if (data.particles[i].lifeTime <= 0.0f) {
					lastUsedParticleIndex = i;
					return i;
				}
			}

			//It has to have something. So this is the default.
			lastUsedParticleIndex = 0;
			return 0;
		}

		void ParticleSystemComponent::Start()
		{
			particleSettings->Init(*gameObject);

			delayTime = data.systemSettings.delayBeforeStart;
			nextSpawnTime = 0.0f;
			lastUsedParticleIndex = 0;
			isRunning = true;
		}

		void ParticleSystemComponent::Stop()
		{
			particleSettings->Destroy();
			isRunning = false;
			data.isRenderable = false;
		}
	}
}