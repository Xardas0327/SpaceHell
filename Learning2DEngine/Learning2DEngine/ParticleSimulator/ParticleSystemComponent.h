#pragma once

#include <mutex>
#include <string>

#include "../Render/RendererComponent.h"
#include "../System/GameObject.h"
#include "../System/UpdaterComponent.h"
#include "ParticleRenderer.h"
#include "ParticleRenderData.h"
#include "ParticleSettings.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleSystemComponent final : public Render::RendererComponent<ParticleRenderData, ParticleRenderer>,
											public System::UpdaterComponent
		{
			friend class System::GameObject;
		private:
			bool isRunning;
			float delayTime;
			float nextSpawnTime;
			unsigned int lastUsedParticleIndex;

			ParticleSettings* particleSettings;

			static const std::string id;

			ParticleSystemComponent(
				System::GameObject* gameObject,
				Render::RendererMode mode,
				unsigned int particleAmount,
				ParticleSettings* particleSettings = nullptr,
				bool isUseCameraView = true,
				unsigned int minAllocateSize = 0,
				int renderLayer = 0);
			ParticleSystemComponent(
				System::GameObject* gameObject,
				Render::RendererMode mode,
				unsigned int particleAmount,
				const Render::Texture2D& texture,
				const ParticleSystemSettings& systemSettings,
				ParticleSettings* particleSettings = nullptr,
				bool isUseCameraView = true,
				unsigned int minAllocateSize = 0,
				int renderLayer = 0);

			void Init() override;
			void Destroy() override;
			void Update() override;
			const std::string& GetId() const override;
			ParticleRenderer* GetInitedRenderer() override;
			void DestroyRenderer() override;

			void UpdateActiveParticles();
			void TryToSpawnNewParticles();
			void SpawnNewParticles();
			unsigned int GetUnusedParticleIndex();
		public:
			~ParticleSystemComponent() override;

			void Start();
			void Stop();

			inline void Restart()
			{
				Stop();
				Start();
			}

			inline bool IsRunning()
			{
				return isRunning;
			}

			inline ParticleSettings* GetParticleSettings()
			{
				return particleSettings;
			}
		};
	}
}