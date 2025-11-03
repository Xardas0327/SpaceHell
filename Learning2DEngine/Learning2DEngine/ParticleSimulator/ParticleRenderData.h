#pragma once

#include "../Render/RenderData.h"
#include "../Render/Texture2D.h"
#include "../Render/Texture2DContainer.h"
#include "Particle.h"

#include "ParticleSystemSettings.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleSystemComponent;

		class ParticleRenderData final : public Render::RenderData, public Render::Texture2DContainer
		{
			friend class ParticleSystemComponent;
		private:
			bool isRenderable;
			const size_t minAllocateSize;
			const size_t particleAmount;
			Particle* particles;

		public:
			ParticleSystemSettings systemSettings;
			bool isUseCameraView;

			ParticleRenderData(const System::Component* component, size_t particleAmount, bool isUseCameraView = true, size_t minAllocateSize = 0)
				: RenderData(component), minAllocateSize(minAllocateSize), particleAmount(particleAmount), isRenderable(false),
				particles(nullptr), systemSettings(), isUseCameraView(isUseCameraView), Texture2DContainer()
			{
				particles = new Particle[particleAmount];
			}

			ParticleRenderData(
				const System::Component* component,
				size_t particleAmount,
				const ParticleSystemSettings& systemSettings,
				const Render::Texture2D& texture,
				bool isUseCameraView = true,
				size_t minAllocateSize = 0)
				: RenderData(component), minAllocateSize(minAllocateSize), particleAmount(particleAmount), isRenderable(false),
				particles(nullptr), systemSettings(systemSettings), isUseCameraView(isUseCameraView), Texture2DContainer(texture)
			{
				particles = new Particle[particleAmount];
			}

			~ParticleRenderData() override
			{
				delete[] particles;
			}

			inline bool IsRenderable() const
			{
				return isRenderable;
			}

			inline const Particle* GetParticles() const
			{
				return particles;
			}

			inline size_t GetParticleAmount() const
			{
				return particleAmount;
			}

			inline size_t GetMinAllocateSize() const
			{
				return minAllocateSize;
			}
		};
	}
}