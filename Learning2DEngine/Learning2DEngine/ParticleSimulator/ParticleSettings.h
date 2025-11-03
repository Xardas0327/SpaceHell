#pragma once

#include "../System/GameObject.h"
#include "Particle.h"

namespace Learning2DEngine
{
    namespace ParticleSimulator
    {
        struct ParticleSettings
        {
            virtual ~ParticleSettings() {};

            //It will run in the ParticleSystemComponent::Start()
            virtual void Init(const System::GameObject& gameObject) {};
            //It will run in the ParticleSystemComponent::Stop()
            virtual void Destroy() {};

            //It will run in the ParticleSystemComponent::Update()
            virtual void SpawnParticle(Particle& particle, const System::GameObject& gameObject) = 0;
            /// <summary>
            /// It will run in the ParticleSystemComponent::Update().
            /// This function will be called with that particle, which life time is bigger than 0, after it was decreased.  
            /// </summary>
            virtual void UpdateParticle(Particle& particle, const System::GameObject& gameObject) = 0;
        };
    }
}